#pragma once
#include "Script.h"

#define PATTERNINTEVAL 10
#define SPAWNINTEVAL 1
class CAnimation;
class CSpawnScript :
    public CScript
{
private:
    clock_t m_uiPatternStart;// pattern
    clock_t m_uiPatternEnd;
    clock_t m_uiPatternInterval;// ���� ���� ����
    bool m_bClockStart;
    bool m_bSpawnStart;

    bool m_bPatternOn;

    UINT m_uiCount; 
    clock_t m_uiSpawnStart;
    clock_t m_uiSpawnEnd;
    clock_t m_uiSpawnInterval;

    SPAWN_PATTERN m_eSpawnPattern;
    SPAWN_COUNT m_eSpawnCount;
    
    CAMP_STATE m_eCampState;
private:
    CGameObject* SpawnObject(const wstring& _strKey, Vec3 _vLocalPos, Vec3 _vLocalScale, Vec3 _vOffsetScale, CAnimation* _pAnimation);
public:

    void SetSpawnState(CAMP_STATE _eState) { m_eCampState = _eState; }
    virtual void Update();

    CSpawnScript();
    virtual ~CSpawnScript();

    CLONE(CSpawnScript);
};

