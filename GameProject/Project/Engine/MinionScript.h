#pragma once
#include "Script.h"
#include "Animation3D.h"
enum class ATTACKTYPE {
    MELEE,
    RANGE,
    CANON,
};

class CMinionScript :
    public CScript
{
    MINION_STATE m_eState;
    float m_fSpeed;
    float m_fRange;
    MINION_CAMP m_eCamp;
    tAnimation* m_CurAnimation;
    CGameObject* m_pTarget;
    CGameObject* m_pNexus;
    float m_fFindRange;
    float m_fAttackRange;
    MINION_STATE m_ePrevState;
    bool m_bFindNear;

    vector<CGameObject*> m_arrEnemy;
    vector<CGameObject*> m_pEnemyPlayers;

    ATTACKTYPE m_eAttackType;


public:
    void SetAttackType(ATTACKTYPE _eType) { m_eAttackType = _eType; }
    CLONE(CMinionScript)
        void Init();

    void SetNexus(CGameObject* _pNexus) { m_pNexus = _pNexus; }
    virtual void Update();
    void SetState(const MINION_STATE& _eState) { m_eState = _eState; }
    const MINION_STATE& GetState()const { return m_eState; }
    const MINION_CAMP& GetCamp()const { return m_eCamp; }


    virtual void OnDetectionEnter(CGameObject* _pOther);
    virtual void OnDetection(CGameObject* _pOther);
    virtual void OnDetectionExit(CGameObject* _pOther);

    void AddObject(CGameObject* _pObject);
    void PushPlayer(CGameObject* _pObject) { m_pEnemyPlayers.push_back(_pObject); }




    void FindNearObject(const vector<CGameObject*>& _pObject);



    void m_FAnimation();
    void m_FFind();



    CMinionScript();
    CMinionScript(float _fSpeed, float _fRange, MINION_STATE _eState, MINION_CAMP _eCamp);
    virtual ~CMinionScript();
};
