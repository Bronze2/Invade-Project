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

    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_F; // Foward
    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_D; // Deferred
    map<INT_PTR, vector<tInstObj>>		m_mapSingleObj; // Single Object


    CGameObject* m_pPlayer;
    SimpleMath::Ray* m_pRay;
    Vec3 m_vLook;
    bool m_bPlayer = false;
    Vec3 m_vFront;

    vector<CGameObject*> m_arrInterSectObject;
    Matrix m_matCamera;
public:
    void SetMatrixCamera(const Matrix& _mat) { m_matCamera = _mat; }
    Vec3 GetLook() { return m_vLook; }
    void SetFront(const Vec3& _vFront) { m_vFront = _vFront; }
    Vec3 GetFront() { return m_vFront; }
    SimpleMath::Ray* GetRay() { return m_pRay; }
    void InterSectsObject(CCollider3D* _pCollider);
    virtual void FinalUpdate();
    void SortGameObject();
    void SortShadowObject();
    void Render_Deferred();
    void Render_Forward();
    void Render();
    void Render_ShadowMap();

    void ClearInterSectObject() { m_arrInterSectObject.clear(); }

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

    void SetPlayer(CGameObject* _pGameObject) { m_pPlayer = _pGameObject; }
    CGameObject* GetPlayer() { return m_pPlayer; }
    void SetbPlay(const bool& _Play) {
        m_bPlayer = _Play;
        if (m_bPlayer)
            m_pRay = new SimpleMath::Ray;
    }
    bool GetPlay() { return m_bPlayer; }

    CLONE(CCamera);

};
