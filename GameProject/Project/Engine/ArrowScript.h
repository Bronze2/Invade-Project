#pragma once
#include "Script.h"
#include "PlayerScript.h"

enum class ARROW_STATE
{
    IDLE,
    ATTACK_READY,
    ATTACK,
};

class CArrowScript :
    public CScript
{
private:
    float m_fSpeed;
    Vec3 m_vDir;
    Vec3 m_vStartPos;
    float m_fTime;
    float m_fVelocityY;
    float m_fVelocityX;
    float m_fVelocityZ;
    float a;
    ELEMENT_TYPE m_iType;
    CGameObject* m_pParticle;
    bool m_bMove;
    float m_fFallSpeed;

    float m_fyPos = 0.f;
    float m_fxPos = 0.f;

    float m_fDotValue;
    bool m_bSetDotValue = false;

    bool m_bCheckDegree;
    bool m_bMaxCharged;

    ARROW_STATE m_eState;
    Vec3 m_vRestorePos;
    Vec3 m_vTargetDir;
    Vec3 m_vDeltaPos;
    
    CGameObject* m_pBow;

    int m_ParentId;
    int m_id;
public:
    virtual void Awake();
    virtual void Update();

    void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }

    void SetFallSpeed(float _fSpeed) { m_fFallSpeed = _fSpeed; }
    void SetDir(Vec3 _vDir) { m_vDir = _vDir; }
    void SetTime(float _fTime) { m_fTime = _fTime; }
    void SetStartPos(Vec3 _vPos) { m_vStartPos = _vPos; }
    void SetVelocityY(float _fVelocityY) { m_fVelocityY = _fVelocityY; }
    void SetVelocityX() { m_fVelocityX = m_vDir.x * m_fSpeed * DT; }
    void SetVelocityZ() { m_fVelocityZ = m_vDir.z * m_fSpeed * DT; }
    void SetType(UINT _iType) { m_iType = (ELEMENT_TYPE)_iType; }
    void SetMove(bool _bMove) { m_bMove = _bMove; }
    void SetParentId(int _pid) { m_ParentId = _pid; }
    int GetParentId() { return m_ParentId; }

    void m_SetId(int _pid) { m_id = _pid; }
    int   m_GetId() { return m_id; }
    void Init();

    void SetState(ARROW_STATE _eState) { m_eState = _eState; }
    void SetMaxCharged(bool _bTrue) { m_bMaxCharged = _bTrue; }

    void SetRestorePos(const Vec3 _vPos) { m_vRestorePos = _vPos; }

    void SetBow(CGameObject* _Obj) { m_pBow = _Obj; }

    virtual void OnCollision3DEnter(CCollider3D* _pColldier);
private:

public:
    CLONE(CArrowScript);

    CArrowScript(ELEMENT_TYPE _iType);
    virtual ~CArrowScript();
};