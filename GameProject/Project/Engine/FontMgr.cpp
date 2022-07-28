#include "pch.h"
#include "FontMgr.h"
#include "ResMgr.h"

CFontMgr::CFontMgr()
{

}

CFontMgr::~CFontMgr()
{

}

void CFontMgr::Clear()
{
	m_mapText.clear();
}

void CFontMgr::RenderText()
{
	// Mtrl이든 셰이더든 설정해줘야함
	for (auto& f : m_mapText)
	{
		f.second.pFont->Render(f.second.text, f.second.vPos, f.second.vScale, f.second.vPadding, f.second.vColor);
	}
}

void CFontMgr::AddText(wstring key, wstring text, Vec2 vPos, Vec2 vScale, Vec2 vPadding, Vec4 vColor)
{
	tTextInfo Info{};
	Info.pFont = CResMgr::GetInst()->Load<CFont>(L"Font", L"font04.fnt");
	Ptr<CTexture> pFontTex = CResMgr::GetInst()->Load<CTexture>(L"FontTex", L"font04.png");
	Ptr<CMaterial> pFontMtrl = new CMaterial;
	pFontMtrl->DisableFileSave();
	pFontMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"FontShader"));
	pFontMtrl->SetData(SHADER_PARAM::TEX_0, pFontTex.GetPointer());
	Info.pFont->SetMaterial(pFontMtrl);
	Info.pFont->CreateVB();

	Info.text = text;
	Info.vPos = vPos;
	Info.vScale = vScale;
	Info.vPadding = vPadding;
	Info.vColor = vColor;

	m_mapText[key] = Info;
}

void CFontMgr::DeleteText(wstring key)
{
	if (m_mapText.find(key) != m_mapText.end()) {
		m_mapText.erase(key);
	}
}

//----------------------------------------------------------