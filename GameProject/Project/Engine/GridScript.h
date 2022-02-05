#pragma once
#include "Script.h"
class CGridScript :
    public CScript
{
private:
	CGameObject* m_pToolCam;
	Vec4			m_vGridColor;
	Vec4			m_vToolCamPos;
	float			m_fThickness;
	float			m_fDistance;
	UINT			m_iMaxStep;		// �׸��� ���� �ִ� �ܰ�
	float			m_fAlpha;		// �׸��� ����

public:
	virtual void Update();

public:
	void SetToolCamera(CGameObject* _pToolCam) { m_pToolCam = _pToolCam; }
	void SetGridColor(const Vec3& _vColor) { m_vGridColor = _vColor; }
private:
	void CalculateGrid();

public:
	CLONE(CGridScript);

public:
	CGridScript();
	virtual ~CGridScript();
};

