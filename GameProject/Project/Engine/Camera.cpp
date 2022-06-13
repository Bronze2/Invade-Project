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
#include "Collider3D.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_frustum(this)
	, m_fFar(1000.f)
	, m_fNear(1.f)
	, m_fFOV(XM_PI / 4.f)
	, m_fScale(1.f)
	, m_eProjType(PROJ_TYPE::PERSPECTIVE)
	, m_iLayerCheck(0)
	, m_bModule(false)
{
}

CCamera::~CCamera()
{
	if (nullptr != m_pRay)
		delete m_pRay;
}

void CCamera::InterSectsObject(CCollider3D* _pCollider)
{
	if (!m_pPlayer)return;
	Vec3 vWorldPos = Vec3(m_matCamera._41, m_matCamera._42, m_matCamera._43);

	Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 target = m_pPlayer->Transform()->GetWorldPos();
	target.y += m_pPlayer->Collider3D()->GetOffsetPos().z;
	m_pRay->position = vWorldPos;
	m_pRay->direction = vDir;
	float min = INFINITE;


	if (m_pRay->Intersects(_pCollider->GetBox(), min)){
		Vec3 target = m_pPlayer->Transform()->GetWorldPos();

		target.y += m_pPlayer->Collider3D()->GetOffsetPos().z;
		float distance= Vec3::Distance(target, vWorldPos);

		float distance2 = Vec3::Distance(_pCollider->GetObj()->Transform()->GetWorldPos(), vWorldPos);
		if (distance2 > distance)
			return;
		if (_pCollider->GetObj()->GetName() == L"Cover") {
			
			if (min > 0 && min < distance) {
				min *= 1.5f;
	
				if (min >= distance - 100.f) {
					min = distance - 100.f;
				}
				m_vFront = (m_pRay->direction * min);
			
			}
		}
		else {
			m_arrInterSectObject.push_back(_pCollider->GetObj());
		}
	
	}
	
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

	m_vLook.x = vRight.z; m_vLook.y = vUp.z; m_vLook.z = vFront.z;
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
		m_matProj = XMMatrixOrthographicLH(m_fWidth * m_fScale,m_fHeight * m_fScale, m_fNear, m_fFar);
		//m_matProj = XMMatrixOrthographicOffCenterLH(0.f, res.fWidth, res.fHeight, 0.f, m_fNear, m_fFar);		
		//�׸���
	}

	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);

	m_frustum.FinalUpdate();
	if(!m_bModule)
		CRenderMgr::GetInst()->RegisterCamera(this);
}

void CCamera::SortGameObject()
{
	for (auto& pair : m_mapInstGroup_F)
		pair.second.clear();
	for (auto& pair : m_mapInstGroup_D)
		pair.second.clear();

	m_vecParticle.clear();
	

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_iLayerCheck & (1 << i))
		{
			const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();

			for (UINT j = 0; j < vecObj.size(); ++j)
			{
				if (!vecObj[j]->GetFrustumCheck()
					|| m_frustum.CheckFrustumSphere(vecObj[j]->Transform()->GetWorldPos(), vecObj[j]->Transform()->GetMaxScale()))
				{
					if (!vecObj[j]->IsActive())continue;
						
					if (vecObj[j]->MeshRender() && vecObj[j]->MeshRender()->GetMesh() != nullptr)
					{
						UINT iMtrlCount = vecObj[j]->MeshRender()->GetMtrlCount();

						for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
						{
							bool bIntersect = false;
							for (int k = 0; k < m_arrInterSectObject.size(); ++k) {
								if (m_arrInterSectObject[k] == vecObj[j]) {
									
										bIntersect = true;
										break;

									
									
								}
							}
						
							if (bIntersect)
								continue;
							


							Ptr<CMaterial> pMtrl = vecObj[j]->MeshRender()->GetSharedMaterial(iMtrl);

							// Material �� �����ϰ� ���� �ʰų�, Material �� ���� Shader �� �������� ���� ���¶�� Continue
							if (nullptr == pMtrl || pMtrl->GetShader() == nullptr)
								continue;

							// Shader �� Deferred ���� Forward ������ ����
							// �ν��Ͻ� �׷��� �з��Ѵ�.
							map<ULONG64, vector<tInstObj>>* pMap = NULL;
							if (pMtrl->GetShader()->GetShaderPov() == SHADER_POV::DEFERRED) {
								
									pMap = &m_mapInstGroup_D;
							}
							else if (pMtrl->GetShader()->GetShaderPov() == SHADER_POV::FORWARD) {
								pMap = &m_mapInstGroup_F;
							}
							else
								continue;

							uInstID uID = {};
							uID.IIID = vecObj[j]->MeshRender()->GetInstID(iMtrl);
							map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.IIID);
							if (iter == pMap->end())
							{
								pMap->insert(make_pair(uID.IIID, vector<tInstObj>{tInstObj{ vecObj[j], iMtrl }}));
							}
							else
							{
								iter->second.push_back(tInstObj{ vecObj[j], iMtrl });
							}
						}
					}
					else {
						

							if (vecObj[j]->ParticleSystem())
							{
								m_vecParticle.push_back(vecObj[j]);
							}								
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

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_D)
	{
		// �׷� ������Ʈ�� ���ų�, ���̴��� ���� ���
		if (pair.second.empty())
			continue;
		else if (pair.second.size() < INSTANCING_COUNT // instancing ���� ����
			|| false == pair.second[0].pObj->MeshRender()->GetSharedMaterial(pair.second[0].iMtrlIdx)->GetShader()->IsPossibleInstancing())
		{
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->MeshRender()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->MeshRender()->GetSharedMaterial(pair.second[0].iMtrlIdx);

		if (nullptr == pMtrl->GetShader())
			continue;

		CInstancingBuffer* pInstBuffer = CInstancingMgr::GetInst()->GetInstancingBuffer((long long)pObj);
		if (pInstBuffer == nullptr)
		{
			// �ν��Ͻ� �����͸� ���� ���� �Ҵ�
			pInstBuffer = CInstancingMgr::GetInst()->AllocBuffer((long long)pObj);
		}

		if (false == pInstBuffer->BeUpdated())
		{
			int iRowIdx = 0;
			for (UINT i = 0; i < pair.second.size(); ++i)
			{

				// �����͸� ��Ƽ� �ν��Ͻ� ���ۿ� ����
				tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
				tInstData.matWV = tInstData.matWorld * m_matView;
				tInstData.matWVP = tInstData.matWV * m_matProj;

				if (pair.second[i].pObj->Animator3D())
				{
					pInstBuffer->Resize_BoneBuffer((UINT)pair.second.size(), sizeof(Matrix) * pMesh->GetBoneCount());
					pair.second[i].pObj->Animator3D()->UpdateData_Inst(pInstBuffer->GetBoneBuffer(), iRowIdx);
					tInstData.iRowIdx = iRowIdx++;
					CInstancingMgr::GetInst()->AddInstancingData(tInstData, true);
				}
				else
				{
					tInstData.iRowIdx = -1;
					CInstancingMgr::GetInst()->AddInstancingData(tInstData, false);
				}
			}

			// �ν��Ͻ̿� �ʿ��� �����͸� ����(SysMem -> GPU Mem)
			if (0 != pInstBuffer->GetInstanceCount())
				CInstancingMgr::GetInst()->SetData();
		}

		if (0 != pInstBuffer->GetInstanceCount())
		{
			if (pInstBuffer->GetAnimInstancingCount() > 0)
			{
				int iAnim = 1;
				int iBoneCount = pair.second[0].pObj->Animator3D()->GetBoneCount();
				pMtrl->SetData(SHADER_PARAM::INT_0, &iAnim);
				pMtrl->SetData(SHADER_PARAM::INT_1, &iBoneCount);
				pInstBuffer->GetBoneBuffer()->UpdateData(TEXTURE_REGISTER::t7);
			}

			pMtrl->UpdateData(1);
			pMesh->Render_Instancing(pair.second[0].iMtrlIdx, pInstBuffer);

			// Animatino ��� �� ����
			int a = 0;
			pMtrl->SetData(SHADER_PARAM::INT_0, &a);
			pMtrl->SetData(SHADER_PARAM::INT_1, &a);
		}
	}

	// ���� ������
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		for (auto& tInstObj : pair.second)
		{
			tInstObj.pObj->MeshRender()->Render(tInstObj.iMtrlIdx);
		}
	}
}

void CCamera::Render_Forward()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProjInv = m_matProjInv;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_F)
	{
		// �׷� ������Ʈ�� ���ų�, ���̴��� ���� ���
		if (pair.second.empty())
			continue;
		else if (pair.second.size() < INSTANCING_COUNT // instancing ���� ����
			|| false == pair.second[0].pObj->MeshRender()->GetSharedMaterial(pair.second[0].iMtrlIdx)->GetShader()->IsPossibleInstancing())
		{
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->MeshRender()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->MeshRender()->GetSharedMaterial(pair.second[0].iMtrlIdx);

		if (nullptr == pMtrl->GetShader())
			continue;

		CInstancingBuffer* pInstBuffer = CInstancingMgr::GetInst()->GetInstancingBuffer(pMesh->GetID());
		if (pInstBuffer == nullptr)
		{
			// �ν��Ͻ� �����͸� ���� ���� �Ҵ�
			pInstBuffer = CInstancingMgr::GetInst()->AllocBuffer(pMesh->GetID());
		}

		if (false == pInstBuffer->BeUpdated())
		{
			int iRowIdx = 0;
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				if (pair.second[i].pObj->Animator2D())// || pair.second[i].pObj->Animator3D())
				{
					map<INT_PTR, vector<tInstObj>>::iterator iter
						= m_mapSingleObj.find((INT_PTR)pair.second.at(0).pObj);

					if (iter != m_mapSingleObj.end())
						iter->second.push_back(pair.second[i]);
					else
					{
						m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[0].pObj, vector<tInstObj>{pair.second[i]}));
					}
					continue;
				}

				// �����͸� ��Ƽ� �ν��Ͻ� ���ۿ� ����
				tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
				tInstData.matWV = tInstData.matWorld * m_matView;
				tInstData.matWVP = tInstData.matWV * m_matProj;

				if (pair.second[i].pObj->Animator3D())
				{
					pInstBuffer->Resize_BoneBuffer((UINT)pair.second.size(), sizeof(Matrix) * pMesh->GetBoneCount());
					pair.second[i].pObj->Animator3D()->UpdateData_Inst(pInstBuffer->GetBoneBuffer(), tInstData.iRowIdx);
					tInstData.iRowIdx = iRowIdx++;
					//CInstancingMgr::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
					CInstancingMgr::GetInst()->AddInstancingData(tInstData, true);
				}
				else
				{
					tInstData.iRowIdx = -1;
					CInstancingMgr::GetInst()->AddInstancingData(tInstData, false);
				}
			}

			// �ν��Ͻ̿� �ʿ��� �����͸� ����(SysMem -> GPU Mem)
			if (0 != pInstBuffer->GetInstanceCount())
				CInstancingMgr::GetInst()->SetData();
		}

		if (0 != pInstBuffer->GetInstanceCount())
		{
			if (pInstBuffer->GetAnimInstancingCount() > 0)
			{
				int iAnim = 1;
				int iBoneCount = pair.second[0].pObj->Animator3D()->GetBoneCount();
				pMtrl->SetData(SHADER_PARAM::INT_0, &iAnim);
				pMtrl->SetData(SHADER_PARAM::INT_1, &iBoneCount);
				pInstBuffer->GetBoneBuffer()->UpdateData(TEXTURE_REGISTER::t7);
			}

			pMtrl->UpdateData(1);
			pMesh->Render_Instancing(pair.second[0].iMtrlIdx, pInstBuffer);

			// Animatino ��� �� ����
			int a = 0;
			pMtrl->SetData(SHADER_PARAM::INT_0, &a);
			pMtrl->SetData(SHADER_PARAM::INT_1, &a);
		}
	}

	// ���� ������
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		for (auto& tInstObj : pair.second)
		{
			tInstObj.pObj->MeshRender()->Render(tInstObj.iMtrlIdx);

			// �浹ü ���� ��, �浹ü�� �׷��ش�.
			if (tInstObj.pObj->Collider3D())
				tInstObj.pObj->Collider3D()->Render();
		}
	}

	// Particle Rendering
	for (size_t i = 0; i < m_vecParticle.size(); ++i)
	{
		if(nullptr!=m_vecParticle[i]->ParticleSystem())
			m_vecParticle[i]->ParticleSystem()->Render();
	}

	// Deferred Collider rendering
	for (auto& pair : m_mapInstGroup_D)
	{
		for (size_t i = 0; i < pair.second.size(); ++i)
		{
			if (pair.second[i].pObj->Collider3D())
			{
				pair.second[i].pObj->Collider3D()->Render();
			}
		}
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
					if (vecObj[i]->Collider3D())
					{
						vecObj[i]->Collider3D()->Render();
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


void CCamera::SetRay(Vec3 _vPos, Vec3 _vDir)
{
	m_pRay->position = _vPos;
	m_pRay->direction = _vDir;
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
