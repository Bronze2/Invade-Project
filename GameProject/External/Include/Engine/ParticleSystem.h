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
	CStructuredBuffer* m_pParticleBuffer; // 파티클 데이터 저장
	CStructuredBuffer* m_pSharedBuffer;   // 파티클 공유 메모리
	UINT				m_iMaxParticle;    // 최대 파티클 개수

	Ptr<CMaterial>		m_pUpdateMtrl;		// 컴퓨트 쉐이더
	Ptr<CMaterial>		m_pMtrl;			// 파티클 렌더링용
	Ptr<CMesh>			m_pMesh;			// 사용 메쉬(Point Mesh)

	float				m_fFrequency;		// 파티클 생성 주기
	float				m_fAccTime;		    // 누적시간 체크용도

	float				m_fMinLifeTime;   // 파티클 최소 생존 시간
	float				m_fMaxLifeTime;   // 파티클 최대 생존 시간

	float				m_fMinSpeed;	  // 파티클 시작 속도
	float				m_fMaxSpeed;	  // 파티클 최종 속도

	float				m_fStartScale;   // 파티클 시작 크기
	float				m_fEndScale;     // 파티클 최종 크기

	Vec4				m_vStartColor;	// 파티클 초기 색상
	Vec4				m_vEndColor;    // 파티클 최종 색상

public:
	virtual void FinalUpdate();
	virtual void Render();
	void Init( Ptr<CTexture> _pTexture);


	void SetMinSpeed(float _fMinSpeed) { m_fMinSpeed = _fMinSpeed; }
	void SetMaxSpeed(float _fMaxSpeed) { m_fMaxSpeed = _fMaxSpeed; }
	void SetStartScale(float _fStartScale) { m_fStartScale = _fStartScale; }
	void SetEndScale(float _fEndScale) { m_fEndScale = _fEndScale; }
	void SetMinLifeTime(float _fMinLifeTime) { m_fMinLifeTime = _fMinLifeTime; }
	void SetMaxLifeTime(float _fMaxLifeTime) { m_fMaxLifeTime = _fMaxLifeTime; }
	void SetFrequency(float _fFrequency) { m_fFrequency = _fFrequency; }
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

