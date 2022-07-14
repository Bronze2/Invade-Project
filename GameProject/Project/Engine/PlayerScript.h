#pragma once
#include "Script.h"
#include "Animation3D.h"


class CTexture;
class CPlayerScript :
    public CScript
{
private:
    vector<CGameObject* >m_arrAlliance;
    bool m_bCheckStartMousePoint;
    Vec3 m_vZoomPos;
    Vec3 m_vRestorePos;
    CGameObject* m_pArrow[20];
    UINT m_iCurArrow;
    Ptr<CTexture>pBlackTex;
    int m_iPower;
    ELEMENT_TYPE m_iType;
    float m_fArrowSpeed;
    Vec3 m_vRestoreRot;
    float m_fArcherLocation;

    tAnimation* m_pCurAnimation;
    PLAYER_STATE m_eState;
    PLAYER_STATE m_ePrevState;

    tMTAnimClip* m_pCurAnimClip;
    tMTAnimClip* m_pNextAnimClip;

    Vec3 m_LerpPos;
    Vec3 m_PrevLerpPos;
    bool m_bColCheck;
    bool m_bMoveCheck;
    bool isMain;

    bool m_bCheckDegree;
    float m_fRotateDegree;
    float m_fMoveSpeed;
    float m_fTurnDegree;
    float m_fFactor;
    bool m_bTurn;
    int m_id;
    vector<CGameObject*>m_arrColObject;

    Vec3 m_LerpRot;


    CGameObject* m_pHealParticle;
    CGameObject* m_pFlameParticle;
    CGameObject* m_pThunderParticle;
    bool m_bHealCheck;
    bool m_bFlameCheck;
    bool m_bThunderCheck;
    bool m_bDarkCheck;

    SKILL* m_tESkill;
    SKILL* m_tZSkill;
    SKILL* m_currentSkill = nullptr;

    CGameObject* m_pESkillObject;
    CGameObject* m_pZSkillObject;

    vector<SKILL*> m_arrSkill;

    CGameObject* m_pHelmetObject;

    
    float m_fCurDegree;
    int m_iKeyHoldCnt;
    UINT m_iMaxHp;
    int m_iCurHp = 1500;
    CAMP_STATE m_eCamp;
    Vec3 restorePos;
    CGameObject* m_pBowObject;
    bool m_bReady;
public:

    void AttachHelmet();
    void m_FAnimation();
    void Init();
    virtual void Awake();
    virtual void Update();
    void SetType(ELEMENT_TYPE _iType);
    void SetMain() { isMain = true; }
    void SetState(int state);
    void m_SetId(int id) { m_id = id; }
    int m_GetId() { return m_id; }
    void Update_LerpPos();
    void SetLerpPos(Vec3 Pos) { m_PrevLerpPos = m_LerpPos; m_LerpPos = Pos; }
    Vec3 GetLerpPos() { return m_LerpPos; }
    void SetPrevLerpPos(Vec3 Pos) { m_PrevLerpPos = Pos; }
    void SetLerpRot(Vec3 Rot) { m_LerpRot = Rot; }
    CPlayerScript();
    virtual ~CPlayerScript();
    void GetDamage(const UINT& _uiDamage);
    void SetDamage(const int& _Damage);
    void SetCamp(CAMP_STATE camp) { m_eCamp = camp; }
    CAMP_STATE GetCamp() { return m_eCamp; }
    bool MoveCheck(const Vec3& _vPos);

    UINT GetType() { return (UINT)m_iType; }

    void m_FColCheck(Vec3 _vBeforePos, Vec3 _vAfterPos);
    bool GetCollCheck(){return m_bColCheck;}
    void SetCollCheckFalse() { m_bColCheck = false; }
    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);


    virtual void OnDetectionEnter(CGameObject* _pOther);
    virtual void OnDetection(CGameObject* _pOther);
    virtual void OnDetectionExit(CGameObject* _pOther);


    void SetBowObject(CGameObject* _pObj) { m_pBowObject = _pObj; }

    void SkillCoolTimeCheck();

    void StatusCheck();

    void UseSkill();
    void DamageBySkill(SKILL* _pSkill);
    void GetDamage();
    SKILL* GetSkill_E() { return m_tESkill; }
    SKILL* GetSkill_Z() { return m_tZSkill; }
    SKILL* GetCurrentSkill() { return m_currentSkill; }

    //Lobby
    void SetReady(bool _bTrue) { m_bReady = _bTrue; }
    bool GetReady() { return m_bReady; }
    void SetAnimationState(PLAYER_STATE _eState) { m_eState = _eState; }
    PLAYER_STATE GetAnimaionState() { return m_eState; }


    CLONE(CPlayerScript);
};

