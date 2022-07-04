#include "pch.h"
#include "ResMgr.h"

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Sound.h"
#include "RenderMgr.h"

CResMgr::CResMgr(){}
CResMgr::~CResMgr() {

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i) {
		Safe_Delete_Vector(m_vecCloneRes[i]);
	}
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i) {
		Safe_Delete_Map(m_mapRes[i]);
	}

}

void CResMgr::CreateDefaultMesh()
{
	vector<VTX> vecVTX;
	vector<UINT> vecIdx;

	Ptr<CMesh> pMesh = nullptr;




	VTX v = {};
	// 1. �Է� ������ �ܰ迡 ������, ���� 3���� ������ �ﰢ�� 1��

	pMesh = new CMesh;

	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);

	UINT iIdx = 0;

	pMesh->Create(sizeof(VTX), 1, (BYTE*)&v
		, DXGI_FORMAT_R32_UINT, 1, (BYTE*)&iIdx);

	AddRes(L"PointMesh", pMesh);
	pMesh = new CMesh;

	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVTX.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2);
	vecIdx.push_back(0); vecIdx.push_back(2); vecIdx.push_back(3);

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	AddRes(L"RectMesh", pMesh);

	// =============		// 0 --- 1
	// Collider Rect Mesh	// |     |
	//						// 3 --- 2
	// =============	
	pMesh = new CMesh;

	vecIdx.clear();
	vecIdx.push_back(0); vecIdx.push_back(1);
	vecIdx.push_back(2); vecIdx.push_back(3); vecIdx.push_back(0);

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data()); //D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP

	AddRes(L"ColRectMesh", pMesh);
	vecVTX.clear();
	vecIdx.clear();
	pMesh = new CMesh;

	v.vPos = Vec3(-0.5f, 0.f, 0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.5f, 0.f, 0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.5f, 0.f, -0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(-0.5f, 0.f, -0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVTX.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2);
	vecIdx.push_back(0); vecIdx.push_back(2); vecIdx.push_back(3);

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	AddRes(L"Rect2Mesh", pMesh);


	//				       3
	// ===========	//    /
	// Dir Mesh		//0,2,4 �� �� 5
	//				//    \
	// ===========	//     1
	pMesh = new CMesh;

	vecVTX.clear();
	vecIdx.clear();

	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(1.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.f, 1.f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.f, 0.f, 1.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	vecVTX.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1);
	vecIdx.push_back(2); vecIdx.push_back(3);
	vecIdx.push_back(4); vecIdx.push_back(5);

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data()); // D3D11_PRIMITIVE_TOPOLOGY_LINELIST

	AddRes(L"DirMesh", pMesh);

	// ============
	// Circle Mesh
	// ============
	vecVTX.clear();
	vecIdx.clear();

	pMesh = new CMesh;

	// ���� �߽���
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);

	vecVTX.push_back(v);

	UINT iSliceCount = 32;
	float fRadius = 0.5f;
	float fSliceTheta = XM_2PI / iSliceCount;

	float fCurTheta = 0.f;
	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fCurTheta), fRadius * sinf(fCurTheta), 0.f);
		v.vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);
		v.vUV = Vec2(0.5f * cosf(fCurTheta), 0.5f * sinf(fCurTheta));

		v.vUV.x += 0.5f;
		v.vUV.y = (0.5f - v.vUV.y);

		fCurTheta += fSliceTheta;

		vecVTX.push_back(v);
	}

	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	AddRes(L"CircleMesh", pMesh);

	// ============
	// ColCircle Mesh
	// ============	
	vecIdx.clear();

	pMesh = new CMesh;

	for (UINT i = 1; i < iSliceCount + 2; ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data()); // D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP

	AddRes(L"ColCircleMesh", pMesh);

	vecVTX.clear();
	vecIdx.clear();

	pMesh = new CMesh;

	// ���� �߽���
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);

	vecVTX.push_back(v);

	UINT iSliceCount2 = 32;
	float fRadius2 = 400.f;
	float fSliceTheta2 = XM_2PI / iSliceCount;

	float fCurTheta2 = 0.f;
	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		v.vPos = Vec3(fRadius2 * cosf(fCurTheta2), 0.f, fRadius2 * sinf(fCurTheta2));
		v.vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);
		v.vUV = Vec2(0.5f * cosf(fCurTheta2), 0.5f * sinf(fCurTheta2));

		v.vUV.x += 0.5f;
		v.vUV.y = (0.5f - v.vUV.y);

		fCurTheta2 += fSliceTheta2;

		vecVTX.push_back(v);
	}

	for (UINT i = 0; i < iSliceCount2; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	AddRes(L"CircleMesh2", pMesh);

	// ============
	// ColCircle Mesh
	// ============	
	vecIdx.clear();

	pMesh = new CMesh;

	for (UINT i = 1; i < iSliceCount2 + 2; ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data()); // D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP

	AddRes(L"ColCircleMesh2", pMesh);

	vecVTX.clear();
	vecIdx.clear();

	// =========
	// Cube Mesh
	// =========
	VTX arrCube[24] = {};

	// ����
	arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);  //
	arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vUV = Vec2(0.f, 0.f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vUV = Vec2(0.f, 0.f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vUV = Vec2(0.f, 0.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vUV = Vec2(0.f, 0.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);


	// �Ʒ� ��	
	arrCube[4].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vUV = Vec2(0.f, 0.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[5].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vUV = Vec2(0.f, 0.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[6].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vUV = Vec2(0.f, 0.f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[7].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vUV = Vec2(0.f, 0.f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);

	// ���� ��
	arrCube[8].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vUV = Vec2(0.f, 0.f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[9].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vUV = Vec2(0.f, 0.f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[10].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vUV = Vec2(0.f, 0.f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[11].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vUV = Vec2(0.f, 0.f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);

	// ������ ��
	arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vUV = Vec2(0.f, 0.f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vUV = Vec2(0.f, 0.f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vUV = Vec2(0.f, 0.f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vUV = Vec2(0.f, 0.f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);

	// �� ��
	arrCube[16].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vUV = Vec2(0.f, 0.f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[17].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vUV = Vec2(0.f, 0.f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[18].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vUV = Vec2(0.f, 0.f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[19].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vUV = Vec2(0.f, 0.f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);

	// �� ��
	arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vUV = Vec2(0.f, 0.f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vUV = Vec2(0.f, 0.f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vUV = Vec2(0.f, 0.f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vUV = Vec2(0.f, 0.f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);

	// �ε���
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	pMesh = new CMesh;

	pMesh->Create(sizeof(VTX), 24, (BYTE*)arrCube
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	pMesh->SetName(L"CubeMesh");
	AddRes<CMesh>(pMesh->GetName(), pMesh);


	//ColCube

	vecIdx.clear();
	// �ε���
	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2); vecIdx.push_back(3); vecIdx.push_back(0);
	vecIdx.push_back(4); vecIdx.push_back(5); vecIdx.push_back(6); vecIdx.push_back(7); vecIdx.push_back(4);
	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(6); vecIdx.push_back(7); vecIdx.push_back(0);
	vecIdx.push_back(3); vecIdx.push_back(2); vecIdx.push_back(5); vecIdx.push_back(4); vecIdx.push_back(3);
	pMesh = new CMesh;

	pMesh->Create(sizeof(VTX), 8, (BYTE*)arrCube
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	pMesh->SetName(L"ColCubeMesh");
	AddRes<CMesh>(pMesh->GetName(), pMesh);

	vecVTX.clear();
	vecIdx.clear();

	// ============
	// Sphere Mesh
	// ============
	fRadius = 1.f;

	// Top
	v.vPos = Vec3(0.f, fRadius, 0.f);
	v.vUV = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, 1.f);
	vecVTX.push_back(v);

	// Body
	UINT iStackCount = 40; // ���� ���� ����
	iSliceCount = 40; // ���� ���� ����

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
			v.vUV = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vTangent.Cross(v.vNormal, v.vBinormal);
			v.vBinormal.Normalize();

			vecVTX.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vec3(0.f, -fRadius, 0.f);
	v.vUV = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVTX.push_back(v);

	// �ε���
	// �ϱ���
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// ����
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// ������
	UINT iBottomIdx = (UINT)vecVTX.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh;

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	pMesh->SetName(L"SphereMesh");
	AddRes<CMesh>(pMesh->GetName(), pMesh);

	vecVTX.clear();
	vecIdx.clear();


	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);

	vecVTX.push_back(v);

	iSliceCount = 32;
	fRadius = 0.5f;
	fSliceTheta = XM_2PI / iSliceCount;
	pMesh = new CMesh;
	fCurTheta = 0.f;
	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fCurTheta), fRadius * sinf(fCurTheta), 0.f);
		v.vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);
		v.vUV = Vec2(0.5f * cosf(fCurTheta), 0.5f * sinf(fCurTheta));

		v.vUV.x += 0.5f;
		v.vUV.y = (0.5f - v.vUV.y);

		fCurTheta += fSliceTheta;

		vecVTX.push_back(v);
	}
	fCurTheta = 0.f;
	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		v.vPos = Vec3(0.f, fRadius * sinf(fCurTheta), fRadius * cosf(fCurTheta));
		v.vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);
		v.vUV = Vec2(0.5f * cosf(fCurTheta), 0.5f * sinf(fCurTheta));

		v.vUV.x += 0.5f;
		v.vUV.y = (0.5f - v.vUV.y);

		fCurTheta += fSliceTheta;

		vecVTX.push_back(v);
	}
	fCurTheta = 0.f;
	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		v.vPos = Vec3(fRadius * sinf(fCurTheta),0.f, fRadius * cosf(fCurTheta));
		v.vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);
		v.vUV = Vec2(0.5f * cosf(fCurTheta), 0.5f * sinf(fCurTheta));

		v.vUV.x += 0.5f;
		v.vUV.y = (0.5f - v.vUV.y);

		fCurTheta += fSliceTheta;

		vecVTX.push_back(v);
	}

	for (UINT i = 1; i < (iSliceCount+2)*3; ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data()); // D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP

	AddRes(L"ColSphereMesh", pMesh);

}

void CResMgr::CreateDefaultShader()
{
	//=============
	// TestShader
	// int 1 �� �ʿ�
	//=============
	Ptr<CShader> pShader = nullptr;

	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Test", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Test", "ps_5_0");

	// BlendState ����
//	pShader->SetBlendState(BLEND_TYPE::ALPHABLEND);

	pShader->Create(SHADER_POV::FORWARD);

	pShader->AddShaderParam(tShaderParam{ L"Test Value", SHADER_PARAM::INT_0 });

	AddRes(L"TestShader", pShader);

	// ==============
	// Texture Shader
	// ==============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Tex", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Tex", "ps_5_0");

	// BlendState ����
	// pShader->SetBlendState(BLEND_TYPE::ALPHABLEND);

	// DSState
	//pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_DEPTHTEST_NO_WRITE);

	// Shader Parameter �˸�
	pShader->AddShaderParam(tShaderParam{ L"Output Texture", SHADER_PARAM::TEX_0 });

	pShader->Create(SHADER_POV::FORWARD);

	AddRes(L"TexShader", pShader);

	// =================
	// Collider2D Shader
	// =================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Collider2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Collider2D", "ps_5_0");

	// DepthStencilState ����
//	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::);

	pShader->Create(SHADER_POV::FORWARD, D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddRes(L"Collider2DShader", pShader);

	// =================
	// LINE 2D Shader
	// =================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_LINE2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_LINE2D", "ps_5_0");

	pShader->Create(SHADER_POV::FORWARD, D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddRes(L"Line2DShader", pShader);

	// =================
	// STD2D Shader
	// =================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Std2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Std2D", "ps_5_0");

	// BlendState ����
	pShader->SetBlendState(BLEND_TYPE::ALPHABLEND);

	// Parameter ����
	pShader->AddShaderParam(tShaderParam{ L"Output Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Anim Tex", SHADER_PARAM::TEX_3 });

	pShader->Create(SHADER_POV::FORWARD);
	AddRes(L"Std2DShader", pShader);

	// =================
	// 2DShadow Shader
	// =================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_2DShadow", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_2DShadow", "ps_5_0");

	// BlendState ����
	pShader->SetBlendState(BLEND_TYPE::ALPHABLEND);

	// Parameter ����
	pShader->AddShaderParam(tShaderParam{ L"Output Texture", SHADER_PARAM::TEX_0 });
	pShader->Create(SHADER_POV::FORWARD);
	AddRes(L"2DShadowShader", pShader);

	// ============
	// Std3D Shader
	// ============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std3d.fx", "VS_Std3D", "vs_5_0");
	pShader->CreateVertexInstShader(L"Shader\\std3d.fx", "VS_Std3D_Inst", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std3d.fx", "PS_Std3D", "ps_5_0");
	
	pShader->Create(SHADER_POV::DEFERRED);		//DEFERRED
	AddRes(L"Std3DShader", pShader);


	// =============
	// Skybox Shader
	// =============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std3d.fx", "VS_Skybox", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std3d.fx", "PS_Skybox", "ps_5_0");
	pShader->SetRasterizerType(RS_TYPE::CULL_FRONT);
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::LESS_EQUAL);
	pShader->Create(SHADER_POV::FORWARD);
	AddRes(L"SkyboxShader", pShader);



	// ============
	// GridShader
	// ============
	pShader = new CShader;

	pShader->CreateVertexShader(L"Shader\\tool.fx", "VS_Grid", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\tool.fx", "PS_Grid", "ps_5_0");
	pShader->SetBlendState(BLEND_TYPE::ALPHABLEND);
	pShader->SetRasterizerType(RS_TYPE::CULL_NONE);
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::LESS_NO_WRITE);
	pShader->Create(SHADER_POV::FORWARD);
	AddRes(L"GridShader", pShader);

	// ===============
	// DirLight Shader
	// ===============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_DirLight", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_DirLight", "ps_5_0");

	// One-One Blend
	pShader->SetBlendState(BLEND_TYPE::ONEBLEND);

	// No Depth Test, No Depth Write
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_DEPTHTEST_NO_WRITE);

	pShader->AddShaderParam(tShaderParam{ L"Light Index", SHADER_PARAM::INT_0 });
	pShader->AddShaderParam(tShaderParam{ L"Normal Target Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Position Target Texture", SHADER_PARAM::TEX_1 });
	pShader->AddShaderParam(tShaderParam{ L"Specular Target Texture", SHADER_PARAM::TEX_2 });
	pShader->Create(SHADER_POV::LIGHTING);

	AddRes(L"DirLightShader", pShader);

	// ==================
	// PointLight Shader
	// ==================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_PointLight", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_PointLight", "ps_5_0");

	// One-One Blend
	pShader->SetBlendState(BLEND_TYPE::ONEBLEND);

	// No Depth Test, No Depth Write
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_DEPTHTEST_NO_WRITE);

	// Cull_None
	pShader->SetRasterizerType(RS_TYPE::CULL_FRONT);

	pShader->AddShaderParam(tShaderParam{ L"Light Index", SHADER_PARAM::INT_0 });
	pShader->AddShaderParam(tShaderParam{ L"Normal Target Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Position Target Texture", SHADER_PARAM::TEX_1 });
	pShader->AddShaderParam(tShaderParam{ L"Render Target Resolution", SHADER_PARAM::VEC2_0 });

	pShader->Create(SHADER_POV::LIGHTING);
	AddRes(L"PointLightShader", pShader);

	// ==================
	// MergeLight Shader
	// ==================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_MergeLight", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_MergeLight", "ps_5_0");

	// No Depth Test, No Depth Write
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_DEPTHTEST_NO_WRITE);

	pShader->AddShaderParam(tShaderParam{ L"Diffuse Target Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Light Target Texture", SHADER_PARAM::TEX_1 });
	pShader->AddShaderParam(tShaderParam{ L"Specular Target Texture", SHADER_PARAM::TEX_2 });

	pShader->Create(SHADER_POV::LIGHTING);
	AddRes(L"MergeLightShader", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_ShadowMap", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_ShadowMap", "ps_5_0");
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::LESS);

	pShader->Create(SHADER_POV::SHADOWMAP);

	AddRes(L"ShadowMapShader", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\compute.fx", "CS_TEST", "cs_5_0");
	pShader->AddShaderParam(tShaderParam{ L"Test Value", SHADER_PARAM::INT_0 });
	AddRes(L"CSTestShader", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\particle.fx", "VS_Particle", "vs_5_0");
	pShader->CreateGeometryShader(L"Shader\\particle.fx", "GS_Particle", "gs_5_0");
	pShader->CreatePixelShader(L"Shader\\particle.fx", "PS_Particle", "ps_5_0");

	pShader->SetBlendState(BLEND_TYPE::ALPHABLEND); // ���� ���� ���
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::LESS_NO_WRITE); // �����׽�Ʈ o, ���� ��� x

	pShader->Create(SHADER_POV::PARTICLE, D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // TOPOLOGY �� �� ����(���� 1��)

	pShader->AddShaderParam(tShaderParam{ L"Start Scale", SHADER_PARAM::FLOAT_0 });
	pShader->AddShaderParam(tShaderParam{ L"End Scale", SHADER_PARAM::FLOAT_1 });
	pShader->AddShaderParam(tShaderParam{ L"Start Color", SHADER_PARAM::VEC4_0 });
	pShader->AddShaderParam(tShaderParam{ L"End Color", SHADER_PARAM::VEC4_1 });
	pShader->AddShaderParam(tShaderParam{ L"Particle Texture", SHADER_PARAM::TEX_0 });

	AddRes(L"ParticleShader", pShader);



	// ======================
	// Particle Update Shader
	// ======================
	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\particle.fx", "CS_ParticleUpdate", "cs_5_0");
	AddRes(L"ParticleUpdateShader", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\particle.fx", "CS_ParticleUpdate2", "cs_5_0");
	AddRes(L"ParticleUpdateShader2", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\particle.fx", "CS_ParticleUpdate3", "cs_5_0");
	AddRes(L"ParticleUpdateShader3", pShader);
	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\particle.fx", "CS_ParticleUpdate4", "cs_5_0");
	AddRes(L"ParticleUpdateShader4", pShader);
	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\particle.fx", "CS_ParticleUpdate5", "cs_5_0");
	AddRes(L"ParticleUpdateShader5", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\animation.fx", "CS_Animation3D", "cs_5_0");
	AddRes(L"Animation3DUpdateShader", pShader);


	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\BillBoard.fx", "VS_BillBoard", "vs_5_0");
	pShader->CreateGeometryShader(L"Shader\\BillBoard.fx", "GS_BillBoard", "gs_5_0");
	pShader->CreatePixelShader(L"Shader\\BillBoard.fx", "PS_BillBoard", "ps_5_0");
	pShader->SetBlendState(BLEND_TYPE::ALPHABLEND); // ���� ���� ���
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::LESS_NO_WRITE); // �����׽�Ʈ o, ���� ��� x

	pShader->Create(SHADER_POV::PARTICLE, D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // TOPOLOGY �� �� ����(���� 1

	pShader->AddShaderParam(tShaderParam{ L"Start Scalex", SHADER_PARAM::FLOAT_0 });
	pShader->AddShaderParam(tShaderParam{ L"End Scalex", SHADER_PARAM::FLOAT_1 });
	pShader->AddShaderParam(tShaderParam{ L"Start Scaley", SHADER_PARAM::FLOAT_2 });
	pShader->AddShaderParam(tShaderParam{ L"End Scaley", SHADER_PARAM::FLOAT_3 });

	AddRes(L"BillBoardShader", pShader);


	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\terrain.fx", "VS_Terrain", "vs_5_0");
	pShader->CreateHullShader(L"Shader\\terrain.fx", "HS_Terrain", "hs_5_0");
	pShader->CreateDomainShader(L"Shader\\terrain.fx", "DS_Terrain", "ds_5_0");
	pShader->CreatePixelShader(L"Shader\\terrain.fx", "PS_Terrain", "ps_5_0");

	pShader->SetRasterizerType(RS_TYPE::WIRE_FRAME);
	pShader->SetRasterizerType(RS_TYPE::CULL_BACK);
	pShader->Create(SHADER_POV::FORWARD, D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	AddRes(L"TerrainShader", pShader);


	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Range2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Range2D", "ps_5_0");

	// DepthStencilState ����
//	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::);

	pShader->Create(SHADER_POV::FORWARD, D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddRes(L"RangeShader", pShader);


}

void CResMgr::CreateDefaultMaterial()
{

	Ptr<CMaterial> pMtrl = nullptr;

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"TestShader"));
	AddRes(L"TestMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"TexShader"));
	AddRes(L"TexMtrl", pMtrl);

	int a = 0;
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Collider2DShader"));
	pMtrl->SetData(SHADER_PARAM::INT_0, &a);
	AddRes(L"Collider2DMtrl_0", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Collider2DShader"));
	a = 1;
	pMtrl->SetData(SHADER_PARAM::INT_0, &a);
	AddRes(L"Collider2DMtrl_1", pMtrl);



	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Std2DShader"));
	AddRes(L"Std2DMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"DirShader"));
	AddRes(L"DirMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Std3DShader"));
	AddRes(L"Std3DMtrl", pMtrl);



	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"SkyboxShader"));
	AddRes(L"SkyboxMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"GridShader"));
	//Ptr<CTexture> pPositionTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex");
	//pMtrl->SetData(SHADER_PARAM::TEX_0, pPositionTargetTex.GetPointer());
	AddRes(L"GridMtrl", pMtrl);

	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"DirLightShader"));

		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"NormalTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_0, pTargetTex.GetPointer());

		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, pTargetTex.GetPointer());

		AddRes(L"DirLightMtrl", pMtrl);
	}

	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"PointLightShader"));

		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"NormalTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_0, pTargetTex.GetPointer());

		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, pTargetTex.GetPointer());

		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
		pMtrl->SetData(SHADER_PARAM::VEC2_0, &vResolution);

		AddRes(L"PointLightMtrl", pMtrl);
	}

	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"MergeLightShader"));

		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"DiffuseTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_0, pTargetTex.GetPointer());

		pTargetTex = FindRes<CTexture>(L"DiffuseLightTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, pTargetTex.GetPointer());

		pTargetTex = FindRes<CTexture>(L"SpecularLightTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_2, pTargetTex.GetPointer());

		AddRes(L"MergeLightMtrl", pMtrl);
	}

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"CSTestShader"));
	AddRes(L"CSTestMtrl", pMtrl);

	// Particle Mtrl
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	AddRes(L"ParticleMtrl", pMtrl);

	// Particle Update
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"ParticleUpdateShader"));

	Ptr<CTexture> pNoiseTex = Load<CTexture>(L"Texture\\noise.png", L"Texture\\noise.png");
	pMtrl->SetData(SHADER_PARAM::TEX_0, pNoiseTex.GetPointer());
	Vec2 value = Vec2(pNoiseTex->Width(), pNoiseTex->Height());
	pMtrl->SetData(SHADER_PARAM::VEC2_0, &value);

	AddRes(L"ParticleUpdateMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"ParticleUpdateShader2"));
	pMtrl->SetData(SHADER_PARAM::TEX_0, pNoiseTex.GetPointer());
	pMtrl->SetData(SHADER_PARAM::VEC2_0, &value);
	AddRes(L"ParticleUpdate2Mtrl", pMtrl);


	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"ParticleUpdateShader3"));
	pMtrl->SetData(SHADER_PARAM::TEX_0, pNoiseTex.GetPointer());
	pMtrl->SetData(SHADER_PARAM::VEC2_0, &value);
	AddRes(L"ParticleUpdate3Mtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"ParticleUpdateShader4"));
	pMtrl->SetData(SHADER_PARAM::TEX_0, pNoiseTex.GetPointer());
	pMtrl->SetData(SHADER_PARAM::VEC2_0, &value);
	AddRes(L"ParticleUpdate4Mtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"ParticleUpdateShader5"));
	pMtrl->SetData(SHADER_PARAM::TEX_0, pNoiseTex.GetPointer());
	pMtrl->SetData(SHADER_PARAM::VEC2_0, &value);
	AddRes(L"ParticleUpdate5Mtrl", pMtrl);
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Animation3DUpdateShader"));
	AddRes(L"Animation3DUpdateMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"ShadowMapShader"));
	AddRes(L"ShadowMapMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"BillBoardShader"));
	AddRes(L"BillBoardMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"TerrainShader"));
	AddRes(L"TerrainMtrl", pMtrl);
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"RangeShader"));
	AddRes(L"RangeMtrl", pMtrl);

}

FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2);


void CResMgr::InitSound()
{

	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}
	
	


	// 32�� ä�� ����
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
	CSound::g_pFMOD->set3DSettings(1.0, 1.0f, 1.0f);


}

void CResMgr::Init()
{
	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultMaterial();
	InitSound();
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strName, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, const D3D12_HEAP_PROPERTIES& _HeapProperty, D3D12_HEAP_FLAGS _eHeapFlag, D3D12_RESOURCE_FLAGS _eResFlag, Vec4 _vClearColor)
{
	Ptr<CTexture> pNewTexture = new CTexture;
	pNewTexture->Create(_iWidth, _iHeight, _eFormat, _HeapProperty, _eHeapFlag, _eResFlag, _vClearColor);
	AddRes<CTexture>(_strName, pNewTexture);

	return pNewTexture;
}

Ptr<CTexture> CResMgr::CreateTextureFromResource(const wstring& _strName, ComPtr<ID3D12Resource> _pTex2D)
{
	Ptr<CTexture> pNewTexture = new CTexture;
	pNewTexture->CreateFromResource(_pTex2D);
	AddRes<CTexture>(_strName, pNewTexture);

	return pNewTexture;
}

Ptr<CMeshData> CResMgr::LoadFBX(const wstring& _strPath)
{
	wstring strFileName = CPathMgr::GetFileName(_strPath.c_str());

	wstring strName = L"MeshData\\";
	strName += strFileName + L".mdat";

	Ptr<CMeshData> pMeshData = FindRes<CMeshData>(strName);

	if (nullptr != pMeshData)
		return pMeshData;

	pMeshData = CMeshData::LoadFromFBX(_strPath);
	pMeshData->SetName(strName);
	pMeshData->SetPath(strName);
	m_mapRes[(UINT)RES_TYPE::MESHDATA].insert(make_pair(strName, pMeshData.GetPointer()));
	
	return pMeshData;
}
