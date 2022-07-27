#include "pch.h"
#include "MinionScript.h"
#include "Animator3D.h"
#include"ProjectileScript.h"
#include "Transform.h"
#include "Sensor.h"
#include "MeshRender.h"
#include "Collider3D.h"
#include "TowerScript.h"
#include "PlayerScript.h"

// Animation State (IDLE / WALK / ATTACK / DIE)
// enum class MINION_STATE (WALK / ATTACK / DIE / END)

void CMinionScript::Init()
{
	m_bDetection = false;
	m_fMinStartX = 990;
	m_fMaxStartX = 1010;
	m_fStartXValue = Transform()->GetLocalPos().x;
	m_eState = MINION_STATE::WALK;
	if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
		m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
		GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::WALK);
		GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
		GetObj()->Animator3D()->SetCurTime(0.f);
		GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
		m_eState = MINION_STATE::WALK;
		m_ePrevState = MINION_STATE::WALK;
	}
	switch (m_eAttackType)
	{
	case MINION_ATTACK_TYPE::MELEE: {
		SetAttackRange(200);
		m_uiMaxHp = 450; m_uiAttackDamage = 30;
	}
	break;

	case MINION_ATTACK_TYPE::RANGE: {
		SetAttackRange(250);
		m_uiMaxHp = 300; m_uiAttackDamage = 20;
		//	if (CAMP_STATE::BLUE == m_eCamp) {
		//		m_pProjectile = new CGameObject;
		//		m_pProjectile->AddComponent(new CTransform);
		//		m_pProjectile->AddComponent(new CMeshRender);
		//		m_pProjectile->AddComponent(new CProjectileScript);
		//		m_pProjectile->GetScript<CProjectileScript>()->SetObject(GetObj());
		//		CreateObject(m_pProjectile, L"Blue");
		//	}
	}
	break;

	case MINION_ATTACK_TYPE::CANON: {
		SetAttackRange(350);
		m_uiMaxHp = 550; m_uiAttackDamage = 60;
	}
	break;

	}
	m_iCurHp = m_uiMaxHp;
	m_pTarget = m_pNexus;
	m_pRay = new SimpleMath::Ray;
	m_fSpeed = 200.f;

	// 에이치피바
	m_pHPBar = new CGameObject;
	m_pHPBar->SetName(L"Minion HPBar");
	m_pHPBar->AddComponent(new CTransform);
	m_pHPBar->AddComponent(new CMeshRender);
	m_pHPBar->FrustumCheck(false);
	m_pHPBar->MeshRender()->SetDynamicShadow(false);

	Vec3 vHPBarScale = Vec3(60.f, 10.f, 1.f);
	m_pHPBar->Transform()->SetLocalScale(vHPBarScale);
	m_pHPBar->Transform()->SetLocalPos(Vec3(0.f, GetObj()->Collider3D()->GetOffsetPos().y + GetObj()->Collider3D()->GetOffsetScale().y + 30.f, 0.f));
	if (m_eCamp == CAMP_STATE::BLUE) {
		m_pHPBar->Transform()->SetLocalRot(Vec3(XMConvertToRadians(180.f), 0.f, 0.f));
	}
	else{
		m_pHPBar->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	}
	m_pHPBar->Transform()->SetBillBoard(true);
	m_pHPBar->Transform()->SetCamera(CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->GetParentObj()[1]->GetChild()[0]);

	m_pHPBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pUIHPBarMtrl = new CMaterial;
	pUIHPBarMtrl->DisableFileSave();
	pUIHPBarMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"HPBarShader"));
	m_pHPBar->MeshRender()->SetMaterial(pUIHPBarMtrl);
	GetObj()->AddChild(m_pHPBar);

	m_fCollisionCoolTime = 0.f;
	m_fMaxCollisionCoolTime = 5.f;
	m_fCollisionCoolTime = false;

	rotateCnt = 0;
}


void CMinionScript::Update()
{
	CheckObstacle();
	CheckHp();
	m_FAnimation();
	if (m_pNexus == nullptr)
		return;

	if (m_eState == MINION_STATE::DIE) {
		return;
	}
	FindNearObject(m_arrEnemy);
	CheckRange();
	Vec3 vPos = Transform()->GetWorldPos(); 
	Vec3 vTargetPos;
	Vec3 vRot = Transform()->GetLocalRot();
	if (nullptr != m_pTarget && !m_bAllienceCol) {
		Vec3 vTargetPos;
		if (!m_pTarget->IsDead()) {

			if (FIND_STATE::RAY_FIRST == m_eFindState) {
				float angle = atan2(vPos.x - m_fStartXValue, vPos.z - vPos.z) * (180 / PI);
				float rotate = angle * 0.0174532925f;
				vRot.y = rotate;

			}
			else {
				vTargetPos = m_pTarget->Transform()->GetWorldPos();
				float angle = atan2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z) * (180 / PI);
				float rotate = angle * 0.0174532925f;
				vRot.y = rotate;
			}
		}
	}
	if (m_bAllienceCol && !m_bSeparate) {
		if (m_bRotate) {

			vRot.y += PI / 2;
			m_bRotate = false;

			//m_bCollisionCoolCheck = true;
		}
	}

	/*if (m_bCollisionCoolCheck) {
		m_fCollisionCoolTime += DT;
		if (m_fCollisionCoolTime >= m_fMaxCollisionCoolTime) {
			m_bCollisionCoolCheck = false;
			m_fCollisionCoolTime = 0.f;
		}
	}*/



	Vec3 vLocalPos = Transform()->GetLocalPos();
	
	if (FIND_STATE::RAY_FIRST == m_eFindState)
	{
		if ((m_fMinStartX < vLocalPos.x) && (m_fMaxStartX > vLocalPos.x)) {
			m_eFindState = FIND_STATE::NONE;
			if (!m_pFirstTower->IsDead())
			{

				m_pTarget = m_pFirstTower;

			}
			else if (!m_pSecondTower->IsDead()) {

				m_pTarget = m_pSecondTower;

			}
			else
				m_pTarget = m_pNexus;
			m_arrEnemy.clear();
		}


	}

	switch (m_eState)
	{
	case MINION_STATE::WALK: {
		Vec3 vWorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vLocalPos.x -= vWorldDir.x * m_fSpeed * DT;
		vLocalPos.z -= vWorldDir.z * m_fSpeed * DT;
		Transform()->SetLocalPos(vLocalPos);
		Transform()->SetLocalRot(vRot);
	}
		break;
	case MINION_STATE::ATTACK:
	{
		if (m_ePrevState != m_eState) {

			if (FIND_STATE::RAY_FIRST == m_eFindState) {
				float angle = atan2(vPos.x - m_fStartXValue, vPos.z - vPos.z) * (180 / PI);
				float rotate = angle * 0.0174532925f;
				vRot.y = rotate;
				m_eState = MINION_STATE::WALK;
				m_arrEnemy.clear();
			}
			else {
				//Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
				//float angle = atan2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z) * (180 / PI);
				//float rotate = angle * 0.0174532925f;
				//vRot.y = rotate;
			}
		}
	}
		break;
	case MINION_STATE::DIE:
		break;
	default:
		break;
	}
	Transform()->SetLocalPos(vLocalPos);
	Transform()->SetLocalRot(vRot);

	m_pHPBar->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &m_iCurHp);
	m_pHPBar->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_1, &m_uiMaxHp);
	m_pHPBar->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_2, &m_eCamp);
}

#include "PlayerScript.h"

void CMinionScript::CreateProjectile(const wstring& _Key, const UINT& _Bone, const wstring& _Layer)
{
	if (nullptr == m_pTarget)
		return;
	if (m_pTarget->IsDead())
		return;
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(_Key, _Key);
	CGameObject* pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CProjectileScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->GetScript<CProjectileScript>()->SetObject(GetObj());
	pObject->GetScript<CProjectileScript>()->SetBone(_Bone);
	pObject->GetScript<CProjectileScript>()->SetDamage(m_uiAttackDamage);

	pObject->GetScript<CProjectileScript>()->SetProjectileType(PROJECTILE_TYPE::MINION);
	if (nullptr != m_pTarget->GetScript<CPlayerScript>()) {
		pObject->GetScript<CProjectileScript>()->SetTargetPos(Vec3(m_pTarget->Transform()->GetWorldPos().x, m_pTarget->Transform()->GetWorldPos().y + m_pTarget->Collider3D()->GetOffsetPos().z, m_pTarget->Transform()->GetWorldPos().z));
	}
	else {
		pObject->GetScript<CProjectileScript>()->SetTargetPos(Vec3(m_pTarget->Transform()->GetWorldPos().x, m_pTarget->Transform()->GetWorldPos().y + m_pTarget->Collider3D()->GetOffsetPos().y, m_pTarget->Transform()->GetWorldPos().z));
	}

	pObject->Transform()->SetLocalPos(Vec3(GetObj()->Transform()->GetWorldPos().x, GetObj()->Transform()->GetWorldPos().y + GetObj()->Collider3D()->GetOffsetPos().y, GetObj()->Transform()->GetWorldPos().z));

	pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
	pObject->GetScript<CProjectileScript>()->SetDir(GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT));

	CreateObject(pObject, _Layer);
}

void CMinionScript::InterSectsObject(CCollider3D* _pCollider)
{
	Vec3 vWorldPos = Transform()->GetWorldPos();
	vWorldPos.y += Collider3D()->GetOffsetPos().y;

	Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);


	if (nullptr == m_pTarget)
		return;
	if (nullptr == m_pTarget->GetScript<CPlayerScript>())		// 미니언 많이 나오면 여기서터짐 220727 am0230
		return;


	Vec3 target = m_pTarget->Transform()->GetWorldPos();
	target.y += m_pTarget->Collider3D()->GetOffsetPos().z;
	m_pRay->position = vWorldPos;
	m_pRay->direction = vDir;
	float min = INFINITE;
	if (_pCollider->GetObj()->GetName() != L"Obstacle")
	{

	}
	if (m_pRay->Intersects(_pCollider->GetBox(), min)) {
		Vec3 target = m_pTarget->Transform()->GetWorldPos();

		target.y += m_pTarget->Collider3D()->GetOffsetPos().z;
		float distance = Vec3::Distance(target, vWorldPos);
		Vec3 vPos2 = Vec3();

		float distance2 = Vec3::Distance(_pCollider->GetObj()->Transform()->GetWorldPos(), vWorldPos);
		if (min > distance)
			return;
		if (_pCollider->GetObj()->GetName() == L"obstacle") {
			m_InterSectObject = _pCollider->GetObj();

			m_eFindState = FIND_STATE::RAY_FIRST;

			m_fPrevXValue = vWorldPos.x;

		}

	}
}

void CMinionScript::OnDetectionEnter(CGameObject* _pOther)
{
	if (_pOther->GetLayerIdx() != GetObj()->GetLayerIdx()) {
		m_arrEnemy.push_back(_pOther);
		m_bFindNear = true;
	}
}

void CMinionScript::OnDetection(CGameObject* _pOther)
{
	
}

void CMinionScript::CheckObstacle()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	RayCollision(pCurScene->FindLayer(L"Obstacle"));

}

void CMinionScript::OnDetectionExit(CGameObject* _pOther)
{
	vector<CGameObject*>::iterator iter = m_arrEnemy.begin();

	for (int i = 0; iter != m_arrEnemy.end(); ++iter, ++i) {
		if (m_arrEnemy[i] == _pOther) {
			m_arrEnemy.erase(iter);
			if (Sensor()->GetDetectionCount() == 0) {
				m_bFindNear = false;
				if (!m_pFirstTower->IsDead())
				{

					m_pTarget = m_pFirstTower;

				}
				else if (!m_pSecondTower->IsDead()) {

					m_pTarget = m_pSecondTower;

				}
				else
					m_pTarget = m_pNexus;
			}
			else {
				FindNearObject(m_arrEnemy);
			}
			return;
		}

	}

}

void CMinionScript::RayCollision(const CLayer* _pLayer)
{
	const vector<CGameObject*>& vecObj = _pLayer->GetObjects();

	map<DWORD_PTR, bool>::iterator iter;
	for (size_t i = 0; i < vecObj.size(); ++i) {
		CCollider3D* pCollider = vecObj[i]->Collider3D();
		if (nullptr == pCollider)
			continue;
		InterSectsObject(pCollider);
	}



}

void CMinionScript::AddObject(CGameObject* _pObject)
{

	m_arrEnemy.push_back(_pObject);

}

void CMinionScript::CheckHp()
{
	if (m_iCurHp <= 0.f && !GetObj()->IsFallDown()) {
		m_eState = MINION_STATE::DIE;
		GetObj()->SetFallDown();
	}
}
#include "GameObject.h"
void CMinionScript::CheckRange()
{
	if (m_pTarget == nullptr)return;
	if (m_pTarget->IsDead())return;

	Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
	Vec3 vPos = Transform()->GetWorldPos();
	float length = sqrt(pow(vTargetPos.x - vPos.x, 2) + pow(vTargetPos.z - vPos.z, 2));
	if (m_pTarget->GetScript<CTowerScript>()) {
		float m_fTowerAttackRange = m_fAttackRange + 100.f;

		if (m_fTowerAttackRange >= length) {
			m_eState = MINION_STATE::ATTACK;
		}
		else {
			if (m_bFinishAnimation) {
				m_eState = MINION_STATE::WALK;
			}
		}
	}
	else {
		if (m_fAttackRange >= length) {
			m_eState = MINION_STATE::ATTACK;
		}
		else {
			if (m_bFinishAnimation) {
				m_eState = MINION_STATE::WALK;
			}
		}
	}
}

void  CMinionScript::FindNearObject(const vector<CGameObject*>& _pObject)
{
	if (FIND_STATE::RAY_FIRST == m_eFindState) {


		return;
	}

	if (0 == _pObject.size() || !m_bFindNear) return;

	for (int i = 0; i < _pObject.size(); ++i) {
		if (i == 0) {
			m_pTarget = _pObject[i];
		}
		else {
			if (L"Obstacle" == _pObject[i]->GetName())
				continue;
			Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
			Vec3 vPos = Transform()->GetWorldPos();
			float length1 = sqrt(pow(vTargetPos.x - vPos.x, 2) + pow(vTargetPos.z - vPos.z, 2));

			Vec3 vTargetPos2 = _pObject[i]->Transform()->GetWorldPos();
			float length2 = sqrt(pow(vTargetPos2.x - vPos.x, 2) + pow(vTargetPos2.z - vPos.z, 2));
			if (length1 > length2) {
				m_pTarget = _pObject[i];

				if (nullptr != m_pTarget->GetScript<CPlayerScript>())
					m_eFindState = FIND_STATE::SENSOR_FIRST;

			}
		}
	}

	if (m_arrEnemy.size() == 0) {

		if (!m_bDetection)
		{
			m_pTarget = m_pNexus;
		}
		else {
			if (!m_pFirstTower->IsDead())
			{

				m_pTarget = m_pFirstTower;

			}
			else if (!m_pSecondTower->IsDead()) {

				m_pTarget = m_pSecondTower;

			}
			else
				m_pTarget = m_pNexus;
		}

	}
}

void CMinionScript::m_FAnimation()
{
	m_bFinishAnimation = false;
	if (m_eState!=m_ePrevState)
	{
		switch (m_eState)
		{
		case MINION_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)MINION_STATE::IDLE);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::IDLE, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = m_eState;
			}
		}

		break;
		case MINION_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)MINION_STATE::WALK);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::WALK, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = m_eState;
			}
		}

		break;
		case MINION_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)MINION_STATE::ATTACK);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::ATTACK, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = m_eState;
			}
		}

		break;
		case MINION_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
				m_pNextAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
				GetObj()->Animator3D()->SetBlendState(true);
				GetObj()->Animator3D()->SetNextClipIndex((UINT)MINION_STATE::DIE);
				GetObj()->Animator3D()->SetNextFrameIdx(m_pNextAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::DIE, 0.f);
				GetObj()->Animator3D()->SetStartNextFrameTime(m_pNextAnimClip->dStartTime);
				m_ePrevState = m_eState;
			}
		}

		break;
		default:
			break;
		}
	}
	else {
		switch (m_eState)
		{
		case MINION_STATE::IDLE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"IDLE");
					if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame()) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::IDLE);
						GetObj()->Animator3D()->SetNextFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
						m_bFinishAnimation = true;
					}
				}
			}
		}

		break;
		case MINION_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {

				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
					if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame()) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::WALK);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
						m_bFinishAnimation = true;
					}
				}
			}
		}

		break;
		case MINION_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
					if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame - GetObj()->Animator3D()->GetBlendMaxFrame()) {
						GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::ATTACK);
						GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
						GetObj()->Animator3D()->SetCurTime(0.f);
						GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);

						m_bFindNear = true;
						m_bFinishAnimation = true;
						m_bProjectile = false;
						if (m_pTarget == nullptr) {
						}
						else
							if (m_pTarget->GetScript<CMinionScript>() != nullptr) {
								m_pTarget->GetScript<CMinionScript>()->GetDamage(m_uiAttackDamage);
							}
							else if (m_pTarget->GetScript<CTowerScript>() != nullptr) {
								m_pTarget->GetScript<CTowerScript>()->GetDamage(m_uiAttackDamage);
							}
					}
					if (m_eAttackType != MINION_ATTACK_TYPE::MELEE) {
						switch (m_eCamp)
						{
						case CAMP_STATE::BLUE:
						{
							if (MINION_ATTACK_TYPE::RANGE == m_eAttackType) {
								if (GetObj()->Animator3D()->GetFrameIdx() >= 68) {
									if (!m_bProjectile)
									{
										CreateProjectile(L"MeshData\\blueball.mdat", 6, L"Blue");
										m_bProjectile = true;
									}
								}
							}
							else {
								if (GetObj()->Animator3D()->GetFrameIdx() >= 64) {
									if (!m_bProjectile)
									{
										CreateProjectile(L"MeshData\\blueball.mdat", 17, L"Blue");
										m_bProjectile = true;
									}
								}
							}
						}
						break;
						case CAMP_STATE::RED:
						{
							if (MINION_ATTACK_TYPE::RANGE == m_eAttackType) {
								if (GetObj()->Animator3D()->GetFrameIdx() >= 59) {
									if (!m_bProjectile)
									{
										CreateProjectile(L"MeshData\\redball.mdat", 10, L"Red");
										m_bProjectile = true;
									}
								}
							}
							else {
								if (GetObj()->Animator3D()->GetFrameIdx() >= 84) {
									if (!m_bProjectile)
									{
										CreateProjectile(L"MeshData\\redball.mdat", 12, L"Red");

										m_bProjectile = true;
									}
								}
							}
						}
						break;

						default:
							break;
						}
					}
					//
				}
			}
		}

		break;
		case MINION_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {

				if (!GetObj()->Animator3D()->GetBlendState()) {
					m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
					if (GetObj()->Animator3D()->GetFrameIdx() >= (m_pCurAnimClip->iEndFrame - 1) || m_pCurAnimClip->iStartFrame > GetObj()->Animator3D()->GetFrameIdx()) {
						DeleteObject(GetObj());
						GetObj()->MeshRender()->SetRender(false);
						DeleteObject(m_pHPBar);
					}
				}
			}
		}

		break;
		default:
			break;
		}

	}
}

void CMinionScript::m_FFind()
{
}

CMinionScript::CMinionScript():CScript((UINT)SCRIPT_TYPE::MINIONSCRIPT),m_eState(MINION_STATE::WALK)
{
}

CMinionScript::CMinionScript(float _fSpeed, float _fRange, MINION_STATE _eState, CAMP_STATE _eCamp)
	: CScript((UINT)SCRIPT_TYPE::MINIONSCRIPT),m_fSpeed(_fSpeed),m_fRange(_fRange),m_eState(_eState),m_eCamp(_eCamp), m_bAllienceCol(false),m_bFinishAnimation(false)
	,m_bRotate(false),m_bSeparate(false), m_bProjectile(false)
{
}

CMinionScript::~CMinionScript()
{
}

void CMinionScript::SetDamage(const int& _Damage)
{
	m_iCurHp -= _Damage;
	if (m_iCurHp < 0) {
		m_iCurHp = 0;
	}
	if (m_iCurHp > m_uiMaxHp)
		m_iCurHp = m_uiMaxHp;
}

#include "Collider3D.h"

static bool iSeparate=true;

void CMinionScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	if (_pOther->GetObj()->GetScript<CMinionScript>() == nullptr) {
		if (nullptr != _pOther->GetObj()->GetScript<CTowerScript>()) {	
			if ((UINT)_pOther->GetObj()->GetScript<CTowerScript>()->GetCamp() != (UINT)m_eCamp) {
				m_eState = MINION_STATE::ATTACK;
				m_pTarget = _pOther->GetObj();
			}
		}
	}
	else {
		if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() == m_eCamp && m_eState == MINION_STATE::WALK) {
			m_bAllienceCol = true;
			m_bRotate = true;
			if (_pOther->GetObj()->GetScript<CMinionScript>()->GetState() == MINION_STATE::WALK) {
				float Value = _pOther->Transform()->GetLocalPos().x - Transform()->GetLocalPos().x;
				Vec3 Value3 = _pOther->Transform()->GetLocalPos() - Transform()->GetLocalPos();
				Vec3 Up = Transform()->GetLocalDir(DIR_TYPE::UP);
				Vec3 F = Transform()->GetLocalDir(DIR_TYPE::FRONT);
				Vec3 Value2 = F * Value3;
				float DotValue = Dot(Up, Value2);
				if (DotValue >= 0) {
					_pOther->GetObj()->GetScript<CMinionScript>()->SetSeparate(false);
					SetSeparate(true);
				}
				else {
					SetSeparate(false);
					_pOther->GetObj()->GetScript<CMinionScript>()->SetSeparate(true);
				}

			}
		}
		m_iAllienceCol += 1;
	}
}




void CMinionScript::OnCollision3D(CCollider3D* _pOther)
{
}

void CMinionScript::OnCollision3DExit(CCollider3D* _pOther)
{
	if (_pOther->GetObj()->GetScript<CMinionScript>() == nullptr) {

	}
	else {
		if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() == m_eCamp && m_eState == MINION_STATE::WALK) {
			
		
		}
		m_iAllienceCol -= 1;
		if (m_iAllienceCol <= 0) {
			m_iAllienceCol = 0;
		}
		if (m_iAllienceCol == 0) {
			m_bAllienceCol = false;
		}
	}

}
