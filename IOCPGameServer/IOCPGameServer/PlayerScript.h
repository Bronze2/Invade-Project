#pragma once
#include "pch.h"
#include "Script.h"
class CPlayerScript :
    public CScript
{
private:
    Vec3 m_LerpPos;

    bool isMain;
    int m_id;
    CGameObject* m_pArrow[20];

public:
    void Init();
    virtual void Awake();
    virtual void Update();

    CPlayerScript();
    virtual ~CPlayerScript();



    void m_SetId(int id) { m_id = id; };
    int m_GetId() { return m_id; };

    CLONE(CPlayerScript);
};

