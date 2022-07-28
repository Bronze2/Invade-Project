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
	unordered_map<wstring, tTextInfo> m_mapText;

public:
	void Clear();

	void AddText(wstring key, wstring text, Vec2 vPos, Vec2 vScale, Vec2 vPadding, Vec4 vColor);
	void RenderText();
	void DeleteText(wstring key);

	void SetFont(Ptr<CFont> _pFont) { m_pFont = _pFont; }
	Ptr<CFont> GetFont() { return m_pFont; }
};

