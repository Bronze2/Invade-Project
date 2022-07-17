#pragma once
#include "Component.h"
#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"
#include "Script.h"

struct TrailPoint
{
	Vec3						vPos;
	float						fCurTime;			// 생성 후 경과 시간
	float						fLifeTime;			// 생존 시간
	int							iIdx;
};


class CTrailRenderer
	: public CComponent
{
private:
	//Ptr<CMesh>					m_pMesh;
	//vector<Ptr<CMesh>>			m_pMesh;
	//vector<Ptr<CMaterial>>		m_pMtrl;
	Ptr<CTexture>				m_pTex;

	Vec3						m_vColor;
	float						m_fMaxWidth;
	float						m_fMinWidth;

	bool						m_bEmit;			// 정점 생성 여부

	float						m_fCurTime;			// 정점 생성 후 경과 시간
	float						m_fEmitTime;		// 정점 생성 기준 시간
	float						m_fLifeTime;		// 정점 생존 시간

	//vector<TrailPoint>			m_vecVtx;
	//vector<UINT>				m_vecIdx;

	vector<TrailPoint>			m_pTrails;
	int							m_iMaxTrail;

	//ComPtr<ID3D12Resource>		m_pVB;
	//D3D12_VERTEX_BUFFER_VIEW	m_tVtxView;

	//CStructuredBuffer*			m_pTrailBuffer;
	//CStructuredBuffer*			m_pSharedBuffer;
	//Ptr<CMaterial>				m_pUpdateMtrl;
	Ptr<CMesh>					m_pMesh;
	Ptr<CMaterial>				m_pMtrl;
	int							m_iCount;

public:
	void Init(Ptr<CTexture> _pTex);
	virtual void Update();
	virtual void FinalUpdate();

	void SetColor(Vec4 _vColor) { m_vColor = _vColor; }
	void SetMaxWidth(float _fWidth) { m_fMaxWidth = _fWidth; }
	void SetMinWidth(float _fWidth) { m_fMinWidth = _fWidth; }
	void SetLifeTime(float _fLifeTime) { m_fLifeTime = _fLifeTime; }

	bool GetEmit() { return m_bEmit; }
	void SetEmit(bool _bTrue) { m_bEmit = _bTrue; }

	void EmitPoint(Vec3 _vPos, float _fAngle);
	void ErasePoint();

public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CTrailRenderer();
	virtual ~CTrailRenderer();
	CLONE(CTrailRenderer);
};

