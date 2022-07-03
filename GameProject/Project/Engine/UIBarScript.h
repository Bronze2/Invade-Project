#pragma once
#include "Script.h"

class CUIBarScript
	: public CScript
{
private:
	tResolution m_tRes;
	CGameObject* m_pMainCam;

public:
	virtual void Awake();
	virtual void Update();

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	void ConvertPos(Vec3 _vPos);
	void SetMainCam(CGameObject* _pCam) { m_pMainCam = _pCam; }

public:
	CUIBarScript();
	virtual ~CUIBarScript();

	CLONE(CUIBarScript);
};

