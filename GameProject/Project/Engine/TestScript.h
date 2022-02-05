#pragma once
#include "Script.h"
class CTestScript :
    public CScript
{
private:
    int m_iLifeCount;
public:
    virtual void Update();
    virtual void OnCollisionEnter(CCollider2D* _pOther);
    virtual void OnCollisionExit(CCollider2D* _pOther);

    CTestScript();
    virtual ~CTestScript();

    CLONE(CTestScript);
};

