#pragma once
#include "Script.h"

//·Îºñ¾À
class CButtonScript
	: public CScript
{
private:
	bool m_bIsPressed;
	POINT m_ptCursorPos;
	Vec3 m_vPos;
	Vec3 m_vScale;
	RECT m_rcImg;
	BUTTON_TYPE m_eType;

	CGameObject* m_pPlayerObj;
	CGameObject* m_pPlayerHelmet;
	CGameObject* m_pReadyBar;

	Ptr<CMaterial> m_pReadyNonePressedMtrl;
	Ptr<CTexture> m_pReadyNonePressedTex;
	Ptr<CMaterial> m_pReadyPressedMtrl;
	Ptr<CTexture> m_pReadyPressedTex;

public:
	virtual void Awake();
	virtual void Update();

	void ConvertPos();
	void StatusCheck();
	void Execute();
	void SetButtonType(BUTTON_TYPE _eType) { m_eType = _eType; }
	void SetMainPlayer(CGameObject* _pObj) { m_pPlayerObj = _pObj; }

public:
	CButtonScript();
	virtual ~CButtonScript();

	CLONE(CButtonScript);
};

