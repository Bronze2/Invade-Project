#pragma once
#include "Component.h"

class CCamera;

class CStaticUI
	: public CComponent
{
private:
	CCamera* m_pCamera;

public:
	CCamera* GetCamera() { return m_pCamera; }
	void SetCamera(CCamera* _cam) { m_pCamera = _cam; }

	void Init();
	virtual void Update();

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CStaticUI();
	virtual ~CStaticUI();

	CLONE(CStaticUI)
};

