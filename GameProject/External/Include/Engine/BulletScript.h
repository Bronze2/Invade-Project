#pragma once
#include "Script.h"
class CBulletScript :
    public CScript
{
public:
    virtual void Update();
    virtual void OnCollisionEnter(CCollider2D* _pOther);

public:
    CLONE(CBulletScript);

    CBulletScript();
    virtual ~CBulletScript();
};

