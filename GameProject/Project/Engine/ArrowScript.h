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
    PACKET_SKILL m_PacketSkill;
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
    UINT m_iLayerIdx;
    UINT m_iDamage;
    SKILL* m_pSkill;
    CGameObject* m_pPlayer;
    bool m_isMain;

    CGameObject* m_pTrail;
    float m_fAngle;
    float m_fRotateAngle;
    Vec3 m_vXZDir;
    Vec3 m_vQtrnRotAxis;
    Quaternion m_qRot;

    Vec3 m_LerpPos;
    Vec4 m_LerpQut;

    CAMP_STATE m_eCamp;
   
public:
    virtual void Awake();
    virtual void Update();

    void SetQtrnRotAxis(Vec3 _Axis) { m_vQtrnRotAxis = _Axis; }
    void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
    bool bSetSkill() { if (nullptr == m_pSkill)return true; else return false; }
    void SetSkill(SKILL* _pSkill);
    void SetLayerIdx(UINT _Idx) { m_iLayerIdx = _Idx; }
    UINT GetLayerIdx() { return m_iLayerIdx; }
    void SkillCheck();

    PACKET_SKILL GetPacketSkill() { return m_PacketSkill; }
    void SetPacketSkill(PACKET_SKILL paket_skill) {  m_PacketSkill = paket_skill; }

    void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
    void SetCamp(CAMP_STATE camp) { m_eCamp = camp; }
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

    void EnterSkill(Vec3 vPos);

    void SetisMain(bool isMain) { m_isMain = isMain; }

    virtual void OnCollision3DEnter(CCollider3D* _pColldier);

    void LerpUpdate();
    void SetLerp(Vec3 LocalPos, Vec4 Quaternion) { m_LerpPos = LocalPos; m_LerpQut = Quaternion; }
    
    void StopTrail();
    void StartTrail();

    void Collision();
    
    //½ºÅ³
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