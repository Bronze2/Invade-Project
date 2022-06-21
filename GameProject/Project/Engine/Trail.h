#pragma once
#include "Ptr.h"

class CMaterial;
class CMesh;
class CTexture;
class CStructuredBuffer;
class CTrail
{
private:
	CStructuredBuffer*		m_pTrailBuffer;			// 트레일 데이터 저장 버퍼
	CStructuredBuffer*		m_pSharedBuffer;		// 트레일 공유 메모리

	Ptr<CMaterial>			m_pMtrl;
	Ptr<CMesh>				m_pMesh;

	float					m_fFrequency;			// 생성 주기
	float					m_fWidth;

public:
	virtual void FinalUpdate();
	virtual void Render();

	//void Init(Ptr<CTexture> _pTexture, const wstring& _MtrlName = L"ParticleUpdateMtrl");

public:
	CTrail();
	virtual ~CTrail();
	CLONE(CTrail);
};

