#pragma once

struct tAnimation {
	wstring Name;
	UINT StartFrame;
	UINT EndFrame;
	bool Stop;
	bool Repeat;
};


class CAnimation
{

private:
	vector<tAnimation*> m_tAnimation;
	vector<tMTAnimClip> m_pVecAnimClip;

private:


public:
	void InsertAnimation(const wstring& _Name, const UINT& _StartFrame, const UINT& _EndFrame, bool _bRepeat, bool bStop);
	void InsertAnimClip(const wstring& _Name, const UINT& _StartFrame, const UINT& _EndFrame);

	tAnimation* FindAnimation(const wstring& _strName);
	tMTAnimClip* FindAnimClip(const wstring& _strName);

	void SetAnimation(tAnimation* _pAnimation);
	void SetAnimClip(tMTAnimClip _pAnimClip);

	const vector<tAnimation*>& GetAnimation() { return m_tAnimation; }
	const vector<tMTAnimClip>* GetAnimClip() { return &m_pVecAnimClip; }

	void Update();
	void FinalUpdate();

	CAnimation();
	~CAnimation();
};

