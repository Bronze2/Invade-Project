#include "pch.h"
#include "Animator3D.h"
#include "TimeMgr.h"
#include "MeshRender.h"
#include "Material.h"
#include "ResMgr.h"
#include "StructuredBuffer.h"
#include "SceneMgr.h"
#include "PlayerScript.h"
#include "CameraScript.h"
#include "CrossHairScript.h"

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize((UINT)PLAYER_STATE::END);
	for (int i = 0; i < m_vecClipUpdateTime.size(); i++) {
		m_vecClipUpdateTime[i] = 0.f;
	}
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

		float fDegree = 0;
		float fIsPlayer = 0.f;
		if (GetObj()->GetScript<CPlayerScript>() != nullptr) { 
			fIsPlayer = 1.f;
			CGameObject* pMainCam = dynamic_cast<CGameObject*>(CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->GetParentObj()[1])->GetChild()[1];
			Vec3 vRot = pMainCam->Transform()->GetLocalRot();
			fDegree = XMConvertToDegrees(vRot.x);
		}
		m_pBoneMtrl->SetData(SHADER_PARAM::FLOAT_1, &fDegree);
		m_pBoneMtrl->SetData(SHADER_PARAM::FLOAT_2, &fIsPlayer);

		UINT iGrounX = (iBoneCount / 256) + 1;
		m_pBoneMtrl->Dispatch(iGrounX, 1, 1);
		m_bFinalMatUpdate = true;
	}

	m_pBoneFinalMat->UpdateData(TEXTURE_REGISTER::t7);

}

void CAnimator3D::UpdateData_Inst(CStructuredBuffer* _pBoneBuffer, UINT _iRow)
{
	if (!m_bFinalMatUpdate)
	{
		// Bone Data Update	
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		pMesh->GetBoneFrameDataBuffer()->UpdateData_CS(TEXTURE_REGISTER::t10);
		pMesh->GetBoneOffsetBuffer()->UpdateData_CS(TEXTURE_REGISTER::t11);

		Check_Mesh(pMesh);
		_pBoneBuffer->UpdateRWData(UAV_REGISTER::u0);

		UINT iBoneCount = (UINT)m_pVecBones->size();
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_0, &iBoneCount);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_1, &m_iFrameIdx);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_2, &m_iNextFrameIdx);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_3, &_iRow);
		m_pBoneMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fRatio);



		UINT iGrounX = (iBoneCount / 256) + 1;
		m_pBoneMtrl->Dispatch(iGrounX, 1, 1);

		m_bFinalMatUpdate = true;
	}
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

	if (m_bBlendAnimation) 
	{
		m_fBlendFrame += 0.3f;
		m_vecClipUpdateTime[m_iCurClip] += (DT * m_fAnimationSpeed);
		m_vecClipUpdateTime[m_iNextClip] += (DT * m_fAnimationSpeed);
		if (nullptr == m_pAnimation) {
			m_dCurTime = 0.f;
		

			if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
			{
				m_vecClipUpdateTime[m_iCurClip] = 0.f;
			}

			m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];
		}
		else {
			if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
			{
				m_vecClipUpdateTime[m_iCurClip] = m_pVecClip->at(m_iCurClip).dTimeLength;
			}
			if (m_vecClipUpdateTime[m_iNextClip] >= m_pVecClip->at(m_iNextClip).dTimeLength)
			{
				m_vecClipUpdateTime[m_iNextClip] = m_pVecClip->at(m_iNextClip).dTimeLength;
			}

			m_dCurTime = m_dStartFrameTime + m_vecClipUpdateTime[m_iCurClip];
			m_dNextTime = m_dStartNextFrameTime + m_vecClipUpdateTime[m_iNextClip];
		}
		double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
		m_iFrameIdx = (int)(dFrameIdx);

		double dNextFrameIdx = m_dNextTime * (double)m_iFrameCount;
		m_iNextFrameIdx = (int)(dNextFrameIdx);

		m_fRatio = m_fBlendFrame / m_fBlendMaxFrame;
		m_bFinalMatUpdate = false;

		
		if (m_fRatio >= m_fBlendMaxFrame) {
			m_bBlendAnimation = false;
			m_fBlendFrame = 0.f;
			m_dStartFrameTime = m_dStartNextFrameTime;
			m_vecClipUpdateTime[m_iCurClip] = m_vecClipUpdateTime[m_iNextClip];
			m_iCurClip = m_iNextClip;
			m_iFrameIdx = m_iNextFrameIdx;
		}
	}
	else
	{
		m_vecClipUpdateTime[m_iCurClip] += (DT * m_fAnimationSpeed);
		if (nullptr == m_pAnimation) {
			m_dCurTime = 0.f;

			if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
			{
				m_vecClipUpdateTime[m_iCurClip] = m_pVecClip->at(m_iCurClip).dTimeLength;
			}

			m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];
		}
		else {
			if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
			{
				m_vecClipUpdateTime[m_iCurClip] = m_pVecClip->at(m_iCurClip).dTimeLength;
			}

			m_dCurTime = m_dStartFrameTime + m_vecClipUpdateTime[m_iCurClip];
		}
		double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
		m_iFrameIdx = (int)(dFrameIdx);

		if (m_iFrameIdx >= m_pVecClip->at(m_iCurClip).iEndFrame - 1)
			m_iNextFrameIdx = m_iFrameIdx;
		else
			m_iNextFrameIdx = m_iFrameIdx + 1;

		m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);
		m_bFinalMatUpdate = false;
	}
}

void CAnimator3D::SaveToScene(FILE* _pFile)
{
}

void CAnimator3D::LoadFromScene(FILE* _pFile)
{
}


CAnimator3D::CAnimator3D() :CComponent(COMPONENT_TYPE::ANIMATOR3D), m_fAnimationSpeed(1.f), m_iCurClip(0), m_dCurTime(0.f), m_iFrameCount(30), m_fRatio(0.f), m_pBoneFinalMat(nullptr), m_bFinalMatUpdate(false), m_bBlendAnimation(false), m_fBlendFrame(0.f), m_fBlendMaxFrame(1.f)
{
	m_pBoneMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Animation3DUpdateMtrl");
	m_pBoneFinalMat = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE(m_pBoneFinalMat);
	SAFE_DELETE(m_pAnimation);
}
