#pragma once
#include "Script.h"
class CParticleScript :
    public CScript
{
private:
    int m_iCoolTime=0;
    chrono::system_clock::time_point m_cstart;
    
    Ptr<CSound> m_pSound;

public:
    void SetCoolTime(const int& _CoolTime) { m_iCoolTime = _CoolTime; }
    void SetTime() { m_cstart = chrono::system_clock::now(); }
    CParticleScript();
    virtual ~CParticleScript();

    void Update();

    void SetSound(Ptr<CSound> _pSound) { m_pSound = _pSound; }


    CLONE(CParticleScript)
};

