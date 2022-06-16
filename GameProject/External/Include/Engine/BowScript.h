#pragma once
#include "Script.h"
#include "Animation3D.h"

enum class BOW_STATE {
    IDLE,
    ATTACK_READY,
    ATTACK,
};

class CBowScript
    :public CScript
{
private:
    tAnimation* m_pCurAnimation;
    
    tMTAnimClip* m_pCurAnimClip;
    tMTAnimClip* m_pNextAnimClip;

    tMTBone* m_pTargetBone;
    CGameObject* m_pTargetObject;
    int m_iTargetBoneIdx;

    BOW_STATE m_eState;
    BOW_STATE m_ePrevState;

    CGameObject* m_pArrow[20];
    UINT m_iCurArrow;
    Ptr<CTexture>pBlackTex;
    int m_iPower;
    ELEMENT_TYPE m_iType;
    float m_fArrowSpeed;

    float m_fArcherLocation;

    Vec3 m_vArrowRestorePos;

    bool m_bMaxCharged;
    bool m_bCheckDegree;
      

    CGameObject* m_pPlayer;
public:

    CGameObject* GetCurArrow() { return m_pArrow[m_iCurArrow]; }
    void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
    

    void m_FAnimation();

    void Init();
    virtual void Awake();
    virtual void Update();

    void SetTarget(CGameObject* _target) { m_pTargetObject = _target; }
    void SetBoneIdx(int _boneIdx) { m_iTargetBoneIdx = _boneIdx; }

    void SetState(BOW_STATE _eState) { m_eState = _eState; }

    float GetArrowSpeed() { return m_fArrowSpeed; }
    
    CBowScript();
    virtual ~CBowScript();

    CLONE(CBowScript);
};

