#pragma once
#include "Script.h"
#include "Animation3D.h"
class CMinionScript :
    public CScript
{
    MINION_STATE m_eState;
    float m_fSpeed;
    float m_fRange;
    MINION_CAMP m_eCamp;
    tAnimation* m_CurAnimation;
public:
    CLONE(CMinionScript)
    void Init();
    virtual void Update();
    void SetState(const MINION_STATE& _eState) { m_eState = _eState; }
    const MINION_STATE& GetState()const { return m_eState; }
    const MINION_CAMP& GetCamp()const { return m_eCamp; }
    CMinionScript();
    CMinionScript(float _fSpeed,float _fRange,MINION_STATE _eState,MINION_CAMP _eCamp);
    virtual ~CMinionScript();
};

