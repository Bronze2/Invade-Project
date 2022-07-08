#include "pch.h"
#include "Trail.h"
#include "StructuredBuffer.h"
#include "ResMgr.h"
#include "Transform.h"

void CTrail::FinalUpdate()
{

}

void CTrail::Render()
{
	Transform()->UpdateData();

	m_pTrailBuffer->UpdateData(TEXTURE_REGISTER::t13);
	m_pMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fStartScale);
	m_pMtrl->SetData(SHADER_PARAM::FLOAT_1, &m_fEndScale);
	m_pMtrl->UpdateData();
}

void CTrail::Init(Ptr<CTexture> _pTexture)
{
	m_pTrailBuffer = new CStructuredBuffer;
	m_pTrailBuffer->Create(sizeof(tTrail), 1, nullptr);

	m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh");
	m_pMtrl = new CMaterial;
	m_pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TrailShader"));
	Ptr<CTexture> pParticle = _pTexture;
	m_pMtrl->SetData(SHADER_PARAM::TEX_0, pParticle.GetPointer());
}

CTrail::CTrail() : CScript(0)
	, m_pTrailBuffer(nullptr)
	, m_fFrequency(0.f)
	, m_fStartScale(1.f)
	, m_fEndScale(2.f)
{

}

CTrail::~CTrail()
{
	SAFE_DELETE(m_pTrailBuffer);
}