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
	vector<CGameObject*> m_player;

public:
	void Init();
	void Update();
	void Update_Tool();
	void EnterClient(int id, float x, float y, float z);
	void setMainClient(int id, float x, float y, float z);
	void setEnableClient(int id);

	CScene* GetCurScene();
	void ChangeScene(CScene* _pNextScene);
	void FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj);
	void net_setLocalPosByID(int id, float x, float y , float z);
};

