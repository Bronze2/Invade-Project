#include "pch.h"
#include "GridScript.h"

#include "MeshRender.h"
CGridScript::CGridScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
	, m_vGridColor(Vec4(0.6f, 0.6f, 0.6f, 1.f))
	, m_vToolCamPos()
	, m_pToolCam(nullptr)
	, m_fThickness(0.004f)
	, m_fDistance(0.f)
	, m_iMaxStep(5)
{
}

CGridScript::~CGridScript()
{
}

void CGridScript::Update()
{
	m_vToolCamPos = m_pToolCam->Transform()->GetWorldPos();
	Transform()->SetLocalPos(Vec3(m_vToolCamPos.x, 0.f, m_vToolCamPos.z));

	CalculateGrid();
}

void CGridScript::CalculateGrid()
{
	// ī�޶� ���̿� ���� Grid ������ ���Ѵ�.
	// 1 �ܰ� : ���� 10, ī�޶� ���� 0 ~ 100
	// 2 �ܰ� : ���� 100, ī�޶� ���� 100 ~ 1000
	// 3 �ܰ� : ���� 1000, ī�޶� ���� 1000 ~ 10000
	// 4 �ܰ� : ���� 10000, ī�޶� ���� 10000 ~ 
	Ptr<CMaterial> pMtrl = MeshRender()->GetSharedMaterial();

	UINT iHeight = (UINT)abs(m_vToolCamPos.y);
	UINT iStep = 1;
	UINT i = 100;
	for (; iStep <= 4; i *= 10, ++iStep)
	{
		if (iHeight <= i)
			break;
	}

	float fThickness = ((float)iHeight * m_fThickness);
	m_fAlpha = abs(1.f - (float)((double)iHeight / pow(10., (double)iStep + 1.)));

	// Material �� ���� ����	
	pMtrl->SetData(SHADER_PARAM::INT_0, &iStep);
	pMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fAlpha);
	pMtrl->SetData(SHADER_PARAM::FLOAT_1, &fThickness);
	pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vToolCamPos);
	pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vGridColor);
}
