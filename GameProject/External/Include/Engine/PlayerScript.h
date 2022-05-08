#pragma once
#include "Script.h"
#include "Animation3D.h"


class CTexture;
class CPlayerScript :
    public CScript
{
private:
    bool m_bCheckStartMousePoint;
    Vec3 m_vZoomPos;
    Vec3 m_vRestorePos;
    CGameObject* m_pArrow[20];
    UINT m_iCurArrow;
    Ptr<CTexture>pBlackTex;
    int m_iPower;
    ELEMENT_TYPE m_iType;
    float m_fArrowSpeed;
    Vec3 m_vRestoreRot;
    float m_fArcherLocation;

    tAnimation* m_pCurAnimation;
    PLAYER_STATE m_eState;
    PLAYER_STATE m_ePrevState;

    tMTAnimClip* m_pCurAnimClip;
    tMTAnimClip* m_pNextAnimClip;

    Vec3 m_LerpPos;
    Vec3 m_PrevLerpPos;
    bool m_bColCheck;
    bool m_bMoveCheck;
    bool isMain;

    bool m_bCheckDegree;
    float m_fRotateDegree;
    float m_fMoveSpeed;
    float m_fTurnDegree;
    float m_fFactor;
    bool m_bTurn;
    int m_id;
    vector<CGameObject*>m_arrColObject;

    Vec3 m_LerpRot;

    float m_fCurDegree;
    int m_iKeyHoldCnt;
    UINT m_uiMaxHp;
    int m_iCurHp = 1500;
    CAMP_STATE m_eCamp;
    Vec3 restorePos;
public:
    void m_FAnimation();
    void Init();
    virtual void Awake();
    virtual void Update();
    void SetType(ELEMENT_TYPE _iType) { m_iType = _iType; }
    void SetMain() { isMain = true; }
    void SetState(int state);
    void m_SetId(int id) { m_id = id; }
    int m_GetId() { return m_id; }
    void Update_LerpPos();
    void SetLerpPos(Vec3 Pos) { m_PrevLerpPos = m_LerpPos; m_LerpPos = Pos; }
    Vec3 GetLerpPos() { return m_LerpPos; }
    void SetPrevLerpPos(Vec3 Pos) { m_PrevLerpPos = Pos; }
    void SetLerpRot(Vec3 Rot) { m_LerpRot = Rot; }
    CPlayerScript();
    virtual ~CPlayerScript();
    void GetDamage(const UINT& _uiDamage);

    void m_FColCheck(Vec3 _vBeforePos, Vec3 _vAfterPos);
    bool GetCollCheck(){return m_bColCheck;}
    void SetCollCheckFalse() { m_bColCheck = false; }
    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);


    CLONE(CPlayerScript);
};

