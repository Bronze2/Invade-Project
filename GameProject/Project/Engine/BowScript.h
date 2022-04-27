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

#ifdef ARROW_TEST
    CGameObject* m_pArrow[20];
    CGameObject* m_pEmptyArrow;         // 활에 붙어있는 가짜 활 (발사안됨)
    UINT m_iCurArrow;
    Ptr<CTexture>pBlackTex;
    int m_iPower;
    ELEMENT_TYPE m_iType;
    float m_fArrowSpeed;

    float m_fArcherLocation;

    Vec3 m_vArrowRestorePos;

    bool m_bMaxCharged;
    bool m_bCheckDegree;
#endif


public:
    void m_FAnimation();

    void Init();
    virtual void Awake();
    virtual void Update();

    void SetTarget(CGameObject* _target) { m_pTargetObject = _target; }
    void SetBoneIdx(int _boneIdx) { m_iTargetBoneIdx = _boneIdx; }

    void SetState(BOW_STATE _eState) { m_eState = _eState; }
    
    CBowScript();
    virtual ~CBowScript();

    CLONE(CBowScript);
};

