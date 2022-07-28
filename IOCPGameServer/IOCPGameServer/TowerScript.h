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
    CAMP_STATE  m_eCampState;

    UINT m_uiDef;
    UINT m_uiAttackDamage = 50;
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


public:
    void SetCamp(CAMP_STATE _eCamp) { m_eCampState = _eCamp; }
    const CAMP_STATE& GetCamp()const { return m_eCampState; }

    void Init();
    void SetCampState(CAMP_STATE _eCampState) {
        m_eCampState = _eCampState;
    }
    void SetSecondTower(CGameObject* _pGameObject);
    void SetFirstTower(CGameObject* _pGameObject);
    void SetNexus(CGameObject* _pObject);
    void m_FAttack();
    void m_FRotate();

    void SetType(TOWER_TYPE _eType) { m_eType = _eType; }
    virtual void Update();
    virtual void FinalUpdate();

    void m_SetId(int id) { m_id = id; }
    int  m_GetId() { return m_id; }
    void GetDamage(const UINT& _uiDamage);

    virtual void OnDetectionEnter(CGameObject* _pOther);
    virtual void OnDetection(CGameObject* _pOther);
    virtual void OnDetectionExit(CGameObject* _pOther);
    CTowerScript();
    virtual ~CTowerScript();

    void FindNearObject(const vector<CGameObject*>& _pObject);
    void CreateProjectile(const wstring& _Layer);

    CLONE(CTowerScript);
};
