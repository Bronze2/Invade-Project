#pragma once
#include "Ptr.h"

class CMaterial;
class CMesh;
class CTexture;
class CStructuredBuffer;
class CTrail
{
private:
	CStructuredBuffer*		m_pTrailBuffer;			// Ʈ���� ������ ���� ����
	CStructuredBuffer*		m_pSharedBuffer;		// Ʈ���� ���� �޸�

	Ptr<CMaterial>			m_pMtrl;
	Ptr<CMesh>				m_pMesh;

	float					m_fFrequency;			// ���� �ֱ�
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

