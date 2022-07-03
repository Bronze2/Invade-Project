#pragma once
#include "Script.h"

class CButton
	: public CScript
{
private:
	bool m_bIsPressed;
	POINT m_ptCursorPos;
	Vec3 m_vPos;
	Vec3 m_vScale;
	RECT m_rcImg;

public:
	virtual void Awake();
	virtual void Update();

	void ConvertPos();
	void StatusCheck();


public:
	CButton();
	virtual ~CButton();

	CLONE(CButton);
};

