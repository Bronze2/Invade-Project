#pragma once
#include "Script.h"
class CToolCamScript :
    public CScript
{
private:
    float m_fSpeed;
    float m_fScaleSpeed;
public:
    virtual void Update();
    CToolCamScript();
    virtual ~CToolCamScript();

    CLONE(CToolCamScript);


};

