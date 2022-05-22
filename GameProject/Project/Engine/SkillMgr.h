#pragma once
class CSkillMgr
{
private:
	map<int, SKILL*> m_mapSkill;


public:
	void AddSkill(int Code,SKILL* _pSkill);
	SKILL* FindSkill(int _strName);
	

	SINGLE(CSkillMgr)

};

