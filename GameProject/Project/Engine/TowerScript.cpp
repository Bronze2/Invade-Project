#include "pch.h"
#include "TowerScript.h"
#include "Sensor.h"
#include "SensorMgr.h"
#include "GameObject.h"
#include "MinionScript.h"
#include "PlayerScript.h"
#include "SceneMgr.h"

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

	CSceneMgr::GetInst()->set_towerRot(m_GetId(), GetObj()->Transform()->GetLocalRot());
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
					m_pTarget->GetScript<CMinionScript>()->GetDamage(150);
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

	}
	Transform()->SetLocalRot(vRot);
}

void CTowerScript::Update()
{

	Vec3 vRot = Vec3::Lerp(Transform()->GetLocalRot(), CSceneMgr::GetInst()->get_towerRot(m_GetId()), DT * 10.f);

	Transform()->SetLocalRot(vRot);

	//if (KEY_TAB(KEY_TYPE::KEY_ENTER)) {
	//	ChangeScene(SCENE_TYPE::INGAME);
	//}
	//FindNearObject(m_arrEnemy);
	//m_FRotate();
	//m_FAttack();
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
