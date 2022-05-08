#include "pch.h"
#include "TowerScript.h"
#include "Sensor.h"
#include "SensorMgr.h"
#include "GameObject.h"
#include "MinionScript.h"
#include "PlayerScript.h"
#include "Server.h"

void CTowerScript::Init()
{
	switch (m_eType)
	{
	case TOWER_TYPE::FIRST:
		break;
	case TOWER_TYPE::SECOND:
		break;
	case TOWER_TYPE::NEXUS:
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
				if (m_pTarget->GetScript<CMinionScript>() != nullptr) {
					if (CAMP_STATE::BLUE == m_eCampState)
						CreateProjectile( L"Blue");
					else if (CAMP_STATE::RED == m_eCampState)
						CreateProjectile(L"Red");
				}
				if (m_pTarget->GetScript<CPlayerScript>() != nullptr && m_pTarget->GetScript<CPlayerScript>()->GetCamp() != m_eCampState) {
					std::cout << " Found Player" << endl;
					if (CAMP_STATE::BLUE == m_eCampState )
						CreateProjectile(L"Blue");
					else if (CAMP_STATE::RED == m_eCampState)
						CreateProjectile(L"Red");
				}
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

		Transform()->SetLocalRot(vRot);
		SHARED_DATA::g_tower[m_GetId()] = vRot;
		CServer::GetInst()->send_rot_tower_packet(m_GetId());
	}
	
}

void CTowerScript::Update()
{
	FindNearObject(m_arrEnemy);
	m_FRotate();
	m_FAttack();
}

void CTowerScript::FinalUpdate()
{
}

void CTowerScript::OnDetectionEnter(CGameObject* _pOther)
{
	m_arrEnemy.push_back(_pOther);
	m_bFindNear = true;
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
{
}

CTowerScript::~CTowerScript()
{
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

#include "ProjectileScript.h"
#include "Collider3D.h"

void CTowerScript::CreateProjectile(const wstring& _Layer)
{
	if (nullptr == m_pTarget)
		return;
	if (m_pTarget->IsDead())
		return;
	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CProjectileScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pObject->GetScript<CProjectileScript>()->SetObject(GetObj());
	pObject->GetScript<CProjectileScript>()->SetDamage(m_uiAttackDamage);
	pObject->GetScript<CProjectileScript>()->SetProjectileType(PROJECTILE_TYPE::TOWER);

	Vec3 vPlayerTargetPos = Vec3(m_pTarget->Transform()->GetWorldPos().x, m_pTarget->Transform()->GetWorldPos().y + m_pTarget->Collider3D()->GetOffsetPos().z, m_pTarget->Transform()->GetWorldPos().z);
	Vec3 vMinionTargetPos = Vec3(m_pTarget->Transform()->GetWorldPos().x, m_pTarget->Transform()->GetWorldPos().y + m_pTarget->Collider3D()->GetOffsetPos().y, m_pTarget->Transform()->GetWorldPos().z);
	Vec3 vProjectileStartPos = Vec3(GetObj()->Transform()->GetWorldPos().x, GetObj()->Transform()->GetWorldPos().y + 185.f, GetObj()->Transform()->GetWorldPos().z);

	if (nullptr != m_pTarget->GetScript<CPlayerScript>()) {
		pObject->GetScript<CProjectileScript>()->SetTargetPos(vPlayerTargetPos);
	}
	else {
		pObject->GetScript<CProjectileScript>()->SetTargetPos(vMinionTargetPos);
	}

	pObject->Transform()->SetLocalPos(vProjectileStartPos);

	pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));

	Vec3 vDir;
	if (nullptr != m_pTarget->GetScript<CPlayerScript>()) {
		vDir = vPlayerTargetPos - vProjectileStartPos;
	}
	else {
		vDir = vMinionTargetPos - vProjectileStartPos;
	}
	vDir.Normalize();

	pObject->GetScript<CProjectileScript>()->SetDir(vDir);

	pObject->GetScript<CProjectileScript>()->Init();


	CreateObject(pObject, _Layer);

}