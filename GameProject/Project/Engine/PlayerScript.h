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
    bool mainClient;

    float m_fArcherLocation;

    MINION_STATE m_eState;
    tAnimation* m_CurAnimation;

public:
    void Init();
    virtual void Awake();
    virtual void Update();
    void SetType(ELEMENT_TYPE _iType) { m_iType = _iType; }
    void SetMain() { mainClient = true; }
    void SetState(MINION_STATE state);

    CPlayerScript();
    virtual ~CPlayerScript();
    void do_Animation();


    CLONE(CPlayerScript);
};

