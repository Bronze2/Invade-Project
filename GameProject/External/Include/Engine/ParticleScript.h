#pragma once
#include "Script.h"
class CParticleScript :
    public CScript
{
private:
    int m_iCoolTime=0;
    chrono::system_clock::time_point m_cstart;
    


public:
    void SetCoolTime(const int& _CoolTime) { m_iCoolTime = _CoolTime; }
    void SetTime() { m_cstart = chrono::system_clock::now(); }
    CParticleScript();
    virtual ~CParticleScript();

    void Update();




    CLONE(CParticleScript)
};

