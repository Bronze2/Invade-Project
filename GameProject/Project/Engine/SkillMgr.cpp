#include "pch.h"
#include "SkillMgr.h"

void CSkillMgr::AddSkill(int Code, SKILL* _pSkill)
{
	SKILL* pSkill = FindSkill(Code);
	if (nullptr != pSkill)
		assert(nullptr);


	m_mapSkill.insert(make_pair(Code, _pSkill));
	
}

SKILL* CSkillMgr::FindSkill(int Code)
{
	static map<int, SKILL*>::iterator iter;
	iter = m_mapSkill.find(Code);
	if (iter == m_mapSkill.end())
		return nullptr;

	return iter->second;
}

CSkillMgr::CSkillMgr(){}
CSkillMgr::~CSkillMgr() {
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i) {
		Safe_Delete_Map(m_mapSkill);
	}

}