#pragma once
#include "Script.h"

#include <iostream>


class CMinionScript :
    public CScript
{
    MINION_STATE m_eState;
    float m_fSpeed;
    float m_fRange;
    CAMP_STATE m_eCamp;
    CGameObject* m_pTarget;
    CGameObject* m_pNexus;
    float m_fFindRange;
    float m_fAttackRange;
    MINION_STATE m_ePrevState;
    bool m_bFindNear;
    vector<CGameObject*> m_arrEnemy;
    vector<CGameObject*> m_pEnemyPlayers;

    bool m_bAllienceCol;
    bool m_bRotate;

    UINT m_iAllienceCol;
    UINT m_uiMaxHp;
    int m_iCurHp;
    UINT m_uiAttackDamage;

    MINION_ATTACK_TYPE m_eAttackType;
    bool m_bSeparate;
    int m_id;
<<<<<<< HEAD
    void CreateProjectile(const wstring& _Layer);
    bool m_bProjectile;
    CGameObject* m_pProjectile;
=======

    int attack_delay;
<<<<<<< HEAD
>>>>>>> parent of 1dcb62e (Minion_Attack_Pos)
=======
>>>>>>> parent of 1dcb62e (Minion_Attack_Pos)
public:
    void CheckHp();
    CLONE(CMinionScript)
    void Init();
    void SetCamp(CAMP_STATE _eCamp) { m_eCamp = _eCamp; }
    
    void GetDamage(const UINT& _uiDamage) { m_iCurHp -= _uiDamage; 
    std::cout << this->m_GetId() << " : �ĸ¾���!" << endl;
    }
   
    void SetNexus(CGameObject* _pNexus) { m_pNexus = _pNexus; }
    virtual void Update();
    void SetState(const MINION_STATE& _eState) { m_eState = _eState; }
    const MINION_STATE& GetState()const { return m_eState; }
    const CAMP_STATE& GetCamp()const { return m_eCamp; }

    void SetSeparate(bool _bSeparate) { m_bSeparate = _bSeparate; }
<<<<<<< HEAD
<<<<<<< HEAD
    //void SetAttackMaxTime(float time) { m_attack_max_time = time; }
=======

>>>>>>> parent of 1dcb62e (Minion_Attack_Pos)
=======

>>>>>>> parent of 1dcb62e (Minion_Attack_Pos)
    
    virtual void OnDetectionEnter(CGameObject* _pOther);
    virtual void OnDetection(CGameObject* _pOther);
    virtual void OnDetectionExit(CGameObject* _pOther);

    void AddObject(CGameObject* _pObject);
    void PushPlayer(CGameObject* _pObject) { m_pEnemyPlayers.push_back(_pObject); }
   
    
    void SetAttackRange(float _fRange) { m_fAttackRange = _fRange; }

    void SetAttackType(MINION_ATTACK_TYPE _eType) { m_eAttackType = _eType; }
    float GetRange() { return m_fAttackRange; }

    void CheckRange();


    void FindNearObject(const vector<CGameObject*>& _pObject);
    
    
    
    void m_FFind();
    
    void m_SetId(int id) { m_id = id; };
    int m_GetId() { return m_id; };
   
    CMinionScript();
    CMinionScript(float _fSpeed,float _fRange,MINION_STATE _eState, CAMP_STATE _eCamp);
    virtual ~CMinionScript();



    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);
};

