#pragma once
#include "Network.h"
class CScene;
class CCamera;
class CGameObject;

class CSceneMgr
{
	SINGLE(CSceneMgr)
private:
	CScene* m_pCurScene;

public:
	void Init();
	void Update();
	void Update_Tool();
	void Enter_Client(float x, float y, float z);
	void Enter_Main_Client(float x, float y, float z);

	CScene* GetCurScene();
	void ChangeScene(CScene* _pNextScene);
	void FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj);
};

