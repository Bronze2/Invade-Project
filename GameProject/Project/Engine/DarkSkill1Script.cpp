#include "pch.h"
#include "DarkSkill1Script.h"

#include "PlayerScript.h"
CDarkSkill1Script::CDarkSkill1Script() :CScript((UINT)SCRIPT_TYPE::DARKSKILL1), m_bStart(false), m_bTickCheck(false)
{
}

CDarkSkill1Script::~CDarkSkill1Script()
{
}

void CDarkSkill1Script::Update()
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
			if (m_tInterval >= m_pSkill->fCoolTime) {
				m_bStart = false;
				m_bTickCheck = false;
				DeleteObject(GetObj());
			}
		}
	}

}
