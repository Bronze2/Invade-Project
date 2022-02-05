#include "pch.h"
#include "Light3D.h"

#include "Transform.h"
#include "RenderMgr.h"
#include "ResMgr.h"
#include "Camera.h"

CLight3D::CLight3D() :CComponent(COMPONENT_TYPE::LIGHT3D),m_iArrIdx(-1),m_pCamObj(nullptr)
{
	m_pCamObj = new CGameObject;
	m_pCamObj->AddComponent(new CTransform);
	m_pCamObj->AddComponent(new CCamera);

	m_pCamObj->Camera()->SetModule(true);
	m_pCamObj->Camera()->SetLayerAllCheck();
}

CLight3D::CLight3D(const CLight3D& _Light):CComponent(_Light),m_tLightInfo(_Light.m_tLightInfo),m_pLightMtrl(_Light.m_pLightMtrl),m_pVolumeMesh(_Light.m_pVolumeMesh),m_iArrIdx(-1),m_pCamObj(nullptr)
{
	m_pCamObj = _Light.m_pCamObj->Clone();
}

CLight3D::~CLight3D()
{
	SAFE_DELETE(m_pCamObj);
}


void CLight3D::SetLightType(LIGHT_TYPE _eType)
{
	m_tLightInfo.iLightType = (int)_eType;

	if (LIGHT_TYPE::DIR == (LIGHT_TYPE)m_tLightInfo.iLightType)
	{
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl");
		m_pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHGRAPHIC);
		m_pCamObj->Camera()->SetScale(1.f);
		m_pCamObj->Camera()->SetFar(100000.f);
		m_pCamObj->Camera()->SetWidth(512.f);
		m_pCamObj->Camera()->SetHeight(512.f);
	}
	else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_tLightInfo.iLightType)
	{
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	}
	else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_tLightInfo.iLightType)
	{
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
	}
}

void CLight3D::SetLightPos(const Vec3& _vPos)
{
	m_tLightInfo.vLightPos = _vPos;
	Transform()->SetLocalPos(_vPos);
}

void CLight3D::SetLightDir(const Vec3& _vDir)
{
	m_tLightInfo.vLightDir = _vDir;
	m_tLightInfo.vLightDir.Normalize();
	Transform()->LookAt(Vec3(m_tLightInfo.vLightDir.x, m_tLightInfo.vLightDir.y, m_tLightInfo.vLightDir.z));
}

void CLight3D::FinalUpdate()
{
	m_tLightInfo.vLightPos = Transform()->GetWorldPos();
	Transform()->SetLocalScale(Vec3(m_tLightInfo.fRange, m_tLightInfo.fRange, m_tLightInfo.fRange));
	m_iArrIdx = CRenderMgr::GetInst()->RegisterLight3D(this);
	*m_pCamObj->Transform() = *Transform();
	m_pCamObj->FinalUpdate();
}

void CLight3D::Render()
{
	if (-1 == m_iArrIdx)
		return;
	if (m_tLightInfo.iLightType == (UINT)LIGHT_TYPE::DIR) {
		Ptr<CTexture> pShadowMapTex = CResMgr::GetInst()->FindRes<CTexture>(L"ShadowMapTargetTex");
		m_pLightMtrl->SetData(SHADER_PARAM::TEX_3, pShadowMapTex.GetPointer());
		Matrix matVP = m_pCamObj->Camera()->GetViewMat() * m_pCamObj->Camera()->GetProjMat();

		m_pLightMtrl->SetData(SHADER_PARAM::MATRIX_0, &matVP);
	}



	Transform()->UpdateData();
	m_pLightMtrl->SetData(SHADER_PARAM::INT_0, &m_iArrIdx); // ±¤¿ø ¹è¿­ ÀÎµ¦½ºÁ¤º¸ ¾÷µ¥ÀÌÆ®
	m_pLightMtrl->UpdateData(); // int_0, ±¤¿ø ÀÎµ¦½º , tex_0 : normaltarget, tex_1 : positiontarget 
	m_pVolumeMesh->Render();
}

void CLight3D::Render_ShadowMap()
{
	m_pCamObj->Camera()->SortShadowObject();
	m_pCamObj->Camera()->Render_ShadowMap();
}

void CLight3D::SaveToScene(FILE* _pFile)
{
}

void CLight3D::LoadFromScene(FILE* _pFile)
{
}
