#pragma once
#include "Script.h"
class CParticleScript :
    public CScript
{
private:
    float m_iCoolTime=0;
    chrono::system_clock::time_point m_cstart;
    

    CSound* m_pSound;

public:
    void SetCoolTime(const float& _CoolTime) { m_iCoolTime = _CoolTime; }
    void SetTime() { m_cstart = chrono::system_clock::now(); }
    void SetSound(CSound* _pSound) { m_pSound = _pSound; }
    CParticleScript();
    virtual ~CParticleScript();

    void Update();




    CLONE(CParticleScript)
};

