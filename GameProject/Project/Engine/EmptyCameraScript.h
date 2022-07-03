#pragma once
#include "Script.h"

class CEmptyCameraScript
	:    public CScript
{
private:

	bool m_bCheckStartMousePoint;
	float m_fDegree;

public:
	CEmptyCameraScript();
	virtual ~CEmptyCameraScript();

	void Init();
	virtual void Update();

	CLONE(CEmptyCameraScript);
};

