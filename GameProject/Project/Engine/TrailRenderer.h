#pragma once
#include "Component.h"
#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"

class CTrailRenderer
	: public CComponent
{
private:	
	Ptr<CMaterial>		m_pMtrl;			// ������ ���̴�
	Ptr<CMesh>			m_pMesh;			// ��� �޽�

	Vec4				m_vColor;			// Ʈ���� ����
	float				m_fWidth;			// Ʈ���� �ʺ�
	float				m_fHeight;			// Ʈ���� ����
	
	float				m_fCurTime;			// ���� ���� �� ��� �ð�
	float				m_fEmitTime;		// ���� ���� ���� �ð�

	bool				m_bEmit;			// ���� ���� ����

	CGameObject*		m_pObj;				// Ʈ���� ��� ������Ʈ	

public:
	void Init(Ptr<CTexture> _pTex);
	virtual void Update();
	virtual void FinalUpdate();
	virtual void Render();

	void SetColor(Vec4 _vColor) { m_vColor = _vColor; }
	void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
	void SetHeight(float _fHeight) { m_fHeight = _fHeight; }

	void EmitPoint(Vec3 _vPos, float _fAngle);
	// ���� �ð����� �� �������� Point ���� (Update���� ȣ��)
	// bool ������ ������ �ƴ��� �Ǻ�

	bool GetEmit() { return m_bEmit; }
	void SetEmit(bool _bTrue) { m_bEmit = _bTrue; }

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
public:
	CTrailRenderer();
	virtual ~CTrailRenderer();
	CLONE(CTrailRenderer);
};

