#pragma once
#include "Script.h"
#include "Animation3D.h"

enum class MINION_ATTACK_TYPE {
    MELEE,//50
    RANGE,//150
    CANON//200
};


enum class FIND_STATE {
    NONE,
    SENSOR_FIRST,
    RAY_FIRST,
    END,
};
class CMinionScript :
    public CScript
{
    MINION_STATE m_eState;
    FIND_STATE m_eFindState;
    float m_fSpeed;
    float m_fRange;
    CAMP_STATE m_eCamp;
    tAnimation* m_CurAnimation;
    tMTAnimClip* m_pCurAnimClip;
    tMTAnimClip* m_pNextAnimClip;
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


    CGameObject* m_pProjectile;

    bool m_bRotate;

    UINT m_iAllienceCol;
    UINT m_uiMaxHp;
    int m_iCurHp;
    UINT m_uiAttackDamage;

    MINION_ATTACK_TYPE m_eAttackType;
    bool m_bSeparate;
    bool m_bProjectile;
    wstring m_CampName;
  //  CGameObject* m_pProjectile;
    CGameObject* m_InterSectObject;
    Ptr<CSound> m_pMeleeSound;
    Ptr<CSound> m_pRangeSound;
    SimpleMath::Ray* m_pRay;
    SimpleMath::Ray* GetRay() { return m_pRay; }
   


    //Ray
    float m_fStartXValue;
    float m_fMinStartX;
    float m_fMaxStartX;
    int m_bDetection;
    float m_fPrevXValue;


    CGameObject* m_pFirstTower;
    CGameObject* m_pSecondTower;




public:

    void SetFirstTower(CGameObject* _pObject) { m_pFirstTower = _pObject; }
    void SetSecondTower(CGameObject* _pObject) { m_pSecondTower = _pObject; }
 
 
    void CheckHp();
    CLONE(CMinionScript)
    void Init();
    void InterSectsObject(CCollider3D* _pCollider);

    void CheckObstacle();
    void RayCollision(const CLayer* _pLayer);
    void SetCamp(CAMP_STATE _eCamp) { m_eCamp = _eCamp;
    if (m_eCamp == CAMP_STATE::RED)
        m_CampName = L"Red";

    if (m_eCamp == CAMP_STATE::BLUE)
        m_CampName = L"Blue";
    }
    wstring GetCampName() { return m_CampName; }
    void GetDamage(const UINT& _uiDamage) { m_iCurHp -= _uiDamage; }
   
    void SetNexus(CGameObject* _pNexus) { m_pNexus = _pNexus; }
    virtual void Update();
    void SetState(const MINION_STATE& _eState) { m_eState = _eState; }
    const MINION_STATE& GetState()const { return m_eState; }
    const CAMP_STATE& GetCamp()const { return m_eCamp; }

    void SetSeparate(bool _bSeparate) { m_bSeparate = _bSeparate; }

    void CreateProjectile(const wstring& _Key, const UINT& _Bone, const wstring& _Layer);
    
    virtual void OnDetectionEnter(CGameObject* _pOther);
    virtual void OnDetection(CGameObject* _pOther);
    virtual void OnDetectionExit(CGameObject* _pOther);

    void AddObject(CGameObject* _pObject);
    void PushPlayer(CGameObject* _pObject) { m_pEnemyPlayers.push_back(_pObject); }
   
    
    void SetAttackRange(float _fRange) { m_fAttackRange = _fRange; }

    void SetAttackType(MINION_ATTACK_TYPE _eType) { m_eAttackType = _eType; }
    float GetRange() { return m_fAttackRange; }

    void CheckRange();

    void SetDamage(const int& _Damage);
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

