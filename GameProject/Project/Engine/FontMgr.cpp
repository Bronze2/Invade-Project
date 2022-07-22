#include "pch.h"
#include "FontMgr.h"
#include "ResMgr.h"

CFontMgr::CFontMgr()
{

}

CFontMgr::~CFontMgr()
{

}

void CFontMgr::Init()
{

}

void CFontMgr::RenderText()
{
	// Mtrl이든 셰이더든 설정해줘야함
	for (auto& f : m_vecText)
	{
		wcout << f.text << endl;
		f.pFont->Render(f.text, f.vPos, f.vScale, f.vPadding, f.vColor);
	}

	//m_pFont->Render(text, vPos, vScale, vPadding, vColor);
}

void CFontMgr::AddText(wstring text, Vec2 vPos, Vec2 vScale, Vec2 vPadding, Vec4 vColor)
{
	tTextInfo Info{};
	Info.pFont = CResMgr::GetInst()->Load<CFont>(L"Font", L"Font03.fnt");
	Ptr<CTexture> pFontTex = CResMgr::GetInst()->Load<CTexture>(L"FontTex", L"Font03.png");
	Ptr<CMaterial> pFontMtrl = new CMaterial;
	pFontMtrl->DisableFileSave();
	pFontMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"FontShader"));
	pFontMtrl->SetData(SHADER_PARAM::TEX_0, pFontTex.GetPointer());
	Info.pFont->SetMaterial(pFontMtrl);
	//Info.pFont->SetTexture(pFontTex);
	//Info.pFont->GetMaterial()->SetData(SHADER_PARAM::TEX_0, pFontTex.GetPointer());
	Info.pFont->CreateVB();

	Info.text = text;
	Info.vPos = vPos;
	Info.vScale = vScale;
	Info.vPadding = vPadding;
	Info.vColor = vColor;

	m_vecText.emplace_back(Info);
}