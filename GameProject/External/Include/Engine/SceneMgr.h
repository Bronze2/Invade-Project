#pragma once
//#include "Network.h"
class CScene;
class CCamera;
class CGameObject;

enum class SCENE_TYPE {
	TITLE,
	LOBBY,
	INGAME,
	RESULT,
	END,
};
class CSceneMgr
{
	SINGLE(CSceneMgr)
private:
	CScene* m_pCurScene;
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];
	vector<CGameObject*> m_player;

public:
	void Init();
	void Update();
	void Update_Tool();
	void Init(SCENE_TYPE _eType);
	
	void net_enterClient(int id, float x, float y, float z);
	void net_setMainClient(int id, float x, float y, float z);
	void net_setEnableClient(int id);	//지울가능성 높음
	void net_setLocalPosByID(int id, float x, float y , float z);
	void net_setRotationByID(int id, float x, float y, float z);
	void net_setAnimationByID(int id , int state);
	void net_spawnMinion(int id);

	CScene* GetCurScene();
	void ChangeScene(CScene* _pNextScene);
	void ChangeScene(SCENE_TYPE _Type);
	void FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj);
};

