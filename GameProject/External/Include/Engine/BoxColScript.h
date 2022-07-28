#pragma once
#include "Script.h"
class CBoxColScript :
    public CScript
{
private:
 
    CGameObject* m_pBox;
public:


    virtual void Update();
    CBoxColScript();
    virtual ~CBoxColScript();
    void SetBox(CGameObject* _pObj) { m_pBox = _pObj; }
    CGameObject* GetBox() { return m_pBox; }
    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);

    CLONE(CBoxColScript)
};

