#include "pch.h"
#include "TrailRenderer.h"
#include "ResMgr.h"
#include "Transform.h"
#include "ArrowScript.h"
#include "Device.h"
#include "MeshRender.h"
#include "Collider3D.h"

CatmullRomSpline::CatmullRomSpline(const vector<Vec3>& data) : segment_count(data.size() - 1)
{
	assert(data.size() >= 2);
	m_vControlPoints.resize(data.size() + 2);
	copy(data.begin(), data.end(), m_vControlPoints.begin() + 1);
	m_vControlPoints[0] = 2 * data[0] - data[1];
	m_vControlPoints[m_vControlPoints.size() - 1] = 2 * data[data.size() - 1] - data[data.size() - 2];
}

void CatmullRomSpline::interpolate(vector<Vec3>& vInterpolatedData, size_t steps)
{
	for (size_t segment_index = 0; segment_index < segment_count; ++segment_index)
	{
		interpolate(segment_index, vInterpolatedData, steps);
	}
	vInterpolatedData.emplace_back(m_vControlPoints[segment_count + 1]);
}

void CatmullRomSpline::interpolate(size_t segment, vector<Vec3>& vInterpolatedData, size_t steps)
{
	assert(segment < segment_count);
	const float power = 0.5f; 
	Vec3 P0 = m_vControlPoints[segment + 0];
	Vec3 P1 = m_vControlPoints[segment + 1];
	Vec3 P2 = m_vControlPoints[segment + 2];
	Vec3 P3 = m_vControlPoints[segment + 3];
	Vec3 V0 = (P2 - P0) * power;
	Vec3 V1 = (P3 - P1) * power;
	for (size_t step = 0; step < steps; ++step)
	{
		const float t = static_cast<float>(step) / steps;

		Vec3 P;
		P = t * t * t * (2 * P1 - 2 * P2 + V0 + V1);
		P += t * t * (-3 * P1 + 3 * P2 - 2 * V0 - V1);
		P += t * V0 + P1;

		Vec3 vInterpolatedPoint{};
		vInterpolatedPoint = P;
		vInterpolatedData.emplace_back(vInterpolatedPoint);
	}

}

void CTrailRenderer::Update()
{
	m_fCurTime += DT * 3;

	if (m_bEmit)
	{
		// 화살에 달린 트레일일 때 
		if (nullptr != m_pObj->GetScript<CArrowScript>()) {
			if (m_fCurTime > m_fEmitTime) {
				EmitPoint(m_pObj->Transform()->GetWorldPos());
				m_fCurTime = 0.f;
			}
		}
	}
	else
	{
		m_fCurTime = 0.f;
		m_iCount = 0.f;
	}

	ErasePoint();
}

void CTrailRenderer::FinalUpdate()
{
	VTX v = {};
	vector<VTX> vecVTX;
	vector<UINT> vecIdx;
	if (nullptr != m_pMesh) {
		m_pMesh.~Ptr();
		m_pMesh = nullptr;
	}

	Interpolate(10);

	// 최종 정점으로 메쉬 생성
	if (m_pTrails.size() > 1) {

		for (int i = 0; i < m_vecVtxLeft.size(); i++) {
			float fRatio = 1.f / m_vecVtxLeft.size();
			v.vColor = m_vColor;

			v.vPos = m_vecVtxLeft[i];
			v.vUV = Vec2(0.f, i * fRatio);
			vecVTX.push_back(v);
			//cout << v.vPos.x << ", " << v.vPos.y << ", " << v.vPos.z << endl;

			v.vPos = m_vecVtxRight[i];
			v.vUV = Vec2(1.f, i * fRatio);
			vecVTX.push_back(v);
			//cout << v.vPos.x << ", " << v.vPos.y << ", " << v.vPos.z << endl;
		}

		for (int i = 0; i < (vecVTX.size() - 2); i += 2) {
			vecIdx.push_back(1 + i); vecIdx.push_back(0 + i); vecIdx.push_back(2 + i);
			vecIdx.push_back(1 + i); vecIdx.push_back(2 + i); vecIdx.push_back(3 + i);
			vecIdx.push_back(1 + i); vecIdx.push_back(3 + i); vecIdx.push_back(2 + i);
			vecIdx.push_back(1 + i); vecIdx.push_back(2 + i); vecIdx.push_back(0 + i);
		}

		m_pMesh = new CMesh;
		m_pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
			, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());
		
		GetObj()->MeshRender()->SetMesh(m_pMesh);
	}
	else {
		GetObj()->MeshRender()->SetMesh(nullptr);
	}

	vecVTX.clear();
	vecIdx.clear();
}

void CTrailRenderer::EmitPoint(Vec3 _vPos)
{
	// 일정 시간마다 정점 생성
	float ratio = 1.f / m_pTrails.size();
	float fScale = m_fMaxWidth - (m_fMaxWidth - m_fMinWidth) * ratio;

	Vec3 vLeftPoint = _vPos;
	vLeftPoint.x -= m_fMinWidth / 2;
	//vLeftPoint.x -= fScale / 2;
	m_vecPosLeft.emplace_back(vLeftPoint);

	Vec3 vRightPoint = _vPos;
	vRightPoint.x += m_fMinWidth / 2;
	//vRightPoint.x += fScale / 2;
	m_vecPosRight.emplace_back(vRightPoint);

	//cout << "Left : " << vLeftPoint.x << ", " << vLeftPoint.y << ", " << vLeftPoint.z << endl;
	//cout << "Right : " << vRightPoint.x << ", " << vRightPoint.y << ", " << vRightPoint.z << endl;
}

void CTrailRenderer::ErasePoint()
{
	// 생존시간 초과한 정점들 소멸
	//for (int i = 0; i < m_pTrails.size(); ++i) {
	//	if (m_pTrails[i].fCurTime > m_pTrails[i].fLifeTime) {
	//		if (i % 2 == 1) {	// 두 정점 중 하나만 삭제 막기
	//			m_pTrails.erase(m_pTrails.begin(), m_pTrails.begin() + i);
	//		}
	//		else {
	//			m_pTrails.erase(m_pTrails.begin(), m_pTrails.begin() + i - 1);
	//		}
	//		return;
	//	}
	//}

	//if (m_bEmit && m_pTrails.size() >= m_iMaxTrail)
	//{
	//	m_pTrails.erase(m_pTrails.begin(), m_pTrails.begin() + 1);
	//}
	//else if (!m_bEmit) {
	//	for (int i = 0; i < m_pTrails.size(); ++i) {
	//		if (i % 2 == 1) {	// 두 정점 중 하나만 삭제 막기
	//			m_pTrails.erase(m_pTrails.begin(), m_pTrails.begin() + i);
	//		}
	//		else {
	//			m_pTrails.erase(m_pTrails.begin(), m_pTrails.begin() + i - 1);
	//		}
	//	}
	//	if (m_pTrails.size() == 1) {
	//		m_pTrails.clear();
	//	}
	//}

	if (m_bEmit && m_vecPosLeft.size() >= m_iMaxTrail)
	{
		m_vecPosLeft.erase(m_vecPosLeft.begin());
		m_vecPosRight.erase(m_vecPosRight.begin());
		m_pTrails.erase(m_pTrails.begin());
	}
	else if (!m_bEmit && m_vecPosLeft.size() >= 1) {
		m_fClearCurTime += DT * 3;
		if (m_fClearCurTime >= m_fLifeTime) {
			m_vecPosLeft.erase(m_vecPosLeft.begin());
			m_vecPosRight.erase(m_vecPosRight.begin());
			m_pTrails.erase(m_pTrails.begin());
			m_fClearCurTime = 0.f;
		}
		if (m_vecPosLeft.size() == 1) {
			m_vecPosLeft.clear();
			m_vecPosRight.clear();
			m_pTrails.clear();
		}
	}
}

void CTrailRenderer::Interpolate(size_t steps)
{
	if (m_vecPosLeft.size() > 1 && m_vecPosRight.size() > 1)
	{
		CatmullRomSpline LeftCurve(m_vecPosLeft);
		m_vecVtxLeft.clear();
		LeftCurve.interpolate(m_vecVtxLeft, steps);

		CatmullRomSpline RightCurve(m_vecPosRight);
		m_vecVtxRight.clear();
		RightCurve.interpolate(m_vecVtxRight, steps);

		m_pTrails.clear();
		for (int i = 0; i < m_vecVtxLeft.size(); i++) {
			TrailPoint Info{};
			Info.vPosLeft = m_vecVtxLeft[i];
			Info.vPosRight = m_vecVtxRight[i];
			m_pTrails.emplace_back(Info);
		}
	}
	else
	{
		m_vecVtxLeft.clear();
		m_vecVtxRight.clear();
		m_pTrails.clear();
	}
}

void CTrailRenderer::Init(Ptr<CTexture> _pTex)
{
	m_pMtrl = new CMaterial;
	m_pMtrl->DisableFileSave();
	m_pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TrailShader"));
	GetObj()->MeshRender()->SetMaterial(m_pMtrl);
	GetObj()->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, _pTex.GetPointer());
}

void CTrailRenderer::SaveToScene(FILE* _pFile)
{
}

void CTrailRenderer::LoadFromScene(FILE* _pFile)
{
}

CTrailRenderer::CTrailRenderer() : CComponent(COMPONENT_TYPE::TRAILRENDERER)
, m_pTrails{}
, m_fEmitTime(0.1f)
, m_bEmit(false)
, m_iMaxTrail(15)
, m_fLifeTime(0.1f)
, m_iCount(0)
, m_fClearCurTime(0.f)
{

}

CTrailRenderer::~CTrailRenderer()
{
	delete m_pMtrl.GetPointer();
}