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
    float m_iCurHp;
    CGameObject* m_pHealParticle;
    CGameObject* m_pFlameParticle;
    CGameObject* m_pThunderParticle;
    CGameObject* m_pBuffParticle;

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
   
    std::chrono::system_clock::time_point m_tBuffTime;



    CGameObject* m_pBowObject;

    CSound* m_pHitSound;
    CSound* m_pDeadSound;

    CSound* m_pErrorSound;

    //�߰�
    CAMP_STATE m_eCampState;
    CGameObject* pDeadEffect;
    bool m_bCanAttack;
    bool m_bColBox;

    UINT m_uiStrength;
    CGameObject* m_pColPlayer;
public:
    void SetStrength(const UINT& _Dmg) { m_uiStrength = _Dmg; }
    const UINT& GetStrength() { return m_uiStrength; }
    void GetBuff();
    void SetCanAttack(const bool& _bCanAttack) { m_bCanAttack = _bCanAttack; }
    CAMP_STATE GetCampState() { return m_eCampState; }
    SKILL* GetESkill() { return m_tESkill; }
    SKILL* GetZSkill() { return m_tZSkill; }
    void m_FAnimation();

    void SetESkillObj( CGameObject* _pObj) { m_pESkillObject = _pObj; }
    void SetZSkillObj( CGameObject* _pObj) { m_pZSkillObject = _pObj; }
    bool MoveCheck(const Vec3& _vPos);
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

    void SetCurHp(int _hp) { m_iCurHp = _hp; }
    void m_FColCheck(Vec3 _vBeforePos,Vec3 _vAfterPos);

    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);

    virtual void OnDetectionEnter(CGameObject* _pOther);
    virtual void OnDetection(CGameObject* _pOther);
    virtual void OnDetectionExit(CGameObject* _pOther);

    CPlayerScript();
    virtual ~CPlayerScript();

    void SetDamage(const int& _Damage);



    void SetColPlayer(CGameObject* _pObj) { m_pColPlayer = _pObj; }
    CGameObject* GetColObj() { return m_pColPlayer; }
    CLONE(CPlayerScript);
};

