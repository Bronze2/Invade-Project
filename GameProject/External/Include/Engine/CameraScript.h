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

    bool m_bBackMode;
    Vec3 m_vBackRestorePos;

    CAMERA_EFFECT_TYPE m_tEffectType;

    bool m_bWind0Camera;

    // 사망수정
    bool m_bDead;

public:

    CCameraScript();
    virtual ~CCameraScript();

    virtual void Update();

    void SetDead(bool _bTrue) { m_bDead = _bTrue; }
    bool GetDead() { return m_bDead; }

    float GetDegree() { return m_fDegree; }

    void SetStartMousePoint(const Vec2& _vStartMousePoint) { m_vStartMousePoint = _vStartMousePoint; }

    Vec3 CameraShake(Vec3 _vPos, float _fDamageTime, float _fDamageSize);
    Vec3 CameraZoom(Vec3 _vPos);

    void SetBackMode(bool _bTrue) { m_bBackMode = _bTrue; }
    void SetBackRestorePos(Vec3 _vPos) { m_vBackRestorePos = _vPos; }

    void SetWind0Camera(bool _bTrue) { m_bWind0Camera = _bTrue; }

    void Init();

    CLONE(CCameraScript);

    //¿©±â¼­ºÎÅÍ

private:
    float m_vCameraDistanceOffset;
    CGameObject* m_pPlayer;


public:
    void SetDistanceOffset(CGameObject* _pObject);






};