#include "pch.h"
#include "MinionScript.h"
#include "Animator3D.h"
#include"ProjectileScript.h"
#include "Transform.h"
#include "Sensor.h"
#include "MeshRender.h"
#include "Collider3D.h"
#include "TowerScript.h"

// Animation State (IDLE / WALK / ATTACK / DIE)
// enum class MINION_STATE (WALK / ATTACK / FIND / DIE / END)

void CMinionScript::Init()
{
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
	case MINION_ATTACK_TYPE::MELEE:{
		SetAttackRange(50);
		m_uiMaxHp = 450; m_uiAttackDamage = 30;
		}
		break;

	case MINION_ATTACK_TYPE::RANGE: {
		SetAttackRange(150);
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

}


void CMinionScript::Update()
{

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
	if (nullptr != m_pTarget&&!m_bAllienceCol) {
		Vec3 vTargetPos;
		if (!m_pTarget->IsDead()) {
			vTargetPos = m_pTarget->Transform()->GetWorldPos();
			float angle = atan2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z) * (180 / PI);
			float rotate = angle * 0.0174532925f;
			vRot.y = rotate;
		}
	}
	if (m_bAllienceCol&&!m_bSeparate) {
		if (m_bRotate) {
			if (m_eCamp == CAMP_STATE::RED) {
				vRot.y -= PI / 2;
			}
			else {
				vRot.y += PI / 2;
			}
			m_bRotate = false;
		}
	}


	Vec3 vLocalPos = Transform()->GetLocalPos();
	

	switch (m_eState)
	{
	case MINION_STATE::WALK: {
		Vec3 vWorldDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vLocalPos.x -= vWorldDir.x * 100.f * DT;
		vLocalPos.z -= vWorldDir.z * 100.f * DT;
		Transform()->SetLocalPos(vLocalPos);
		Transform()->SetLocalRot(vRot);
	}
		break;
	case MINION_STATE::ATTACK:
	{
		if (m_ePrevState != m_eState) {
			Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
			float angle = atan2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z) * (180 / PI);
			float rotate = angle * 0.0174532925f;
			vRot.y = rotate;
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



}



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

	pObject->GetScript<CProjectileScript>()->SetTargetPos(Vec3(m_pTarget->Transform()->GetWorldPos().x, m_pTarget->Transform()->GetWorldPos().y + m_pTarget->Collider3D()->GetOffsetPos().y, m_pTarget->Transform()->GetWorldPos().z));
	tMTBone* p = const_cast<tMTBone*>(MeshRender()->GetMesh()->GetBone(_Bone));//6
	pObject->Transform()->SetLocalPos(p->vecKeyFrame[Animator3D()->GetFrameIdx()].vTranslate);


	Vec3 vPos = Transform()->GetLocalPos();
	Matrix matTranslation = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	Vec3 vScale = Transform()->GetLocalScale();
	Matrix matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	Vec3 vRot = Transform()->GetLocalRot();
	Matrix matRot = XMMatrixRotationX(vRot.x);
	matRot *= XMMatrixRotationY(vRot.y);
	matRot *= XMMatrixRotationZ(vRot.z);
	Matrix Matrix = matScale * matRot * matTranslation;
	pObject->Transform()->SetNotParent(true);
	pObject->Transform()->SetObjectMatrix(Matrix);
	pObject->GetScript<CProjectileScript>()->SetMatrixObject(Matrix);

	//		GetObj()->AddChild(pObject);
	//		pObject->Transform()->SetLocalPos(Vec3(0.f, 50.f, -100.f));
	pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(0.1f, 0.1f, 0.1f));
	pObject->GetScript<CProjectileScript>()->Init();
	CreateObject(pObject, _Layer);
}

void CMinionScript::OnDetectionEnter(CGameObject* _pOther)
{
	m_arrEnemy.push_back(_pOther);
	m_bFindNear = true;
}

void CMinionScript::OnDetection(CGameObject* _pOther)
{
	
}

void CMinionScript::OnDetectionExit(CGameObject* _pOther)
{
	vector<CGameObject*>::iterator iter=m_arrEnemy.begin();
	for (int i=0; iter != m_arrEnemy.end(); ++iter,++i) {
		if (m_arrEnemy[i] == _pOther) {
			m_arrEnemy.erase(iter);
			if (Sensor()->GetDetectionCount() == 0) {
				m_bFindNear = false;
				m_pTarget = m_pNexus;
			}
			else {
				FindNearObject(m_arrEnemy);
			}
			return;
		}
		
	}

}

void CMinionScript::AddObject(CGameObject* _pObject)
{

	m_arrEnemy.push_back(_pObject);

}

void CMinionScript::CheckHp()
{
	if (m_iCurHp <= 0.f&&!GetObj()->IsFallDown()) {
		m_eState = MINION_STATE::DIE;
		GetObj()->SetFallDown();
	}
}
#include "GameObject.h"
void CMinionScript::CheckRange()
{
	
	if (m_pTarget == nullptr)return;
	if (m_pTarget->IsDead())return;
	Vec3 vTargetPos=m_pTarget->Transform()->GetWorldPos();
	Vec3 vPos = Transform()->GetWorldPos();
	float length = sqrt(pow(vTargetPos.x - vPos.x, 2) + pow(vTargetPos.z - vPos.z, 2));
	if (m_fAttackRange >= length) {
		m_eState = MINION_STATE::ATTACK;
	}
	/*else {
		if (m_bFinishAnimation) {
			
			
			m_eState = MINION_STATE::WALK;
			
			
		}
		
	}*/
}

void  CMinionScript::FindNearObject(const vector<CGameObject*>& _pObject)
{
	if (m_arrEnemy.size() == 0) {
		m_pTarget = m_pNexus;
	}

	if (0 == _pObject.size()||!m_bFindNear)return;
	for (int i = 0; i < _pObject.size(); ++i) {
		if (i == 0) {
			m_pTarget = _pObject[i];
		}
		else {
			Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
			Vec3 vPos = Transform()->GetWorldPos();
			float length1= sqrt(pow(vTargetPos.x - vPos.x, 2) + pow(vTargetPos.z - vPos.z, 2));

			Vec3 vTargetPos2 = _pObject[i]->Transform()->GetWorldPos();
			float length2 = sqrt(pow(vTargetPos2.x - vPos.x, 2) + pow(vTargetPos2.z - vPos.z, 2));
			if (length1 > length2) {
				m_pTarget = _pObject[i];
			}
		}
	}



}

void CMinionScript::m_FAnimation()
{
#ifdef _ANIMATION_TEST
	m_bFinishAnimation = false;
	if (m_eState!=m_ePrevState)
	{
		switch (m_eState)
		{
		case MINION_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK");
				GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::WALK);
				GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::WALK, 0.f);
				GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
				m_ePrevState = m_eState;
			}
		}

		break;
		case MINION_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK");
				GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::ATTACK);
				GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::ATTACK, 0.f);
				GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
				m_ePrevState = m_eState;
			}
		}

		break;
		case MINION_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
				m_pCurAnimClip = GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE");
				GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::DIE);
				GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
				GetObj()->Animator3D()->SetCurTime((UINT)MINION_STATE::DIE, 0.f);
				GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
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
		case MINION_STATE::WALK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"WALK")) {
				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame - 1) {
					GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::WALK);
					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
					GetObj()->Animator3D()->SetCurTime(0.f);
					GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);
					//m_bFinishAnimation = true;
				}
			}
		}

		break;
		case MINION_STATE::ATTACK:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"ATTACK")) {
				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iEndFrame) {
					GetObj()->Animator3D()->SetCurClipIndex((UINT)MINION_STATE::ATTACK);
					GetObj()->Animator3D()->SetFrameIdx(m_pCurAnimClip->iStartFrame);
					GetObj()->Animator3D()->SetCurTime(0.f);
					GetObj()->Animator3D()->SetStartFrameTime(m_pCurAnimClip->dStartTime);

					m_bFindNear = true;
					//m_bFinishAnimation = true;
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

		break;
		case MINION_STATE::DIE:
		{
			if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimClip(L"DIE")) {
				if (GetObj()->Animator3D()->GetFrameIdx() >= m_pCurAnimClip->iFrameLength || m_pCurAnimClip->iStartFrame > GetObj()->Animator3D()->GetFrameIdx()) {
					DeleteObject(GetObj());
				}
			}
		}

		break;
		default:
			break;
		}

	}
#else
m_bFinishAnimation = false;
if (m_eState != m_ePrevState)
{
	switch (m_eState)
	{
	case MINION_STATE::WALK:
	{
		if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK")) {
			m_CurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK");
			GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
			double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
			GetObj()->Animator3D()->SetCurTime(0.f);
			GetObj()->Animator3D()->SetStartFrameTime(time);
			m_ePrevState = m_eState;
		}
	}

	break;
	case MINION_STATE::ATTACK:
	{
		if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK")) {
			m_CurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK");
			GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
			double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
			GetObj()->Animator3D()->SetCurTime(0.f);
			GetObj()->Animator3D()->SetStartFrameTime(time);
			m_ePrevState = m_eState;
		}
	}

	break;
	case MINION_STATE::DIE:
	{
		if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"DIE")) {
			m_CurAnimation = GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"DIE");
			GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
			double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
			GetObj()->Animator3D()->SetCurTime(0.f);
			GetObj()->Animator3D()->SetStartFrameTime(time);
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
	case MINION_STATE::WALK:
	{
		if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"WALK")) {
			if (GetObj()->Animator3D()->GetFrameIdx() >= m_CurAnimation->EndFrame) {
				GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
				GetObj()->Animator3D()->SetCurTime(0.f);
				GetObj()->Animator3D()->SetStartFrameTime(time);
				m_bFinishAnimation = true;
			}
		}
	}

	break;
	case MINION_STATE::ATTACK:
	{
		if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"ATTACK")) {
			if (GetObj()->Animator3D()->GetFrameIdx() >= m_CurAnimation->EndFrame) {
				GetObj()->Animator3D()->SetFrmaeIdx(m_CurAnimation->StartFrame);
				double time = (double)GetObj()->Animator3D()->GetFrameIdx() / (double)GetObj()->Animator3D()->GetFrameCount();
				GetObj()->Animator3D()->SetCurTime(0.f);
				GetObj()->Animator3D()->SetStartFrameTime(time);
				m_bFindNear = true;
				m_bFinishAnimation = true;
				if (m_pTarget == nullptr) {

				}
				else
					if (m_pTarget->GetScript<CMinionScript>() != nullptr) {
						m_pTarget->GetScript<CMinionScript>()->GetDamage(m_uiAttackDamage);
					}
			}
		}
	}

	break;
	case MINION_STATE::DIE:
	{
		if (nullptr != GetObj()->Animator3D()->GetAnimation()->FindAnimation(L"DIE")) {
			if (GetObj()->Animator3D()->GetFrameIdx() >= m_CurAnimation->EndFrame || m_CurAnimation->StartFrame > GetObj()->Animator3D()->GetFrameIdx()) {
				DeleteObject(GetObj());

			}
		}
	}

	break;
	default:
		break;
	}

}
#endif
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

#include "Collider3D.h"

static bool iSeparate=true;

void CMinionScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	if (_pOther->GetObj()->GetScript<CMinionScript>() == nullptr) {
	
	}
	else {
		if (_pOther->GetObj()->GetScript<CMinionScript>()->GetCamp() == m_eCamp&&m_eState==MINION_STATE::WALK) {
			m_bAllienceCol = true;
			m_bRotate = true;
			if (_pOther->GetObj()->GetScript<CMinionScript>()->GetState() == MINION_STATE::WALK) {
				float Value=_pOther->Transform()->GetLocalPos().x - Transform()->GetLocalPos().x;
				if (Value >= 0) {
					_pOther->GetObj()->GetScript<CMinionScript>()->SetSeparate(false);
					GetObj()->GetScript<CMinionScript>()->SetSeparate(true);
				}
				else {
					GetObj()->GetScript<CMinionScript>()->SetSeparate(false);
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
