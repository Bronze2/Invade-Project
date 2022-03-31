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

private:


public:
	void InsertAnimation(const wstring& _Name, const UINT& _StartFrame, const UINT& _EndFrame, bool _bRepeat, bool bStop);
	tAnimation* FindAnimation(const wstring& _strName);
	void SetAnimation(tAnimation* _pAnimation);


	void Update();
	void FinalUpdate();

	CAnimation();
	~CAnimation();
};

