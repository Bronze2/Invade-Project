#include "pch.h"
#include "FontMgr.h"

CFontMgr::CFontMgr()
{

}

CFontMgr::~CFontMgr()
{

}

void CFontMgr::RenderText(wstring text, Vec3 vPos, Vec3 vScale, Vec2 vPadding, Vec4 vColor)
{
	// Mtrl�̵� ���̴��� �����������
	m_pFont->Render(text, vPos, vScale, vPadding, vColor);
}