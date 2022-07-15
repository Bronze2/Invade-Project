#include "pch.h"
#include "TrailRenderer.h"
#include "ResMgr.h"
#include "Transform.h"
#include "ArrowScript.h"
#include "Device.h"
#include "MeshRender.h"

void CTrailRenderer::Update()
{
	m_fCurTime += DT;

	for (auto& p : m_pTrails) {
		p.fCurTime += DT;
	}

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
		//m_pTrails.clear();
		m_fCurTime = 0.f;
	}
}

void CTrailRenderer::FinalUpdate()
{
	VTX v = {};
	vector<VTX> vecVTX;
	vector<UINT> vecIdx;
	m_pMesh = nullptr;

	// 캣멀롬써서 보간해야댐
	// 최종 정점으로 메쉬 생성
	if (!m_pTrails.empty()) {
		// 2  3
		// 0  1

		for (int i = 0; i < m_pTrails.size(); i++) {
			v.vPos = m_pTrails[i].vPos;
			v.vColor = m_vColor;

			float ratio = (float)(i / m_pTrails.size());
			if (i % 2 == 0) {
				v.vUV = Vec2(0.f, 1.f - ratio);
			}
			else {
				v.vUV = Vec2(1.f, 1.f - ratio);
			}

			vecVTX.push_back(v);
		}

		//for (int i = m_pTrails.size() - 2; i > 2; i -= 2) {
		//	// 0 1 2 3 4 5 6 7 이면 8 (i = 6 4 2
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
		
		// 0 1 
		// 2 3
		// -> 0 1 2 / 1 3 2 

		// 6 7
		// 4 5
		// 2 3
		// 0 1
		// -> size 8 / 6 7 4 / 7 5 4

		for (int i = 0; i < m_pTrails.size(); i += 2) {
			vecIdx.push_back(0 + i); vecIdx.push_back(1 + i); vecIdx.push_back(3 + i);
			vecIdx.push_back(1 + i); vecIdx.push_back(3 + i); vecIdx.push_back(2 + i);
		}

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

	TrailPoint Info{};
	
	// 앵글 추가해서 x y z에서 

	float ratio = m_pTrails.size() / m_iMaxTrail;
	float fScale = m_fMaxWidth - (m_fMaxWidth - m_fMinWidth) * ratio;

	float fAngle1 = 0.f;
	float fAngle2 = 0.f;
	// 0 1 2 3 -> size 4 
	if (m_pTrails.size() != 0) {
		Vec3 vPrevPos1 = m_pTrails.at(m_pTrails.size() - 2).vPos;
		Vec3 vPrevPos2 = m_pTrails.at(m_pTrails.size() - 1).vPos;
		fAngle1 = atan2(_vPos.x - vPrevPos1.x, _vPos.z - vPrevPos1.z);
		fAngle2 = atan2(_vPos.x - vPrevPos2.x, _vPos.z - vPrevPos2.z);
	}

	float fAngle = _fAngle;
	

	Info.vPos = _vPos;
	//Info.vPos.x -= fScale / 2;
	Info.vPos.x -= sin(fAngle1) * fScale / 2;
	Info.vPos.z += cos(fAngle1) * fScale / 2;
	Info.fCurTime = 0.f;
	Info.fLifeTime = m_fLifeTime;
	m_pTrails.push_back(Info);

	Info.vPos = _vPos;
	//Info.vPos.x += fScale / 2;
	Info.vPos.x += sin(fAngle2) * fScale / 2;
	Info.vPos.z -= cos(fAngle2) * fScale / 2;
	Info.fCurTime = 0.f;
	Info.fLifeTime = m_fLifeTime;
	m_pTrails.push_back(Info);

	m_iCount++;
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

	if (m_pTrails.size() > m_iMaxTrail)
	{
		m_pTrails.erase(m_pTrails.begin(), m_pTrails.begin() + 1);
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
, m_fEmitTime(0.5f)
, m_bEmit(false)
, m_iMaxTrail(30)
, m_fLifeTime(1.f)
, m_iCount(0)
{

}

CTrailRenderer::~CTrailRenderer()
{
	delete m_pMtrl.GetPointer();
}