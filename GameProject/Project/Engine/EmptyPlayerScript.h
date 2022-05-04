#pragma once
#include "Script.h"

class CEmptyPlayerScript :
	public CScript
{
private:
	bool m_bCheckStartMousePoint;
	float m_fDegree;

public:
	virtual void Update();


	CEmptyPlayerScript();
	virtual ~CEmptyPlayerScript();

	CLONE(CEmptyPlayerScript);
};
