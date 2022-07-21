#pragma once
#include "Font.h"
#include "Material.h"

class CFontMgr
{
	SINGLE(CFontMgr);
private:
	Ptr<CFont> m_pFont;

public:
	void RenderText(wstring text, Vec3 vPos, Vec3 vScale, Vec2 vPadding, Vec4 vColor);
	void SetFont(Ptr<CFont> _pFont) { m_pFont = _pFont; }
	Ptr<CFont> GetFont() { return m_pFont; }
};

