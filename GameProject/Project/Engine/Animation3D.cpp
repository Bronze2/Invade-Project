#include "pch.h"
#include "Animation3D.h"

void CAnimation::InsertAnimation(const wstring& _Name, const UINT& _StartFrame, const UINT& _EndFrame, bool _bRepeat, bool bStop)
{
	if (nullptr == FindAnimation(_Name)) {
		tAnimation* tNewAnimation = new tAnimation;
		tNewAnimation->Name = _Name;
		tNewAnimation->StartFrame =_StartFrame;
		tNewAnimation->EndFrame = _EndFrame;
		tNewAnimation->Repeat = _bRepeat;
		tNewAnimation->Stop = bStop;
		m_tAnimation.push_back(tNewAnimation);
	}

}

void CAnimation::InsertAnimClip(const wstring& _Name, const UINT& _StartFrame, const UINT& _EndFrame)
{
	if (nullptr == FindAnimClip(_Name)) {
		tMTAnimClip* tNewAnimClip = new tMTAnimClip;
		tNewAnimClip->strAnimName = _Name;
		tNewAnimClip->iStartFrame = _StartFrame;
		tNewAnimClip->iEndFrame = _EndFrame;
		tNewAnimClip->iFrameLength = _EndFrame - _StartFrame;
		tNewAnimClip->dStartTime = (double)_StartFrame / (double)30;
		tNewAnimClip->dEndTime = (double)_EndFrame / (double)30;
		tNewAnimClip->dTimeLength = tNewAnimClip->dEndTime - tNewAnimClip->dStartTime;

		m_pVecAnimClip.push_back(*tNewAnimClip);
		m_pThrash.push_back(tNewAnimClip);
	}
}

tAnimation* CAnimation::FindAnimation(const wstring& _strName)
{
	for (int i = 0; i < m_tAnimation.size(); ++i) {
		if (_strName == m_tAnimation[i]->Name)
		{
			return m_tAnimation[i];
		}
	}

	return nullptr;
}

tMTAnimClip* CAnimation::FindAnimClip(const wstring& _strName)
{
	for (int i = 0; i < m_pVecAnimClip.size(); ++i) {
		if (_strName == m_pVecAnimClip[i].strAnimName)
		{
			return &m_pVecAnimClip[i];
		}
	}

	return nullptr;
}

void CAnimation::SetAnimation(tAnimation* _pAnimation)
{
	if (nullptr == FindAnimation(_pAnimation->Name)) {
		m_tAnimation.push_back(_pAnimation);
	}
}

void CAnimation::SetAnimClip(tMTAnimClip _pAnimClip)
{
	if (nullptr == FindAnimClip(_pAnimClip.strAnimName)) {
		m_pVecAnimClip.push_back(_pAnimClip);
	}
}

void CAnimation::Update()
{
}

void CAnimation::FinalUpdate()
{
}

CAnimation::CAnimation()
{
}

CAnimation::~CAnimation()
{
	Safe_Delete_Vector(m_tAnimation);

	Safe_Delete_Vector(m_pThrash);
}
