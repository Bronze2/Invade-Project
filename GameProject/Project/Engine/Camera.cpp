#include "pch.h"
#include "Camera.h"

#include "Transform.h"
#include "Device.h"
#include "SceneMgr.h"
#include "RenderMgr.h"
#include "Scene.h"
#include "Layer.h"

#include "KeyMgr.h"
#include "TimeMgr.h"

#include "MeshRender.h"
#include "Collider2D.h"
#include "ParticleSystem.h"
#include "InstancingBuffer.h"
#include "InstancingMgr.h"
#include "Animator3D.h"
#include "Material.h"
#include "Mesh.h"
CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_frustum(this)
	, m_fFar(1000.f)
	, m_fNear(1.f)
	, m_fFOV(XM_PI / 4.f)
	, m_fScale(1.f)
	, m_eProjType(PROJ_TYPE::PERSPECTIVE)
	, m_iLayerCheck(0)
	,m_bModule(false)
{
}

CCamera::~CCamera()
{
}

void CCamera::FinalUpdate()
{
	// �����
	Vec3 vPos = Transform()->GetWorldPos();
	Matrix matViewTrans = XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);

	Matrix matViewRot = XMMatrixIdentity();
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vUp = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matViewRot._11 = vRight.x; matViewRot._12 = vUp.x; matViewRot._13 = vFront.x;
	matViewRot._21 = vRight.y; matViewRot._22 = vUp.y; matViewRot._23 = vFront.y;
	matViewRot._31 = vRight.z; matViewRot._32 = vUp.z; matViewRot._33 = vFront.z;

	m_matView = matViewTrans * matViewRot;

	// �������
	m_matProj = XMMatrixIdentity();

	tResolution res = CRenderMgr::GetInst()->GetResolution();

	if (m_eProjType == PROJ_TYPE::PERSPECTIVE)
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV, res.fWidth / res.fHeight, m_fNear, m_fFar);
	}
	else
	{
		m_matProj = XMMatrixOrthographicLH(res.fWidth * m_fScale, res.fHeight * m_fScale, m_fNear, m_fFar);
		//m_matProj = XMMatrixOrthographicOffCenterLH(0.f, res.fWidth, res.fHeight, 0.f, m_fNear, m_fFar);		
	}

	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);

	m_frustum.FinalUpdate();
	if(!m_bModule)
	CRenderMgr::GetInst()->RegisterCamera(this);
}

void CCamera::SortGameObject()
{
	m_vecDeferred.clear();
	m_vecForward.clear();
	m_vecParticle.clear();
	

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_iLayerCheck & (1 << i))
		{
			const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();

			for (UINT i = 0; i < vecObj.size(); ++i)
			{
				if (!vecObj[i]->GetFrustumCheck()
					|| m_frustum.CheckFrustumSphere(vecObj[i]->Transform()->GetWorldPos(), vecObj[i]->Transform()->GetMaxScale()))
				{
					if (vecObj[i]->MeshRender()
						&& vecObj[i]->MeshRender()->GetMesh() != nullptr
						&& vecObj[i]->MeshRender()->GetSharedMaterial() != nullptr
						&& vecObj[i]->MeshRender()->GetSharedMaterial()->GetShader() != nullptr)
					{
						if (SHADER_POV::DEFERRED == vecObj[i]->MeshRender()->GetSharedMaterial()->GetShader()->GetShaderPov())
							m_vecDeferred.push_back(vecObj[i]);
						else if (SHADER_POV::FORWARD == vecObj[i]->MeshRender()->GetSharedMaterial()->GetShader()->GetShaderPov())
							m_vecForward.push_back(vecObj[i]);
					
					}
					else if (vecObj[i]->ParticleSystem())
					{
						m_vecParticle.push_back(vecObj[i]);
					}
				}
			}
		}
	}
}

void CCamera::SortShadowObject()
{
	m_vecShadowObj.clear();
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = nullptr;
	for (UINT i = 0; i < MAX_LAYER; ++i) {
		pLayer = pCurScene->GetLayer(i);
		if (nullptr == pLayer || !(m_iLayerCheck & (1 << i)))continue;
		const vector<CGameObject*>& vecObj = pLayer->GetObjects();
		for (size_t j = 0; j < vecObj.size(); ++j) {
			if (!vecObj[j]->GetFrustumCheck() || m_frustum.CheckFrustumSphere(vecObj[j]->Transform()->GetWorldPos(), vecObj[j]->Transform()->GetMaxScale())) {
				if (vecObj[j]->MeshRender() && vecObj[j]->MeshRender()->GetMesh() != nullptr && vecObj[j]->MeshRender()->GetSharedMaterial() != nullptr
					&& vecObj[j]->MeshRender()->GetSharedMaterial()->GetShader() != nullptr && vecObj[j]->MeshRender()->IsDynamicShadow()) {
					m_vecShadowObj.push_back(vecObj[j]);
				}
			}
		}
	}
}

void CCamera::Render_Deferred()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProjInv = m_matProjInv;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (size_t i = 0; i < m_vecDeferred.size(); ++i)
	{
		m_vecDeferred[i]->MeshRender()->Render();
	}
}

void CCamera::Render_Forward()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProjInv = m_matProjInv;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (size_t i = 0; i < m_vecForward.size(); ++i)
	{
		m_vecForward[i]->MeshRender()->Render();

		if (m_vecForward[i]->Collider2D())
			m_vecForward[i]->Collider2D()->Render();
	}

	for (size_t i = 0; i < m_vecParticle.size(); ++i)
	{
		m_vecParticle[i]->ParticleSystem()->Render();
	}

	for (size_t i = 0; i < m_vecDeferred.size(); ++i)
	{
		if (m_vecDeferred[i]->Collider2D())
			m_vecDeferred[i]->Collider2D()->Render();
	}
}

void CCamera::Render()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_iLayerCheck & (1 << i))
		{
			const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();

			for (UINT i = 0; i < vecObj.size(); ++i)
			{
				if (!vecObj[i]->GetFrustumCheck()
					|| m_frustum.CheckFrustumSphere(vecObj[i]->Transform()->GetWorldPos(), vecObj[i]->Transform()->GetMaxScale()))
				{
					if (vecObj[i]->MeshRender())
					{
						vecObj[i]->MeshRender()->Render();
					}

					if (vecObj[i]->Collider2D())
					{
						vecObj[i]->Collider2D()->Render();
					}
				}
			}
		}
	}
}

void CCamera::Render_ShadowMap()
{
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;
	g_transform.matViewInv = m_matViewInv;

	for (UINT i = 0; i < m_vecShadowObj.size(); ++i) {
		m_vecShadowObj[i]->MeshRender()->Render_ShadowMap();
	}
}

void CCamera::SaveToScene(FILE* _pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	fwrite(&m_fNear, sizeof(float), 1, _pFile);
	fwrite(&m_fFar, sizeof(float), 1, _pFile);

	fwrite(&m_fFOV, sizeof(float), 1, _pFile);
	fwrite(&m_fScale, sizeof(float), 1, _pFile);

	fwrite(&m_eProjType, sizeof(UINT), 1, _pFile);
	fwrite(&m_iLayerCheck, 4, 1, _pFile);
}

void CCamera::LoadFromScene(FILE* _pFile)
{
	fread(&m_fNear, sizeof(float), 1, _pFile);
	fread(&m_fFar, sizeof(float), 1, _pFile);

	fread(&m_fFOV, sizeof(float), 1, _pFile);
	fread(&m_fScale, sizeof(float), 1, _pFile);

	fread(&m_eProjType, sizeof(UINT), 1, _pFile);
	fread(&m_iLayerCheck, 4, 1, _pFile);
}
