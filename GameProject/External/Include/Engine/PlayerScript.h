#pragma once
#include "Script.h"
#include "Animation3D.h"
enum class ELEMENT_TYPE {
    FROZEN=0,
    FIRE=1,
    DARK=2,
    THUNDER=3,
    WIND=4
};

enum class PLAYER_STATE {
    IDLE,
    WALK,
    ATTACK,
    JUMP,
    DIE,
};

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



    bool m_bColCheck;
    bool m_bMoveCheck;
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

