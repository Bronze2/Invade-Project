#pragma once
#include "Component.h"

#include "Frustum.h"

enum class PROJ_TYPE {
    ORTHGRAPHIC,
    PERSPECTIVE,
};
class CCamera :
    public CComponent
{
private:
    CFrustum m_frustum;

    float m_fNear;
    float m_fFar;

    float m_fFOV;
    float m_fScale;

    Matrix m_matView;
    Matrix m_matViewInv;
    Matrix m_matProj;
    Matrix m_matProjInv;

    PROJ_TYPE m_eProjType;
    UINT m_iLayerCheck;

    float m_fWidth;
    float m_fHeight;


    vector<CGameObject*> m_vecParticle;
    vector<CGameObject*> m_vecShadowObj;
    bool m_bModule;

    vector<CGameObject*>	m_vecDeferred;
    vector<CGameObject*>	m_vecForward;
public:

    virtual void FinalUpdate();
    void SortGameObject();
    void SortShadowObject();
    void Render_Deferred();
    void Render_Forward();
    void Render();
    void Render_ShadowMap();

    void SetModule(bool _bModule) { m_bModule = _bModule; }

    void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; Changed(); }
    PROJ_TYPE GetProjType() { return m_eProjType; }
    void SetFar(float _far) { m_fFar = _far; Changed(); }
    void SetScale(float _fScale) { m_fScale = _fScale; if (m_fScale < 0.01f)m_fScale = 0.01f; Changed(); }
    void SetFov(float _fAngle) { m_fFOV = _fAngle; Changed(); }
    void SetNear(float _fNear) { m_fNear = _fNear; Changed(); }

    float GetFar() { return m_fFar; }
    float GetScale() { return m_fScale; }
    float GetFOV() { return m_fFOV; }

    void SetLayerCheck(int _iLayerIdx, bool _bAdd) {
        assert(_iLayerIdx != -1);
        if (_bAdd)
            m_iLayerCheck |= (1 << _iLayerIdx);
        else
            m_iLayerCheck &= ~(1 << _iLayerIdx);
    }
    void SetLayerAllCheck() { m_iLayerCheck = 0xfffffff; }
    void SetLayerAllRelease() { m_iLayerCheck = 0; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetViewInvMat() { return m_matViewInv; }
    const Matrix& GetProjMat() { return m_matProj; }
    const Matrix& GetProjInvMat() { return m_matProjInv; }

    void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
    void SetHeight(float _fHeight) { m_fHeight = _fHeight; }
    CCamera();
    virtual ~CCamera();
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

    CLONE(CCamera);

};

