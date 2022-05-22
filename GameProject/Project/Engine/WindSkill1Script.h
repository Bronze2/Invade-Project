#pragma once
#include "Script.h"
class CWindSkill1Script :
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
	CWindSkill1Script();
	virtual ~CWindSkill1Script();

	virtual void Update();


	CLONE(CWindSkill1Script)


};

