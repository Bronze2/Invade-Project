#include "pch.h"
#include "TrailRenderer.h"
#include "ResMgr.h"
#include "Transform.h"
#include "ArrowScript.h"

void CTrailRenderer::Update()
{
	// 화살에 달린 트레일일 때 
	if (m_bEmit && nullptr != m_pObj->GetScript<CArrowScript>()) {
		m_fCurTime += DT;
		if (m_fCurTime >= m_fEmitTime) {
			EmitPoint(m_pObj->Transform()->GetWorldPos(), m_pObj->GetScript<CArrowScript>()->GetAngle());
			m_fCurTime = 0.f;
		}
	}
}

void CTrailRenderer::FinalUpdate()
{
	Transform()->UpdateData();

	m_pMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fWidth);
	m_pMtrl->SetData(SHADER_PARAM::FLOAT_1, &m_fHeight);
	m_pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vColor);
	m_pMtrl->UpdateData();

}

void CTrailRenderer::Render()
{

	m_pMesh->Render();
}

void CTrailRenderer::EmitPoint(Vec3 _vPos, float _fAngle)
{

}

void CTrailRenderer::Init(Ptr<CTexture> _pTex)
{
	m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh");
	m_pMtrl = new CMaterial;
	m_pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TrailShader"));
	Ptr<CTexture> pTrailTex = _pTex;
	m_pMtrl->SetData(SHADER_PARAM::TEX_0, pTrailTex.GetPointer());

	m_pObj = GetObj()->GetParent();
}

void CTrailRenderer::SaveToScene(FILE* _pFile)
{
}

void CTrailRenderer::LoadFromScene(FILE* _pFile)
{
}

CTrailRenderer::CTrailRenderer() : CComponent(COMPONENT_TYPE::TRAILRENDERER), m_fEmitTime(1.f), m_bEmit(false)
{

}

CTrailRenderer::~CTrailRenderer()
{
	delete m_pMtrl.GetPointer();
}