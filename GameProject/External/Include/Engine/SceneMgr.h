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
	
	
	void net_enterClient(int id, float x, float y, float z);
	void net_setMainClient(int id, float x, float y, float z);
	void net_setEnableClient(int id);	//지울가능성 높음
	void net_setLocalPosByID(int id, float x, float y , float z);
	void net_setRotationByID(int id, float x, float y, float z);
	void net_setAnimationByID(int id , MINION_STATE);

	CScene* GetCurScene();
	void ChangeScene(CScene* _pNextScene);
	void FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj);
};

