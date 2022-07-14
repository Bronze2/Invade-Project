#pragma once
#include "Script.h"

#define ATTACK_INTERVAL 2
enum class TOWER_TYPE {
    FIRST,
    SECOND,
    NEXUS,
};
class CTowerScript :
    public CScript
{
private:
    UINT m_iMaxHp;
    int m_iCurHp;
    float m_fAttackRange;

   Ptr<CSound> m_pSound;
    UINT m_uiDef;

    TOWER_TYPE m_eType;
    CAMP_STATE m_eCampState;
    CGameObject* m_pNexus;
    CGameObject* m_pFirstTower;
    CGameObject* m_pSecondTower;

    clock_t m_cAttackStart;
    clock_t m_cAttackEnd;
    clock_t m_cAttackInterval;
    bool m_bAttackStart;
    vector<CGameObject*> m_arrEnemy;
    CGameObject* m_pTarget;
    bool m_bFindNear;
    CGameObject* m_pObject;
    
    float m_fProjectileStart;
    void CreateProjectile(const wstring& _Key, const wstring& _Layer);
    UINT m_uiAttackDamage;

    wstring m_CampName;
    bool m_bDeadTrue = false;

public:
    bool GetDeadCheck() { return m_bDeadTrue; }
    void SetCampState(CAMP_STATE _eCampState) { 
        m_eCampState = _eCampState;
        if (m_eCampState == CAMP_STATE::RED)
            m_CampName = L"Red";

        if (m_eCampState == CAMP_STATE::BLUE)
            m_CampName = L"Blue";
    }
    wstring GetCampName() { return m_CampName; }

    CAMP_STATE GetCamp() { return m_eCampState; }
    bool m_bTest;
    void Init();

    void SetSecondTower(CGameObject* _pGameObject);
    void SetFirstTower(CGameObject* _pGameObject);
    void SetNexus(CGameObject* _pObject);
    

    void m_FAttack();
    void m_FRotate();

    void SetType(TOWER_TYPE _eType) { m_eType = _eType; }
    virtual void Update();
    virtual void FinalUpdate();

    void GetDamage(const UINT& _Dmg);
   

    virtual void OnDetectionEnter(CGameObject* _pOther);
    virtual void OnDetection(CGameObject* _pOther);
    virtual void OnDetectionExit(CGameObject* _pOther);
    CTowerScript();
    virtual ~CTowerScript();

    void FindNearObject(const vector<CGameObject*>& _pObject);

    CLONE(CTowerScript);
};

