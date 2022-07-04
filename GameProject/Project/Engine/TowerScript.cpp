#include "pch.h"
#include "PathMgr.h"
#include "TowerScript.h"
#include "Sensor.h"
#include "SensorMgr.h"
#include "GameObject.h"
#include "MinionScript.h"
#include "Collider3D.h"
#include "PlayerScript.h"
#include "ProjectileScript.h"

void CTowerScript::CreateProjectile(const wstring& _Key, const wstring& _Layer)
{
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
	pObject->GetScript<CProjectileScript>()->SetDamage(m_uiAttackDamage);

	pObject->Transform()->SetLocalPos(Vec3(0.f, 185.f, -70.f));


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
//	pObject->GetScript<CProjectileScript>()->SetRotY(vRot.y);
	pObject->GetScript<CProjectileScript>()->SetMatrixObject(Matrix);
	pObject->GetScript<CProjectileScript>()->SetStartPos(Matrix.Translation());
	pObject->GetScript<CProjectileScript>()->SetProjectileType(PROJECTILE_TYPE::TOWER);
	if (nullptr != m_pTarget->GetScript<CPlayerScript>()) {
		pObject->GetScript<CProjectileScript>()->SetTargetPos(Vec3(m_pTarget->Transform()->GetWorldPos().x, m_pTarget->Transform()->GetWorldPos().y + m_pTarget->Collider3D()->GetOffsetPos().z, m_pTarget->Transform()->GetWorldPos().z));
	}
	else {
		pObject->GetScript<CProjectileScript>()->SetTargetPos(Vec3(m_pTarget->Transform()->GetWorldPos().x, m_pTarget->Transform()->GetWorldPos().y + m_pTarget->Collider3D()->GetOffsetPos().y, m_pTarget->Transform()->GetWorldPos().z));
	}

	pObject->GetScript<CProjectileScript>()->SetLength(500.f);
	pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(0.1f, 0.1f, 0.1f));
	pObject->GetScript<CProjectileScript>()->Init();
	pObject->GetScript<CProjectileScript>()->SetSpeed(2000.f);
	pObject->GetScript<CProjectileScript>()->SetTarget(m_pTarget);

	CreateObject(pObject, _Layer);
}

void CTowerScript::Init()
{
	wstring strPath = L"Sound\\TowerHit.wav";
	wstring strFullPath = CPathMgr::GetResPath();
	switch (m_eType)
	{
	case TOWER_TYPE::FIRST:
		m_iMaxHp = 1500;
		m_iCurHp = m_iMaxHp;
		m_uiAttackDamage = 150;
	
	
		strFullPath += strPath;
		m_pSound =new CSound;
		m_pSound->Load3D(strFullPath);
		
		break;
	case TOWER_TYPE::SECOND:
		m_iMaxHp = 2000;
		m_iCurHp = m_iMaxHp;
		m_uiAttackDamage = 150;
		strFullPath += strPath;
		m_pSound = new CSound;
		m_pSound->Load3D(strFullPath);
		break;
	case TOWER_TYPE::NEXUS:

		m_iMaxHp = 5000;
		m_iCurHp = m_iMaxHp;
		break;
	default:
		break;
	}




}

void CTowerScript::SetSecondTower(CGameObject* _pGameObject)
{
	if (m_eType == TOWER_TYPE::SECOND) {
		assert(false);
	}
	m_pSecondTower = _pGameObject;

}

void CTowerScript::SetFirstTower(CGameObject* _pGameObject)
{
	if (m_eType == TOWER_TYPE::FIRST) {
		assert(false);
	}
	m_pFirstTower = _pGameObject;
}

void CTowerScript::SetNexus(CGameObject* _pObject)
{
	if (m_eType == TOWER_TYPE::NEXUS) {
		assert(false);
	}
	m_pNexus = _pObject;
}

void CTowerScript::m_FAttack()
{
	if (nullptr != m_pTarget) {
		if (!m_bAttackStart) {
			m_bAttackStart = true;
			m_cAttackStart = clock();
		}
		else {
			m_cAttackEnd = clock();
			m_cAttackInterval = (m_cAttackEnd - m_cAttackStart) / CLOCKS_PER_SEC;
			if (m_cAttackInterval >= ATTACK_INTERVAL) {

				Vec3 vPos=Transform()->GetWorldPos();
				m_pSound->PlaySound3D(vPos,1000.f);
				if (CAMP_STATE::BLUE == m_eCampState)
					CreateProjectile(L"MeshData\\blueball.mdat", L"Blue");
				else if(CAMP_STATE::RED==m_eCampState)
					CreateProjectile(L"MeshData\\redball.mdat", L"Red");
			
				m_bAttackStart = false;
			}
		}
	}
}

void CTowerScript::m_FRotate()
{
	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vRot=Transform()->GetLocalRot();
	if (nullptr != m_pTarget) {
		Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
		float angle = atan2(vPos.x - vTargetPos.x, vPos.z - vTargetPos.z) * (180 / PI);
		float rotate = angle * 0.0174532925f;
		vRot.y = rotate;

	}
	Transform()->SetLocalRot(vRot);
}



void CTowerScript::Update()
{
	if (m_iCurHp <= 0) {
		DeleteObject(GetObj());
		return;
	}
	FindNearObject(m_arrEnemy);
	m_FRotate();
	m_FAttack();
}

void CTowerScript::FinalUpdate()
{
}

void CTowerScript::OnDetectionEnter(CGameObject* _pOther)
{
	if (_pOther->GetLayerIdx() != GetObj()->GetLayerIdx()) {
		m_arrEnemy.push_back(_pOther);
		m_bFindNear = true;
	}
	else {
	
	}
}

void CTowerScript::OnDetection(CGameObject* _pOther)
{
}

void CTowerScript::OnDetectionExit(CGameObject* _pOther)
{
	vector<CGameObject*>::iterator iter = m_arrEnemy.begin();
	for (int i = 0; iter != m_arrEnemy.end(); ++iter, ++i) {
		if (m_arrEnemy[i] == _pOther) {
			m_arrEnemy.erase(iter);
			if (m_arrEnemy.size() == 0) {
				m_bFindNear = false;
				m_pTarget = nullptr;
			}
			return;
		}

	}
}

CTowerScript::CTowerScript():CScript((UINT)SCRIPT_TYPE::TOWERSCRIPT), m_bAttackStart(false), m_bFindNear(false)
,m_bTest(false)
{
}

CTowerScript::~CTowerScript()
{
	delete m_pSound;
	m_pSound = nullptr;
}


void CTowerScript::FindNearObject(const vector<CGameObject*>& _pObject)
{
	if (0 == _pObject.size() || !m_bFindNear)return;
	for (int i = 0; i < _pObject.size(); ++i) {
		if (i == 0) {
			m_pTarget = _pObject[i];
		}
		else {
			Vec3 vTargetPos = m_pTarget->Transform()->GetWorldPos();
			Vec3 vPos = Transform()->GetWorldPos();
			float length1 = sqrt(pow(vTargetPos.x - vPos.x, 2) + pow(vTargetPos.z - vPos.z, 2));

			Vec3 vTargetPos2 = _pObject[i]->Transform()->GetWorldPos();
			float length2 = sqrt(pow(vTargetPos2.x - vPos.x, 2) + pow(vTargetPos2.z - vPos.z, 2));
			if (length1 > length2) {
				m_pTarget = _pObject[i];
			}
		}
	}

}
