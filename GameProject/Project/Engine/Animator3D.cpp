#include "pch.h"
#include "Animator3D.h"
#include "TimeMgr.h"
#include "MeshRender.h"
#include "Material.h"
#include "ResMgr.h"
#include "StructuredBuffer.h"

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());

	static float fTime = 0.f;
	fTime += 1.f;
	m_vecClipUpdateTime[0] = fTime;
}

void CAnimator3D::UpdateData()
{
	if (!m_bFinalMatUpdate) {
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		pMesh->GetBoneFrameDataBuffer()->UpdateData_CS(TEXTURE_REGISTER::t10);
		pMesh->GetBoneOffsetBuffer()->UpdateData_CS(TEXTURE_REGISTER::t11);

		Check_Mesh(pMesh);
		m_pBoneFinalMat->UpdateRWData(UAV_REGISTER::u0);
		UINT iBoneCount = (UINT)m_pVecBones->size();
		UINT iRow = 0;
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_0, &iBoneCount);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_1, &m_iFrameIdx);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_2, &m_iNextFrameIdx);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_3, &iRow);
		m_pBoneMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fRatio);
		UINT iGrounX = (iBoneCount / 256) + 1;
		m_pBoneMtrl->Dispatch(iGrounX, 1, 1);
		m_bFinalMatUpdate = true;
	}
	m_pBoneFinalMat->UpdateData(TEXTURE_REGISTER::t7);
}


void CAnimator3D::Check_Mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMat->GetElementCount() < iBoneCount) {
		m_pBoneFinalMat->Create(sizeof(Matrix), iBoneCount, nullptr);
	}
}

void CAnimator3D::Update()
{
}

void CAnimator3D::LateUpdate()
{
}

void CAnimator3D::FinalUpdate()
{
	m_dCurTime = 0.f;
	// 현재 재생중인 Clip 의 시간을 진행한다.
	m_vecClipUpdateTime[m_iCurClip] += DT;
	auto p=m_pVecClip->at(m_iCurClip);
	if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
	{
		m_vecClipUpdateTime[m_iCurClip] = 0.f;
	}

	m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];

	double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	if (m_iFrameIdx >= m_pVecClip->at(m_iCurClip).iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;
	else
		m_iNextFrameIdx = m_iFrameIdx + 1;
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);
	m_bFinalMatUpdate = false;
}

void CAnimator3D::SaveToScene(FILE* _pFile)
{
}

void CAnimator3D::LoadFromScene(FILE* _pFile)
{
}

CAnimator3D::CAnimator3D():CComponent(COMPONENT_TYPE::ANIMATOR3D),m_iCurClip(0),m_dCurTime(0.f),m_iFrameCount(30),m_pBoneFinalMat(nullptr),m_bFinalMatUpdate(false)
{
	m_pBoneMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Animation3DUpdateMtrl");
	m_pBoneFinalMat = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE(m_pBoneFinalMat);
}
