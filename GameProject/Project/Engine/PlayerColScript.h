#pragma once
#include "Script.h"
class CPlayerColScript :
    public CScript
{
private:
    Vec3 m_vCalPos;
    Vec3 m_vCalRot;
    CGameObject* m_pPlayer;
public:


    virtual void Update();
    CPlayerColScript();
    virtual ~CPlayerColScript();
    void SetPlayer(CGameObject* _pObj) { m_pPlayer = _pObj; }
    CGameObject* GetPlayer() { return m_pPlayer; }
    void SetPos(const Vec3& _Pos) { m_vCalPos = _Pos; }
    void SetRot(const Vec3& _Rot) { m_vCalRot = _Rot; }
    virtual void OnCollision3DEnter(CCollider3D* _pOther);
    virtual void OnCollision3D(CCollider3D* _pOther);
    virtual void OnCollision3DExit(CCollider3D* _pOther);

    CLONE(CPlayerColScript)
};

