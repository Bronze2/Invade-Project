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
    BOW_STATE m_eState;
    BOW_STATE m_ePrevState;

public:
    void m_FAnimation();
    void Init();
    virtual void Awake();
    virtual void Update();

    void ChangeState(BOW_STATE _state) { m_eState = _state; }

    CBowScript();
    virtual ~CBowScript();

    CLONE(CBowScript);
};

