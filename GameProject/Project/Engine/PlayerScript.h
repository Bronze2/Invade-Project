#pragma once
#include "Script.h"
#include "Animation3D.h"

class CTexture;
class CPlayerScript :
    public CScript
{
private:
    bool m_bCheckStartMousePoint;
    Vec3 m_vZoomPos;
    Vec3 m_vRestorePos;
    Vec3 m_vRestoreRot;

    ELEMENT_TYPE m_iType;

    float m_fRotateDegree;
    float m_fMoveSpeed;
    float m_fTurnDegree;
    float m_fFrontDegree;
    float m_fSideDegree;
    float m_fFactor;

    tMTAnimClip* m_pCurAnimClip;
    tMTAnimClip* m_pNextAnimClip;

    PLAYER_STATE m_ePrevState;
    PLAYER_STATE m_eState;

    float m_fLerpTime;
    float m_fMaxLerpTime;

    bool m_bCheckDegree;
    bool m_bColCheck;
    bool m_bMoveCheck;

    bool m_bTurn;

    vector<CGameObject*>m_arrColObject;
public:
    void m_FAnimation();

    void Init();
    virtual void Awake();
    virtual void Update();
    void SetType(ELEMENT_TYPE _iType) { m_iType = _iType; }
  
    void m_FColCheck(Vec3 _vBeforePos,Vec3 _vAfterPos);

    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);


    CPlayerScript();
    virtual ~CPlayerScript();



    CLONE(CPlayerScript);
};

