#pragma once
#include "pch.h"
#include "Script.h"
class CPlayerScript :
    public CScript
{
private:
    Vec3 m_LerpPos;

    bool isMain;
    int m_id;
    CGameObject* m_pArrow[20];
    UINT m_uiMaxHp;
    int m_iCurHp;
    CAMP_STATE m_eCamp;


public:
    void Init();
    virtual void Awake();
    virtual void Update();

    CPlayerScript();
    virtual ~CPlayerScript();

    void InitArrow(int ArrowId, Vec3 Pos, Vec3 Rot, Vec3 Dir, float Power, PACKET_SKILL skill);

    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    void GetDamage(const UINT& _uiDamage);

    void SetCamp(CAMP_STATE _eCamp) { m_eCamp = _eCamp; }
    const CAMP_STATE& GetCamp()const { return m_eCamp; }

    void m_SetId(int id) { m_id = id; };
    int m_GetId() { return m_id; };

    CLONE(CPlayerScript);
};

