#pragma once
#include "Script.h"
class CProjectileScript :
    public CScript
{
private:
    float m_fSpeed;
    UINT m_uiDamage;
    CGameObject* m_pObject;
    
public:
    bool m_bTest=false;

    void SetObject( CGameObject* _pGameObject) { m_pObject = _pGameObject; }
    virtual void Update();
    virtual void FinalUpdate();
    void SetSpeed(const float& _Speed) { m_fSpeed = _Speed; }
    void SetDamage(const UINT& _Damage) { m_uiDamage = _Damage; }




    virtual void OnCollision3DEnter(CCollider3D* _pOther);


    CProjectileScript();
    virtual ~CProjectileScript();
    CLONE(CProjectileScript);
};

