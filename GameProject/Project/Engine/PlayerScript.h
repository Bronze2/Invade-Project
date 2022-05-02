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
    CGameObject* m_pArrow[20];
    UINT m_iCurArrow;
    Ptr<CTexture>pBlackTex;
    int m_iPower;
    ELEMENT_TYPE m_iType;
    float m_fArrowSpeed;

    float m_fArcherLocation;

    tAnimation* m_pCurAnimation;
    PLAYER_STATE m_eState;
    PLAYER_STATE m_ePrevState;

    tMTAnimClip* m_pCurAnimClip;
    tMTAnimClip* m_pNextAnimClip;

    Vec3 m_LerpPos;

    bool isMain;
    vector<CGameObject*>m_arrColObject;
public:
    void m_FAnimation();
    void Init();
    virtual void Awake();
    virtual void Update();
    void SetType(ELEMENT_TYPE _iType) { m_iType = _iType; }
    void SetMain() { isMain = true; }
    void SetState(int state);

    void Update_LerpPos();
    void SetLerpPos(Vec3 Pos) { m_LerpPos  = Pos; }
    CPlayerScript();
    virtual ~CPlayerScript();

    void m_FColCheck(Vec3 _vBeforePos, Vec3 _vAfterPos);

    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);


    CLONE(CPlayerScript);
};

