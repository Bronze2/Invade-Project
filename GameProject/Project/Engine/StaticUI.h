#pragma once
#include "Component.h"

class CCamera;

class CStaticUI
	: public CComponent
{
private:
public:
	virtual void Update();

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);


public:
	CStaticUI();
	virtual ~CStaticUI();

	CLONE(CStaticUI)
};

