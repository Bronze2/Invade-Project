#include "pch.h"
#include "ThunderSkill0Script.h"

#include "PlayerScript.h"
CThunderSkill0Script::CThunderSkill0Script() :CScript((UINT)SCRIPT_TYPE::THUNDERSKILL0), m_bStart(false), m_bTickCheck(false)
{
}

CThunderSkill0Script::~CThunderSkill0Script()
{
}

void CThunderSkill0Script::Update()
{
	if (nullptr == m_pSkill)return;
	if (nullptr != m_pTarget->GetScript<CPlayerScript>()) {
		if (!m_bStart) {
			m_bStart = true;
			m_tStart = clock();

		}
		else {
			m_tEnd = clock();
			m_tInterval = m_tEnd - m_tStart / CLOCKS_PER_SEC;
			if (!m_bTickCheck) {
				m_tTickInterval = m_tEnd - m_tStart / CLOCKS_PER_SEC;

			}
			else {
				m_tTickInterval = m_tEnd - m_tTickStart / CLOCKS_PER_SEC;
			}
			if (m_tTickInterval >= 1.f) {
				m_pTarget->GetScript<CPlayerScript>()->SetDamage(m_pSkill->DotDamage);
				m_tTickStart = clock();
				m_bTickCheck = true;
			}
			if (m_tInterval >= m_pSkill->fCoolTime) {
				m_bStart = false;
				m_bTickCheck = false;
				DeleteObject(GetObj());
			}
		}
	}

}

void CThunderSkill0Script::OnCollision3DEnter(CCollider3D* _pColldier)
{
}
