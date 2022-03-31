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
		tNewAnimation->Stop =bStop;
		m_tAnimation.push_back(tNewAnimation);
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

void CAnimation::SetAnimation(tAnimation* _pAnimation)
{
	if (nullptr == FindAnimation(_pAnimation->Name)) {
		m_tAnimation.push_back(_pAnimation);
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
}
