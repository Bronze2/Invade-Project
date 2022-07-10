#pragma once
#include "Script.h"
#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"

class CStructuredBuffer;
class CTrailScript
	: public CScript
{
private:
	Ptr<CMaterial>			m_pMtrl;
	Ptr<CMesh>				m_pMesh;

	float					m_fFrequency;			// ���� �ֱ�
	float					m_fStartScale;			// ���� ��
	float					m_fEndScale;			// ������ ��


public:
	void Init(Ptr<CTexture> _pTexture);

	virtual void Update();
	virtual void FinalUpdate();
	virtual void Render();

	void SetStartScale(float _fStartScale) { m_fStartScale = _fStartScale; }
	void SetEndScale(float _fEndScale) { m_fEndScale = _fEndScale; }

	//void Init(Ptr<CTexture> _pTexture, const wstring& _MtrlName = L"ParticleUpdateMtrl");

public:
	CTrailScript();
	virtual ~CTrailScript();

	CLONE(CTrailScript);
};

