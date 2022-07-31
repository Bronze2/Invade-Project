#pragma once
#include "Script.h"
class ObstacleScript :
    public CScript
{

    virtual void Update() {};



    ObstacleScript();
    virtual ~ObstacleScript();
    CLONE(ObstacleScript);
};

