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
    CGameObject* m_pArrow[20];
    UINT m_iCurArrow;
    Ptr<CTexture>pBlackTex;
    int m_iPower;
    ELEMENT_TYPE m_iType;
    float m_fArrowSpeed;

    float m_fArcherLocation;

    float m_fRotateDegree;
    float m_fMoveSpeed;

    tMTAnimClip* m_pCurAnimClip;
    tMTAnimClip* m_pNextAnimClip;

    PLAYER_STATE m_ePrevState;
    PLAYER_STATE m_eState;
    
    UINT m_iMaxHp;
    float m_iCurHp;
    CGameObject* m_pHealParticle;

    bool m_bHealCheck;

#ifdef CAMERA_TEST
    float m_fTurnDegree;
    float m_fTurnSpeed;
    bool m_bTurnBack;
#endif

    float m_fLerpTime;
    float m_fMaxLerpTime;

    bool m_bCheckDegree;
    bool m_bColCheck;
    bool m_bMoveCheck;
    vector<CGameObject*>m_arrColObject;

    vector<CGameObject*> m_arrEnemy;

    SKILL* m_tESkill;
    SKILL* m_tZSkill;

    CGameObject* m_pESkillObject;
    CGameObject* m_pZSkillObject;

    vector<SKILL*> m_arrSkill;
  

public:
    void m_FAnimation();

    void Init();
    virtual void Awake();
    virtual void Update();
    void SetType(ELEMENT_TYPE _iType);
  
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

    void SetDamage(int _Damage); 


    CLONE(CPlayerScript);
};

