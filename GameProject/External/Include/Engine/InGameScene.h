#pragma once
#include "Scene.h"
class CInGameScene :
    public CScene
{
private:
    CSound* m_pSceneBGM;
public:
    virtual void Init();
    ~CInGameScene();
};

