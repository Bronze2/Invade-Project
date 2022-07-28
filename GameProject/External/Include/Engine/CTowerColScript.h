#pragma once
#include "Script.h"
class CTowerColScript :
    public CScript
{
private:

    CGameObject* m_pTower;
public:


    virtual void Update();
    CTowerColScript();
    virtual ~CTowerColScript();
    void SetTower(CGameObject* _pObj) { m_pTower = _pObj; }
    CGameObject* GetTower() { return m_pTower; }
    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);

    CLONE(CTowerColScript)

};

