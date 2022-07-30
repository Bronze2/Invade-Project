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
    int m_iMaxHp;
    int m_iCurHp;
    float m_fAttackRange;

    UINT m_uiDef;

    TOWER_TYPE m_eType;
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
    int m_id;

    CGameObject* m_pHPBar;
    CAMP_STATE m_eCampState;
    wstring m_CampName;
public:
    void SetHp(int hp) { m_iCurHp = hp; }

    void SetCampState(CAMP_STATE _eCampState) {
        m_eCampState = _eCampState;
        if (m_eCampState == CAMP_STATE::RED)
            m_CampName = L"Red";

        if (m_eCampState == CAMP_STATE::BLUE)
            m_CampName = L"Blue";
    }
    void Init();

    void SetSecondTower(CGameObject* _pGameObject);
    void SetFirstTower(CGameObject* _pGameObject);
    void SetNexus(CGameObject* _pObject);

    void m_FAttack();
    void m_FRotate();

    void SetType(TOWER_TYPE _eType) { m_eType = _eType; }
    virtual void Update();
    virtual void FinalUpdate();

   

    virtual void OnDetectionEnter(CGameObject* _pOther);
    virtual void OnDetection(CGameObject* _pOther);
    virtual void OnDetectionExit(CGameObject* _pOther);
    CTowerScript();
    virtual ~CTowerScript();

    void FindNearObject(const vector<CGameObject*>& _pObject);

    void m_SetId(int id) { m_id = id; }
    int  m_GetId() { return m_id; }

    CLONE(CTowerScript);
};

