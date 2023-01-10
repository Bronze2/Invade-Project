#pragma once
#include "Script.h"
class CSafeZone :
    public CScript
{

private:

    CAMP_STATE m_eState;
public:
    CSafeZone();
    virtual ~CSafeZone();


    void SetCampState(const CAMP_STATE& _eState) { m_eState = _eState; }
    void Update();
    void Collision(const Vec3& _vPos);

    CLONE(CSafeZone);
};
