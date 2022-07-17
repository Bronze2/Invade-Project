#include "pch.h"
#include "TrailRenderer.h"
#include "ResMgr.h"
#include "Transform.h"
#include "ArrowScript.h"
#include "Device.h"
#include "MeshRender.h"

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
	m_fCurTime += DT;

	//for (auto& p : m_pTrails) {
	//	p.fCurTime += DT;
	//}

	ErasePoint();

	if (m_bEmit)
	{
		CGameObject* pObj = GetObj()->GetParent();
		// 화살에 달린 트레일일 때 
		if (nullptr != GetObj()->GetParent()->GetScript<CArrowScript>()) {
			/*Vec3 vDir = pObj->GetScript<CArrowScript>()->Get*/
			Vec3 vArrowPos = pObj->Transform()->GetLocalPos();
			Vec3 vArrowDir = pObj->GetScript<CArrowScript>()->GetDir();
			Vec3 vTrailPos = vArrowPos - vArrowDir * 40.f;

			if (m_fCurTime > m_fEmitTime) {
				EmitPoint(vTrailPos, pObj->GetScript<CArrowScript>()->GetRotateAngle());
				m_fCurTime = 0.f;
			}
		}
	}
	else
	{
		for (auto& p : m_pTrails) {
			p.fCurTime += DT;
		}
		//m_pTrails.clear();
		m_fCurTime = 0.f;
		m_iCount = 0.f;
	}
}

void CTrailRenderer::FinalUpdate()
{
	VTX v = {};
	vector<VTX> vecVTX;
	vector<UINT> vecIdx;
	m_pMesh = nullptr;

	Interpolate(10);

	// 캣멀롬써서 보간해야댐
	// 최종 정점으로 메쉬 생성
	if (!m_pTrails.empty()) {
		// 2  3
		// 0  1

		for (int i = 0; i < m_pTrails.size(); i++) {
			v.vPos = m_pTrails[i].vPos;
			v.vColor = m_vColor;

			float ratio = (float)(m_pTrails[i].iIdx / m_pTrails.size());
			if (i % 2 == 0) {
				v.vUV = Vec2(0.f, ratio);
			}
			else {
				v.vUV = Vec2(1.f, ratio);
			}

			vecVTX.push_back(v);

		}
		//cout << "-----------------------------------------------" << endl;
		//cout << "VTX : " << vecVTX.size() << endl;
		//for (auto& v : vecVTX) {
		//	cout << v.vPos.x << ", " << v.vPos.y << ", " << v.vPos.z << endl;
		//}
		//cout << endl << endl;

		//for (int i = m_pTrails.size() - 2; i > 2; i -= 2) {
		//	// 0 1 2 3 4 5 6 7 이면 8 (i = 6 4 2
		//	// 16 17
		// //  14 15
		// ....
		//	// 8 9
		//	// 6 7
		//	// 4 5
		//	// 2 3
		//	// 0 1
		//	// -> 3번 돌아야됨 -> 6 4 2 
		//	cout << m_pTrails.size() << " - " << i << endl;

		//	vecIdx.push_back(0 + i); vecIdx.push_back(1 + i); vecIdx.push_back(-1 + i);
		//	vecIdx.push_back(0 + i); vecIdx.push_back(-1 + i); vecIdx.push_back(-2 + i);
		//}
		//cout << endl;

		// 6 7
		// 4 5
		// 2 3
		// 0 1
		// -> size 8 (0 2 4 6)

		for (int i = 0; i < m_pTrails.size(); i += 2) {
			vecIdx.push_back(1 + i); vecIdx.push_back(3 + i); vecIdx.push_back(2 + i);
			vecIdx.push_back(1 + i); vecIdx.push_back(2 + i); vecIdx.push_back(0 + i);

		}
		//cout << "Idx : " << vecIdx.size() << endl;
		////for (auto& i : vecIdx) {
		////	cout << i << endl;
		////}
		//cout << "--------------------------------------------------" << endl;
		//cout << endl << endl;


		//for (int i = m_pTrails.size() - 2; i < 2; i -= 2) {		// 6 4 2
		//	vecIdx.push_back(i); vecIdx.push_back(i + 1); vecIdx.push_back(i - 2);
		//	vecIdx.push_back(i + 1); vecIdx.push_back(i - 1); vecIdx.push_back(i - 2);
		//}


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

void CTrailRenderer::EmitPoint(Vec3 _vPos, float _fAngle)
{
	// 일정 시간마다 정점 생성
	/*
	TrailPoint Info{};
	
	// 앵글 추가해서 x y z에서 

	float ratio = m_pTrails.size() / m_iMaxTrail;
	float fScale = (m_fMaxWidth - m_fMinWidth) * ratio + m_fMinWidth;

	float fAngle1 = 0.f;
	float fAngle2 = 0.f;
	// 0 1 2 3 -> size 4 
	//if (m_pTrails.size() != 0) {
	//	Vec3 vPrevPos1 = m_pTrails.at(m_pTrails.size() - 2).vPos;
	//	Vec3 vPrevPos2 = m_pTrails.at(m_pTrails.size() - 1).vPos;
	//	fAngle1 = atan2(_vPos.x - vPrevPos1.x, _vPos.z - vPrevPos1.z);
	//	fAngle2 = atan2(_vPos.x - vPrevPos2.x, _vPos.z - vPrevPos2.z);
	//}

	float fAngle = _fAngle;

	Info.vPos = _vPos;
	// 이거 포지션을 화살이 나가는 방향대로 받아오자

	//Vec3 vDir = GetObj()->GetParent()->GetScript<CArrowScript>()->GetXZDir();
	Vec3 vArrowDir = GetObj()->GetParent()->GetScript<CArrowScript>()->GetXZDir();
	Vec3 vDir = Vec3(vArrowDir.x * cos(90.f) + (-sin(90.f) * vArrowDir.z), vArrowDir.y, vArrowDir.x * sin(90.f) + cos(90.f) * vArrowDir.z);
	Info.vPos += vDir * fScale / 2.f;
	//Info.vPos.z -= vDir.z * fScale / 2;
	//Info.vPos.x += fScale / 2;  
	//Info.vPos.z -= fScale / 2;
	//Info.vPos.y += fScale / 2;
	//Info.vPos.x -= vDir.x * sin(fAngle1) * fScale / 2;
	//Info.vPos.z += vDir.z * cos(fAngle1) * fScale / 2;
	//Info.fCurTime = 0.f;
	//Info.fLifeTime = m_fLifeTime;
	//Info.iIdx = m_iCount;
	//m_pTrails.push_back(Info);
	m_vecVtx.push_back(Info.vPos);

	Info.vPos = _vPos;
	Info.vPos -= vDir * fScale / 2.f;
	//Info.vPos.z += vDir.z * fScale / 2;
	//Info.vPos.x -= fScale / 2;
	//Info.vPos.z += fScale / 2;
	//Info.vPos.y -= fScale / 2;
	//Info.vPos.x += vDir.x * sin(fAngle2) * fScale / 2;
	//Info.vPos.z -= vDir.z * cos(fAngle2) * fScale / 2;
	//Info.fCurTime = 0.f;
	//Info.fLifeTime = m_fLifeTime;
	//Info.iIdx = m_iCount;
	//m_pTrails.push_back(Info);
	m_vecVtx.push_back(Info.vPos);

	m_iCount++;
	*/
	Vec3 vArrowDir = GetObj()->GetParent()->GetScript<CArrowScript>()->GetXZDir();
	Vec3 vDir = Vec3(vArrowDir.x * cos(90.f) + (-sin(90.f) * vArrowDir.z), vArrowDir.y, vArrowDir.x * sin(90.f) + cos(90.f) * vArrowDir.z);
	float ratio = m_pTrails.size() / m_iMaxTrail;
	float fScale = (m_fMaxWidth - m_fMinWidth) * ratio + m_fMinWidth;

	Vec3 vLeftPoint = _vPos;
	vLeftPoint -= vDir * fScale / 2.f;
	m_vecPosLeft.emplace_back(vLeftPoint);

	Vec3 vRightPoint = _vPos;
	vRightPoint += vDir * fScale / 2.f;
	m_vecPosRight.emplace_back(vRightPoint);
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
			Info.vPosRigh = m_vecVtxRight[i];
			m_pTrails.emplace_back(Info);
		}
	}
	else
	{
		m_pTrails.clear();
	}
}

void CTrailRenderer::Init(Ptr<CTexture> _pTex)
{
	m_pTex = _pTex;
	m_pMtrl = new CMaterial;
	m_pMtrl->DisableFileSave();
	m_pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3DShader"));
	GetObj()->MeshRender()->SetMaterial(m_pMtrl);
	GetObj()->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, m_pTex.GetPointer());
}

void CTrailRenderer::SaveToScene(FILE* _pFile)
{
}

void CTrailRenderer::LoadFromScene(FILE* _pFile)
{
}

CTrailRenderer::CTrailRenderer() : CComponent(COMPONENT_TYPE::TRAILRENDERER)
, m_pTrails{}
, m_fEmitTime(0.2f)
, m_bEmit(false)
, m_iMaxTrail(20)
, m_fLifeTime(1.f)
, m_iCount(0)
{

}

CTrailRenderer::~CTrailRenderer()
{
	delete m_pMtrl.GetPointer();
}