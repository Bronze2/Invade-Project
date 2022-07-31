#include "pch.h"
#include "ThunderSkill1Script.h"

#include "PlayerScript.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"
#include "MinionScript.h"
#include "SkillMgr.h"
#include "InGameScene.h"
#include "Network.h"

void CThunderSkill1Script::Collision()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject* pObject2 = GetObj();
	Vec3 vPos2 = pObject2->Transform()->GetWorldPos();

	for (int i = (UINT)INGAME_LAYER::BLUE; i < (UINT)INGAME_LAYER::RED + 1; ++i) {
		const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetParentObj();
		for (int j = 0; j < vecObj.size(); ++j) {
			CGameObject* pObject1 = vecObj[j];

			Vec3 vPos1 = pObject1->Transform()->GetWorldPos();
			float Radius = sqrt(pow(vPos1.x - vPos2.x, 2) + pow(vPos1.z - vPos2.z, 2));
			if (Radius < 400.f)
			{
				if (TickCheck(m_pSkill->StartTime, m_pSkill->Count) != m_pSkill->Count) {
					if (nullptr != vecObj[j]->GetScript<CPlayerScript>())
					{
						if (m_eCampState != vecObj[j]->GetScript<CPlayerScript>()->GetCamp()) {
							vecObj[j]->GetScript<CPlayerScript>()->GetDamage(m_pSkill->DotDamage);
							Network::GetInst()->send_set_damage(vecObj[j]->GetScript<CPlayerScript>()->m_GetId(),m_pSkill->DotDamage ,PACKET_COLLTYPE::PLAYER,m_eCampState);
						}
							m_pSkill->Count += 1;


					}
					else if (nullptr != vecObj[j]->GetScript<CMinionScript>()) {
						if (m_eCampState != vecObj[j]->GetScript<CMinionScript>()->GetCamp()) {
							Network::GetInst()->send_set_damage(vecObj[j]->GetScript<CMinionScript>()->m_GetId(), m_pSkill->DotDamage, PACKET_COLLTYPE::MONSTER, m_eCampState);


							//vecObj[j]->GetScript<CMinionScript>()->SetDamage(m_pSkill->DotDamage);
						}
						//vecObj[j]->GetScript<CMinionScript>()->SetDamage(m_pSkill->DotDamage);
					}

				}
			}
		}

	}


}
CThunderSkill1Script::CThunderSkill1Script() :CScript((UINT)SCRIPT_TYPE::THUNDERSKILL1), m_bStart(false), m_bTickCheck(false)
{
	m_pSkill = new SKILL;
	m_pSkill->DotDamage = CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::THUNDER_1)->DotDamage;
	m_pSkill->Code = CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::THUNDER_1)->Code;
	m_pSkill->Name = CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::THUNDER_1)->Name;
	m_pSkill->eSkillType = CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::THUNDER_1)->eSkillType;
	m_pSkill->eElementType = CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::THUNDER_1)->eElementType;

	m_pSkill->fCoolTime = CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::THUNDER_1)->fCoolTime;
	m_pSkill->fDuration = CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::THUNDER_1)->fDuration;
	m_pSkill->fDamage = CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::THUNDER_1)->fDamage;
	m_pSkill->bUse = false;
	m_pSkill->bFinal = false;
	m_pSkill->Count = 0;
	m_pSkill->Sum = CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::THUNDER_1)->Sum;

}

CThunderSkill1Script::~CThunderSkill1Script()
{
	if (nullptr != m_pSkill) {
		delete m_pSkill;
		m_pSkill = nullptr;
	}
}

void CThunderSkill1Script::Update()
{
	if (nullptr == m_pSkill)return;

	if (!m_bStart) {
		m_bStart = true;
		m_pSkill->StartTime = std::chrono::system_clock::now();
	}
	else {
		if(nullptr!=GetObj() && isMain)
			Collision();
		if (CoolTimeCheck(m_pSkill->StartTime, m_pSkill->fDuration)) {
			if(m_pSound != nullptr)
				m_pSound->Stop();
			//DeleteObject(GetObj()->GetChild()[0]);
			DeleteObject(GetObj());
			DeleteObject(m_pTarget);
		}
	}
	

}

void CThunderSkill1Script::OnCollision3DEnter(CCollider3D* _pColldier)
{
}
