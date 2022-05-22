#pragma once
#include "Component.h"
class CSkill :
    public CComponent
{

private:
public:
    virtual void Update();
    void Start();
    CSkill();
    virtual ~CSkill();



    CLONE(CSkill)


};

