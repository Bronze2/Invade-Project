#pragma once
#include "Script.h"
class CWaterSkill1Script :
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
	void SetSkill(SKILL* _pSkill) { m_pSkill = _pSkill; }
	void SetTarget(CGameObject* _pTarget) { m_pTarget = _pTarget; }
	CWaterSkill1Script();
	virtual ~CWaterSkill1Script();

	virtual void Update();


	CLONE(CWaterSkill1Script)


};

