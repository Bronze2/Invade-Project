#include "pch.h"
#include "WindSkill1Script.h"

#include "PlayerScript.h"
CWindSkill1Script::CWindSkill1Script() :CScript((UINT)SCRIPT_TYPE::WINDSKILL1), m_bStart(false), m_bTickCheck(false)
{
}

CWindSkill1Script::~CWindSkill1Script()
{
}

void CWindSkill1Script::Update()
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
