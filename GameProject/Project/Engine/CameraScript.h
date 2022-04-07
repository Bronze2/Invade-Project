#pragma once
#include "Script.h"
class CCameraScript :
    public CScript
{
private:
    float m_fSpeed;
    float m_fShakeTime;
    float m_fScaleSpeed;
    float m_fShakeNum;
    bool m_bCheckStartMousePoint;
    bool m_bCheckCameraShake;

    float m_fDegree;
  
    Vec2 m_vStartMousePoint;
    Vec3 m_vRestorePos;

public:
    CCameraScript();
    virtual ~CCameraScript();

    virtual void Update();

    float GetDegree() { return m_fDegree; }

    void SetStartMousePoint(const Vec2& _vStartMousePoint) { m_vStartMousePoint = _vStartMousePoint; }

    void CameraShake(float _fTime);

    CLONE(CCameraScript);


};

