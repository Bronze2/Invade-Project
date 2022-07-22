#pragma once
#include "Font.h"
#include "Material.h"

struct tTextInfo
{
	Ptr<CFont> pFont;
	wstring text;
	Vec2 vPos;
	Vec2 vScale;
	Vec2 vPadding;
	Vec4 vColor;
};

class CFontMgr
{
	SINGLE(CFontMgr);
private:
	Ptr<CFont> m_pFont;
	vector<tTextInfo> m_vecText;

public:
	void Init();

	void AddText(wstring text, Vec2 vPos, Vec2 vScale, Vec2 vPadding, Vec4 vColor);
	void RenderText();
	void SetFont(Ptr<CFont> _pFont) { m_pFont = _pFont; }
	Ptr<CFont> GetFont() { return m_pFont; }
};

