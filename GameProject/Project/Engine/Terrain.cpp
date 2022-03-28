#include "pch.h"
#include "Terrain.h"
#include "ResMgr.h"
#include "MeshRender.h"
#include"RenderMgr.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"



CTerrain::CTerrain():CComponent(COMPONENT_TYPE::TERRAIN)
,m_iFaceX(64),m_iFaceZ(64),m_fMaxTess(4),m_vBrushScale(Vec2(0.1f,0.1f))
,m_iBrushIdx(0),m_iTileIdx(0)
{
}

CTerrain::~CTerrain()
{
}



void CTerrain::Init()
{
	ChangeFaceCount(m_iFaceX, m_iFaceZ);
	Ptr<CMaterial>pMtrl = MeshRender()->GetSharedMaterial();
	pMtrl->SetData(SHADER_PARAM::INT_1, &m_iFaceX);
	pMtrl->SetData(SHADER_PARAM::INT_2, &m_iFaceZ);

	m_pHeightMap = CResMgr::GetInst()->Load<CTexture>(L"HeightMap", L"Texture\\Terrain\\HeightMap.png");
	Vec2 vHeightMapRes = Vec2(m_pHeightMap->Width(), m_pHeightMap->Height());

	pMtrl->SetData(SHADER_PARAM::TEX_2, m_pHeightMap.GetPointer());
	pMtrl->SetData(SHADER_PARAM::VEC2_0, &vHeightMapRes);
	pMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fMaxTess);
}

void CTerrain::Update()
{
}

void CTerrain::FinalUpdate()
{
	CCamera* pMainCamera = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr != pMainCamera) {
		Vec4 vPos = Vec4(pMainCamera->Transform()->GetLocalPos(), 0.f);
		MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC4_0, &vPos);
	}
}

void CTerrain::ChangeFaceCount(UINT _iFaceX, UINT _iFaceY)
{
	m_iFaceX = _iFaceX;
	m_iFaceZ = _iFaceY;
	CreateTerrainMesh();
}

void CTerrain::CreateTerrainMesh()
{
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;
	Ptr<CMesh> pMesh = nullptr;
	pMesh = new CMesh;
	VTX v;

	for (UINT i = 0; i < m_iFaceZ+1; ++i) {
		for (UINT j = 0; j < m_iFaceX+1; ++j) {
			v.vPos = Vec3((float)j, 0.f,(float)i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vUV = Vec2((float)j, (float)(m_iFaceZ-i));
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, 1.f);
			vecVtx.push_back(v);
		}
	}
	for (UINT i = 0; i < m_iFaceZ; ++i) {
		for (UINT j = 0; j < m_iFaceX; ++j) {
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + j);
			vecIdx.push_back((m_iFaceX + 1) * i+ j+1);
			vecIdx.push_back((m_iFaceX + 1) * i + j);

			vecIdx.push_back((m_iFaceX + 1) * (i ) + (j+1));
			vecIdx.push_back((m_iFaceX + 1) * (i+1) + j);
			vecIdx.push_back((m_iFaceX + 1) * (i+1) + j+1);
		}
	}
	pMesh->Create(sizeof(VTX), (UINT)vecVtx.size(), (BYTE*)vecVtx.data(),
		DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());
	Ptr<CMesh> pOriginMesh = CResMgr::GetInst()->FindRes<CMesh>(L"TerrainMesh");
	if (nullptr != pOriginMesh) {
		CResMgr::GetInst()->DestroyResource<CMesh>(L"TerrainMesh");

	}
	CResMgr::GetInst()->AddRes<CMesh>(L"TerrainMesh", pMesh);
	MeshRender()->SetMesh(pMesh);
}

void CTerrain::SaveToScene(FILE* _pFile)
{
}

void CTerrain::LoadFromScene(FILE* _pFile)
{
}
