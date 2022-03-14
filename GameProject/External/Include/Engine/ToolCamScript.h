#pragma once
#include "Script.h"
class CToolCamScript :
    public CScript
{
private:
    float m_fSpeed;
    float m_fScaleSpeed;
    bool m_bCheckStartMousePoint;
  
    Vec2 m_vStartMousePoint;
public:
    virtual void Update();
    CToolCamScript();
    virtual ~CToolCamScript();

    void SetStartMousePoint(const Vec2& _vStartMousePoint) { m_vStartMousePoint = _vStartMousePoint; }

    CLONE(CToolCamScript);


};

