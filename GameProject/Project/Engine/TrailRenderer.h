#pragma once
#include "Component.h"
#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"
#include "Script.h"

struct TrailPoint
{
	Vec3						vPosLeft;
	Vec3						vPosRight;
};

class CatmullRomSpline
{
private:
	vector<Vec3> m_vControlPoints;
public:
	CatmullRomSpline(const vector<Vec3>& data);
	virtual ~CatmullRomSpline() = default;
	CatmullRomSpline(const CatmullRomSpline&) = delete;
	CatmullRomSpline& operator=(const CatmullRomSpline&) = delete;
	CatmullRomSpline(CatmullRomSpline&&) noexcept = delete;
	CatmullRomSpline& operator=(CatmullRomSpline&&) noexcept = delete;

	const size_t segment_count;
	void interpolate(vector<Vec3>& interpolated_data, size_t steps);
	void interpolate(size_t segment, vector<Vec3>& interpolated_data, size_t steps);
};


class CTrailRenderer
	: public CComponent
{
private:
	Ptr<CTexture>				m_pTex;

	Vec3						m_vColor;
	float						m_fMaxWidth;
	float						m_fMinWidth;

	bool						m_bEmit;			// ���� ���� ����

	float						m_fCurTime;			// ���� ���� �� ��� �ð�
	float						m_fClearCurTime;
	float						m_fEmitTime;		// ���� ���� ���� �ð�
	float						m_fLifeTime;		// ���� ���� �ð�

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

	vector<Vec3>				m_vecPosLeft;
	vector<Vec3>				m_vecPosRight;
	vector<Vec3>				m_vecVtxLeft;
	vector<Vec3>				m_vecVtxRight;

	CGameObject*				m_pObj;

public:
	void Init(Ptr<CTexture> _pTex);
	virtual void Update();
	virtual void FinalUpdate();

	void SetColor(Vec4 _vColor) { m_vColor = _vColor; }
	void SetMaxWidth(float _fWidth) { m_fMaxWidth = _fWidth; }
	void SetMinWidth(float _fWidth) { m_fMinWidth = _fWidth; }
	void SetLifeTime(float _fLifeTime) { m_fLifeTime = _fLifeTime; }

	void SetTargetObj(CGameObject* _pObj) { m_pObj = _pObj; }

	bool GetEmit() { return m_bEmit; }
	void SetEmit(bool _bTrue) { m_bEmit = _bTrue; }

	void EmitPoint(Vec3 _vPos);
	void ErasePoint();

	void Interpolate(size_t steps);

public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CTrailRenderer();
	virtual ~CTrailRenderer();
	CLONE(CTrailRenderer);
};

