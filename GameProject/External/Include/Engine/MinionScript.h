#pragma once
#include "Script.h"
#include "Animation3D.h"

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
    
    bool m_bFinishAnimation;
    bool m_bAllienceCol;


    bool m_bRotate;

    UINT m_iAllienceCol;


private:
    void m_FColRotate();
public:
    
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
   
    
    void SetAttackRange(float _fRange) { m_fAttackRange = _fRange; }
    float GetRange() { return m_fAttackRange; }

    void CheckRange();


    void FindNearObject(const vector<CGameObject*>& _pObject);
    
    
    
    void m_FAnimation();
    void m_FFind();


   
    CMinionScript();
    CMinionScript(float _fSpeed,float _fRange,MINION_STATE _eState,MINION_CAMP _eCamp);
    virtual ~CMinionScript();




    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);
};

