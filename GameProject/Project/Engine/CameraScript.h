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

    CAMERA_EFFECT_TYPE m_tEffectType;



public:
  
    CCameraScript();
    virtual ~CCameraScript();

    virtual void Update();

    float GetDegree() { return m_fDegree; }

    void SetStartMousePoint(const Vec2& _vStartMousePoint) { m_vStartMousePoint = _vStartMousePoint; }

    Vec3& CameraShake(Vec3 _vPos, float _fDamageTime, float _fDamageSize);
    Vec3& CameraZoom(Vec3 _vPos);

    void SetZoomDir(Vec3 _vDir) { m_vZoomRestoreFront = _vDir; }

    void Init();

    CLONE(CCameraScript);

    //여기서부터

private:
    float m_vCameraDistanceOffset;
    CGameObject* m_pPlayer;
    

};
