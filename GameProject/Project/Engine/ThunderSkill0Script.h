#pragma once
#include "Script.h"
class CThunderSkill0Script :
	public CScript
{
private:
	SKILL* m_pSkill;

	CGameObject* m_pTarget;
	bool m_bStart;
	clock_t m_tStart;
	clock_t m_tTickStart;
	clock_t m_tEnd;
	clock_t m_tInterval;
	clock_t m_tTickInterval;
	bool m_bTickCheck;
public:
	void SetTarget(CGameObject* _pTarget) { m_pTarget = _pTarget; }
	CThunderSkill0Script();
	virtual ~CThunderSkill0Script();

	virtual void Update();


	CLONE(CThunderSkill0Script)


};

