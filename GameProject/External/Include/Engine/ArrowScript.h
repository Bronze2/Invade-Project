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
    float m_fDir = 1;
    float m_fHighest = 0;
    float m_fPerRotate = 1;

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

    float m_fyPos=0.f;
    float m_fxPos = 0.f;

    float m_fDotValue;
    bool m_bSetDotValue = false;

    bool m_bCheckDegree;
    bool m_bMaxCharged;

    // Ʈ����
    CGameObject* m_pTrail;
    float m_fAngle;
    float m_fRotateAngle;

    ARROW_STATE m_eState;
    Vec3 m_vRestorePos;
    Vec3 m_vTargetDir;
    Vec3 m_vDeltaPos;

    Vec3 m_vQtrnRotAxis;

    CGameObject* m_pBow;

    UINT m_iLayerIdx;

    UINT m_iDamage;
    SKILL* m_pSkill;
    CGameObject* m_pPlayer;

public:
    void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
    bool bSetSkill() { if (nullptr == m_pSkill)return true; else return false; }
    void SetSkill(SKILL* _pSkill); 
    void SetLayerIdx(UINT _Idx) { m_iLayerIdx = _Idx; }
    UINT GetLayerIdx() { return m_iLayerIdx; }
    virtual void Awake();
    virtual void Update();

    void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
   
    void SetFallSpeed(float _fSpeed) { m_fFallSpeed = _fSpeed; }
    void SetDir(Vec3 _vDir) { m_vDir = _vDir; }
    void SetTime(float _fTime) { m_fTime = _fTime; }
    void SetStartPos(Vec3 _vPos) { m_vStartPos = _vPos; }
    void SetVelocityY(float _fVelocityY) { m_fVelocityY = _fVelocityY; }
    void SetVelocityX() { m_fVelocityX = m_vDir.x * m_fSpeed  * DT; }
    void SetVelocityZ() { m_fVelocityZ = m_vDir.z * m_fSpeed * DT; }
    // Ʈ����
    void SetAngle(float _Angle) { m_fAngle = _Angle; }
    float GetAngle() { return m_fAngle; }
    float GetRotateAngle() { return m_fRotateAngle; }
    Vec3 GetDir() { return m_vDir; }

    void SetQtrnRotAxis(Vec3 _Axis) { m_vQtrnRotAxis = _Axis; }

    void SkillCheck();
    void SetType(const UINT& _iType) { m_iType = (ELEMENT_TYPE)_iType; }
    void SetMove(bool _bMove) { m_bMove = _bMove; }
    void Init();

    void SetState(ARROW_STATE _eState) { m_eState = _eState; }
    void SetMaxCharged(bool _bTrue) { m_bMaxCharged = _bTrue; }

    void SetRestorePos(const Vec3 _vPos) { m_vRestorePos = _vPos; }

    void SetBow(CGameObject* _Obj) { m_pBow = _Obj; }

    virtual void OnCollision3DEnter(CCollider3D* _pColldier);


    //��ų
    void WaterSkill0(CCollider3D* _pColldier);
    void DarkSkill0(CCollider3D* _pCollider);
    void ThunderSkill0(CCollider3D* _pCollider);
    void ThunderSkill1(CCollider3D* _pCollider);
    void FireSkill0(CCollider3D* _pCollider);
    void FireSkill1(CCollider3D* _pCollider);
    void WindSkill0(CCollider3D* _pCollider);
    void WindSkill1(CCollider3D* _pCollider);
private:
  
public:
    CLONE(CArrowScript);

    CArrowScript(ELEMENT_TYPE _iType);
    virtual ~CArrowScript();
};

