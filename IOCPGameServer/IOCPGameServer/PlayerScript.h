#pragma once
#include "pch.h"
#include "Script.h"
class CPlayerScript :
    public CScript
{
private:
    Vec3 m_LerpPos;

    bool isMain;

public:
    void Init();
    virtual void Awake();
    virtual void Update();

    CPlayerScript();
    virtual ~CPlayerScript();



    CLONE(CPlayerScript);
};

