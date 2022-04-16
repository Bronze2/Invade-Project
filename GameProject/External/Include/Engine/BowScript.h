#pragma once
#include "Script.h"
#include "Animation3D.h"

enum class BOW_STATE {
    IDLE,
    ATTACK,
};

class CBowScript
    :public CScript
{
private:
    tAnimation* m_pCurAnimation;

    tMTBone* m_pTargetBone;
    CGameObject* m_pTargetObject;
    int m_iTargetBoneIdx;

    BOW_STATE m_eState;
    BOW_STATE m_ePrevState;

public:
    void m_FAnimation();
    void m_FSetAnimation();
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

