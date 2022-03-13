#include "pch.h"
#include "ParticleSystem.h"
#include "StructuredBuffer.h"
#include "ResMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Transform.h"

void CParticleSystem::FinalUpdate()
{
	m_fAccTime += DT;
	int iAdd = 0;
	if (m_fFrequency < m_fAccTime) {
		m_fAccTime = m_fAccTime - m_fFrequency;
		iAdd = 10;

	}
	m_pParticleBuffer->UpdateRWData(UAV_REGISTER::u0);
	m_pSharedBuffer->UpdateRWData(UAV_REGISTER::u1);
	m_pUpdateMtrl->SetData(SHADER_PARAM::INT_0, &m_iMaxParticle);
	m_pUpdateMtrl->SetData(SHADER_PARAM::INT_1, &iAdd);
	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fMinLifeTime);
	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_1, &m_fMaxLifeTime);
	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_2, &m_fMinSpeed);
	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_3, &m_fMaxSpeed);
	m_pUpdateMtrl->Dispatch(1, 1, 1);

}

void CParticleSystem::Render()
{
	Transform()->UpdateData();

	m_pParticleBuffer->UpdateData(TEXTURE_REGISTER::t10);
	m_pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vStartColor);
	m_pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vEndColor);
	m_pMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fStartScale);
	m_pMtrl->SetData(SHADER_PARAM::FLOAT_1, &m_fEndScale);
	m_pMtrl->UpdateData();
	m_pMesh->Render_Particle(m_iMaxParticle);
}

void CParticleSystem::Init(Ptr<CTexture> _pTexture)
{

	m_pParticleBuffer = new CStructuredBuffer;
	m_pParticleBuffer->Create(sizeof(tParticle), m_iMaxParticle, nullptr);

	m_pSharedBuffer = new CStructuredBuffer;
	m_pSharedBuffer->Create(sizeof(tParticleShared), 1, nullptr);
	m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh");
	m_pMtrl = new CMaterial;
	m_pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"ParticleShader"));
	Ptr<CTexture> pParticle = _pTexture;
	m_pMtrl->SetData(SHADER_PARAM::TEX_0, pParticle.GetPointer());
	m_pUpdateMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleUpdateMtrl");
}

void CParticleSystem::SaveToScene(FILE* _pFile)
{
}

void CParticleSystem::LoadFromScene(FILE* _pFile)
{
}

CParticleSystem::CParticleSystem():CComponent(COMPONENT_TYPE::PARTICLESYSTEM),m_pParticleBuffer(nullptr),m_pSharedBuffer(nullptr),m_iMaxParticle(1000),m_fFrequency(0.1f),m_fAccTime(0.4f),m_fMinLifeTime(0.1f),
m_fMaxLifeTime(0.5f),m_fMinSpeed(10),m_fMaxSpeed(20.f),m_fStartScale(1.f),m_fEndScale(2.f),m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
{


}

CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_pParticleBuffer);
	SAFE_DELETE(m_pSharedBuffer);
}
