#pragma once
#include "Script.h"
class CCameraScript :
    public CScript
{
private:
    float m_fSpeed;
    float m_fScaleSpeed;
    bool m_bCheckStartMousePoint;

    float m_fDegree;
  
    Vec2 m_vStartMousePoint;
public:
    virtual void Update();
    CCameraScript();
    virtual ~CCameraScript();

    float GetDegree() { return m_fDegree; }

    void SetStartMousePoint(const Vec2& _vStartMousePoint) { m_vStartMousePoint = _vStartMousePoint; }

    CLONE(CCameraScript);


};

