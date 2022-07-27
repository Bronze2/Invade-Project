#pragma once
#include "Script.h"
#include "Animation3D.h"

class CTexture;

#define DEADTIME 15

class CPlayerScript :
    public CScript
{
private:
    vector<CGameObject* >m_arrAlliance;
    bool m_bCheckStartMousePoint;
    Vec3 m_vZoomPos;
    Vec3 m_vRestorePos;
    Vec3 m_vRestoreRot;

    ELEMENT_TYPE m_iType;

    float m_fRotateDegree;
    float m_fMoveSpeed;
    float m_fTurnDegree;
    float m_fCurDegree;
    float m_fFactor;
    
    tMTAnimClip* m_pCurAnimClip;
    tMTAnimClip* m_pNextAnimClip;

    PLAYER_STATE m_ePrevState;
    PLAYER_STATE m_eState;
    
    UINT m_iMaxHp;
    UINT m_iCurHp;
    CGameObject* m_pHealParticle;
    CGameObject* m_pFlameParticle;
    CGameObject* m_pThunderParticle;
    // DarkUI
    CGameObject* m_pDarkUI;

    // Run
    bool m_bRun = false;

    bool m_bHealCheck;
    bool m_bFlameCheck;
    bool m_bThunderCheck;
    bool m_bDarkCheck;

    float m_fLerpTime;
    float m_fMaxLerpTime;

    bool m_bCheckDegree;
    bool m_bColCheck;
    bool m_bMoveCheck;
   

    bool m_bTurn;
    int m_iKeyHoldCnt;

    vector<CGameObject*>m_arrColObject;

    vector<CGameObject*> m_arrEnemy;

    SKILL* m_tESkill;
    SKILL* m_tZSkill;

    CGameObject* m_pESkillObject;
    CGameObject* m_pZSkillObject;

    vector<SKILL*> m_arrSkill;

    CGameObject* m_pBowObject;

    // 헬멧 플레이어 고개 까딱
    CGameObject* m_pHelmetObject;

    // 에이치피바
    CGameObject* m_pHPBar;
    vector<CGameObject*> m_vecUIHpBar;

    // 사망
    CGameObject* m_pDeadEffect;
    bool m_bDead;
    clock_t m_uiDeadStart;
    clock_t m_uiDeadEnd;
    clock_t m_uiDeadInterval;

    // 로비씬
    bool m_bReady;
public:
    void m_FAnimation();

    void SetBowObject(CGameObject* _pObj) { m_pBowObject = _pObj; }
    void Init();
    virtual void Awake();
    virtual void Update();
    void SetType(ELEMENT_TYPE _iType);
    UINT GetType() { return (UINT)m_iType; }
  
    void SkillCoolTimeCheck();

    void StatusCheck();

    void UseSkill();
    void DamageBySkill(SKILL* _pSkill);
    void GetDamage();

    void AttachHelmet();

    void SetReady(bool _bTrue) { m_bReady = _bTrue; }
    bool GetReady() { return m_bReady; }
    void SetAnimationState(PLAYER_STATE _eState) { m_eState = _eState; }
    PLAYER_STATE GetAnimaionState() { return m_eState; }

    // 결과씬
    void SetResultAnim(bool _bWin);

    // 에이치피바
    UINT GetCurHp() { return m_iCurHp; }
    void SetCurHp(int _hp) { m_iCurHp = _hp; }
    void m_FColCheck(Vec3 _vBeforePos,Vec3 _vAfterPos);
    void AddUIHpBarObj(CGameObject* _pObj) { m_vecUIHpBar.emplace_back(_pObj); }
   
    // 사망처리
    void CheckHpAndDead();

    // 스킬유아이
    SKILL* GetESkill() { return m_tESkill; }
    SKILL* GetZSkill() { return m_tZSkill; }
    void SetESkillObj(CGameObject* _pObj) { m_pESkillObject = _pObj; }
    void SetZSkillObj(CGameObject* _pObj) { m_pZSkillObject = _pObj; }

    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);

    virtual void OnDetectionEnter(CGameObject* _pOther);
    virtual void OnDetection(CGameObject* _pOther);
    virtual void OnDetectionExit(CGameObject* _pOther);

    CPlayerScript();
    virtual ~CPlayerScript();

    void SetDamage(const int& _Damage);


    CLONE(CPlayerScript);
};

