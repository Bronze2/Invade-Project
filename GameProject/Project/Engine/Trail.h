#pragma once
#include "Component.h"
#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"

class CStructuredBuffer;
class CTrail
	: public CComponent
{
private:
	CStructuredBuffer*		m_pTrailBuffer;			// Ʈ���� ������ ���� ����

	Ptr<CMaterial>			m_pMtrl;
	Ptr<CMesh>				m_pMesh;

	float					m_fFrequency;			// ���� �ֱ�
	float					m_fStartScale;			// ���� ��
	float					m_fEndScale;			// ������ ��


public:
	void Init(Ptr<CTexture> _pTexture);

	virtual void FinalUpdate();
	virtual void Render();

	void SetStartScale(float _fStartScale) { m_fStartScale = _fStartScale; }
	void SetEndScale(float _fEndScale) { m_fEndScale = _fEndScale; }

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
	//void Init(Ptr<CTexture> _pTexture, const wstring& _MtrlName = L"ParticleUpdateMtrl");

public:
	CTrail();
	virtual ~CTrail();
	CLONE(CTrail);
};
