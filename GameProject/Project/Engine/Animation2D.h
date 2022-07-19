#pragma once
#include "Entity.h"

#include "Animator2D.h"

// 2D  Animation 1 ������ ����
struct tAnim2DFrm
{
	Ptr<CTexture>	pTex;		// ���� �ؽ���
	Vec2			vLT;		// Left Top UV
	Vec2			vLen;		// UV Length
	Vec2			vOffsetPos; // �����ġ ��� �̵���
	float			fDuration;	// ������ ���ӽð�
};


class CAnimation2D :
	public CEntity
{
private:
	CAnimator2D*		m_pAnimator;
	vector<tAnim2DFrm>  m_vecFrm;
	int                 m_iCurFrm;

	float				m_fAccTime;
	bool				m_bFinish;

public:
	void finalupdate();
	void UpdateData();

	void Play()
	{
		m_iCurFrm = 0;
		m_bFinish = false;
		m_fAccTime = 0.f;
	}

	bool IsFinish() { return m_bFinish; }

public:
	void Create(Ptr<CTexture> _pTex, Vec2 _vLT, Vec2 _vGap, int _iFrmCount, float fDuration);
	CLONE(CAnimation2D);
	void SaveToScene(FILE* _pFile);
	void LoadFromScene(FILE* _pFile);

private:
	void SetAnimator2D(CAnimator2D* _pAnimator2D){m_pAnimator = _pAnimator2D;}
	

public:
	CAnimation2D();
	virtual ~CAnimation2D();

	friend class CAnimator2D;
};

