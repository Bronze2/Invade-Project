#pragma once
#include "Scene.h"
class CLoginScene :
    public CScene
{
private:
    UINT m_uiLoginAttempt;
    wstring m_ID;
    wstring m_Password;

public:
    virtual void Update();
    virtual void Awake();
};

