#pragma once
#include "Script.h"
#include "Animation3D.h"

enum class MINION_ATTACK_TYPE {
    MELEE,//50
    RANGE,//150
    CANON//200
};

class CMinionScript :
    public CScript
{
    MINION_STATE m_eState;
    float m_fSpeed;
    float m_fRange;
    CAMP_STATE m_eCamp;
    tAnimation* m_CurAnimation;
    CGameObject* m_pTarget;
    CGameObject* m_pNexus;
    float m_fFindRange;
    float m_fAttackRange;
    MINION_STATE m_ePrevState;
    bool m_bFindNear;

    vector<CGameObject*> m_arrEnemy;
    vector<CGameObject*> m_pEnemyPlayers;
    
    tMTAnimClip* m_pCurAnimClip;
    tMTAnimClip* m_pNextAnimClip;

    bool m_bFinishAnimation;
    bool m_bAllienceCol;


    bool m_bRotate;

    UINT m_iAllienceCol;
    UINT m_uiMaxHp;
    int m_iCurHp;
    UINT m_uiAttackDamage;

    MINION_ATTACK_TYPE m_eAttackType;
    bool m_bSeparate;
public:
    void CheckHp();
    CLONE(CMinionScript)
    void Init();

    void SetCamp(CAMP_STATE _eCamp) { m_eCamp = _eCamp; }
    
    void GetDamage(const UINT& _uiDamage) { m_iCurHp -= _uiDamage; }
   
    void SetNexus(CGameObject* _pNexus) { m_pNexus = _pNexus; }
    virtual void Update();
    void SetState(const MINION_STATE& _eState) { m_eState = _eState; }
    const MINION_STATE& GetState()const { return m_eState; }
    const CAMP_STATE& GetCamp()const { return m_eCamp; }

    void SetSeparate(bool _bSeparate) { m_bSeparate = _bSeparate; }

    
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
    
    
    
    void m_FAnimation();
    void m_FFind();


   
    CMinionScript();
    CMinionScript(float _fSpeed,float _fRange,MINION_STATE _eState, CAMP_STATE _eCamp);
    virtual ~CMinionScript();




    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);
};

