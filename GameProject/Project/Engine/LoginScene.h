#pragma once
#include "Scene.h"
class CLoginScene :
    public CScene
{
private:
    UINT m_uiLoginAttempt;
    char user_name[255] = "Enter ID";
    char password[255] = "Enter Password";


public:
    virtual void Update();
    virtual void Awake();
    virtual void Init();
};

