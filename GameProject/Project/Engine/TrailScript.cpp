#include "pch.h"
#include "TrailScript.h"
#include "StructuredBuffer.h"
#include "ResMgr.h"
#include "Transform.h"

void CTrailScript::Update()
{

}

void CTrailScript::FinalUpdate()
{

}

void CTrailScript::Render()
{
	//Transform()->UpdateData();

	//m_pMtrl->UpdateData();
}

void CTrailScript::Init(Ptr<CTexture> _pTexture)
{
	//m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	//m_pMtrl = new CMaterial;
	//m_pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TrailShader"));
	//Ptr<CTexture> pTrailTex = _pTexture;
	//m_pMtrl->SetData(SHADER_PARAM::TEX_0, pTrailTex.GetPointer());
}

CTrailScript::CTrailScript() : CScript(0)
{

}

CTrailScript::~CTrailScript()
{
}