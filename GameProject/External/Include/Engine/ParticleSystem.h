#pragma once
#include "Component.h"
#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"
class CStructuredBuffer;
class CParticleSystem :
    public CComponent
{
private:
	CStructuredBuffer* m_pParticleBuffer; // ��ƼŬ ������ ����
	CStructuredBuffer* m_pSharedBuffer;   // ��ƼŬ ���� �޸�
	UINT				m_iMaxParticle;    // �ִ� ��ƼŬ ����

	Ptr<CMaterial>		m_pUpdateMtrl;		// ��ǻƮ ���̴�
	Ptr<CMaterial>		m_pMtrl;			// ��ƼŬ ��������
	Ptr<CMesh>			m_pMesh;			// ��� �޽�(Point Mesh)

	float				m_fFrequency;		// ��ƼŬ ���� �ֱ�
	float				m_fAccTime;		    // �����ð� üũ�뵵

	float				m_fMinLifeTime;   // ��ƼŬ �ּ� ���� �ð�
	float				m_fMaxLifeTime;   // ��ƼŬ �ִ� ���� �ð�

	float				m_fMinSpeed;	  // ��ƼŬ ���� �ӵ�
	float				m_fMaxSpeed;	  // ��ƼŬ ���� �ӵ�

	float				m_fStartScale;   // ��ƼŬ ���� ũ��
	float				m_fEndScale;     // ��ƼŬ ���� ũ��

	Vec4				m_vStartColor;	// ��ƼŬ �ʱ� ����
	Vec4				m_vEndColor;    // ��ƼŬ ���� ����
	Vec4				m_vDir;

	Vec2 m_vTime;

public:
	virtual void FinalUpdate();
	virtual void Render();
	void Init( Ptr<CTexture> _pTexture, const wstring& _MtrlName= L"ParticleUpdateMtrl");


	void SetMinSpeed(float _fMinSpeed) { m_fMinSpeed = _fMinSpeed; }
	void SetMaxSpeed(float _fMaxSpeed) { m_fMaxSpeed = _fMaxSpeed; }
	void SetStartScale(float _fStartScale) { m_fStartScale = _fStartScale; }
	void SetEndScale(float _fEndScale) { m_fEndScale = _fEndScale; }
	void SetMinLifeTime(float _fMinLifeTime) { m_fMinLifeTime = _fMinLifeTime; }
	void SetMaxLifeTime(float _fMaxLifeTime) { m_fMaxLifeTime = _fMaxLifeTime; }
	void SetFrequency(float _fFrequency) { m_fFrequency = _fFrequency; }
	void SetDir(Vec4 _vDir) { m_vDir = _vDir; }
	void SetAccTime(float _fAccTime) { m_fAccTime = _fAccTime; }
public:
	void SetStartColor(const Vec4& _vColor) { m_vStartColor = _vColor; }
	void SetEndColor(const Vec4& _vColor) { m_vEndColor = _vColor; }

public:
	
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CParticleSystem();
	virtual ~CParticleSystem();
	CLONE(CParticleSystem);
};

