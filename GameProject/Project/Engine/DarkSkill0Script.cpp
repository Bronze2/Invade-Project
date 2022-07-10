#include "pch.h"
#include "DarkSkill0Script.h"

#include "PlayerScript.h"
#include "MeshRender.h"
#include "RenderMgr.h"

CDarkSkill0Script::CDarkSkill0Script() :CScript((UINT)SCRIPT_TYPE::DARKSKILL0), m_bStart(false), m_bTickCheck(false)
{
}

CDarkSkill0Script::~CDarkSkill0Script()
{
}

void CDarkSkill0Script::Awake() 
{
}

void CDarkSkill0Script::Update()
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
