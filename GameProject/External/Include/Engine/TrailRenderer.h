#pragma once
#include "Component.h"
#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"

class CTrailRenderer
	: public CComponent
{
private:	
	Ptr<CMaterial>		m_pMtrl;			// 렌더링 쉐이더
	Ptr<CMesh>			m_pMesh;			// 사용 메쉬

	Vec4				m_vColor;			// 트레일 색상
	float				m_fFrequency;		// 트레일 생성 주기
	float				m_fWidth;			// 트레일 너비
	float				m_fHeight;			// 트레일 높이

public:
	void Init(Ptr<CTexture> _pTex);
	virtual void Update();
	virtual void FinalUpdate();
	virtual void Render();

	void SetColor(Vec4 _vColor) { m_vColor = _vColor; }
	void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
	void SetHeight(float _fHeight) { m_fHeight = _fHeight; }

	void EmitPoint(Vec3 _vPos, float _fAngle);
	// 일정 시간마다 점 기준으로 Point 생성 (Update에서 호출)
	// bool 변수로 끝인지 아닌지 판별

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
public:
	CTrailRenderer();
	virtual ~CTrailRenderer();
	CLONE(CTrailRenderer);
};

