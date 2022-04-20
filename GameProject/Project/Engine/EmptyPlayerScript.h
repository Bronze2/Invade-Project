#pragma once
#include "Script.h"

class CEmptyPlayerScript :
	public CScript
{
public:
	virtual void Update();

public:
	CEmptyPlayerScript();
	virtual ~CEmptyPlayerScript();

	CLONE(CEmptyPlayerScript);
};

