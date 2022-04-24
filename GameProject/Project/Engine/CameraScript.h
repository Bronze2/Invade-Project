#pragma once
#include "Script.h"

class CCameraScript :
    public CScript
{
private:
    float m_fSpeed;
    float m_fScaleSpeed;
    float m_fShakeNum;
    float m_fZoomSpeed;
    float m_fZoomElapsedTime;

    bool m_bCheckStartMousePoint;

    float m_fDegree;

    Vec2 m_vStartMousePoint;
    Vec3 m_vZoomRestoreFront;
    Vec3 m_vRestorePos;
    
    Vec3 m_vShootPos;

    CAMERA_EFFECT_TYPE m_tEffectType;

public:
    CCameraScript();
    virtual ~CCameraScript();

    virtual void Update();

    float GetDegree() { return m_fDegree; }

    void SetStartMousePoint(const Vec2& _vStartMousePoint) { m_vStartMousePoint = _vStartMousePoint; }

    Vec3& CameraShake(Vec3 _vPos, float _fDamageTime, float _fDamageSize);
    Vec3& CameraZoom(Vec3 _vPos);

    Vec3& GetShootPos() { return m_vShootPos; }

    void Init();

    CLONE(CCameraScript);
};
