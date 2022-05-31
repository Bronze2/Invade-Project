#pragma once
enum class SKILL_CODE {
	WATER_0,
	WATER_1,
	WIND_0,
	WIND_1,
	DARK_0,
	DARK_1,
	THUNDER_0,
	THUNDER_1,
	FIRE_0,
	_FIRE_1
};

class CSkillMgr
{
private:
	map<int, SKILL*> m_mapSkill;


public:
	void AddSkill(int Code,SKILL* _pSkill);
	SKILL* FindSkill(int Code);
	

	SINGLE(CSkillMgr)

};

