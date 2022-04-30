#pragma once
#include "pch.h"
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

struct minioninfo {
	Vec3 pos;
	Vec3 rot;
	MINION_STATE state;
};

class CSceneMgr
{
	SINGLE(CSceneMgr)
private:
	CScene* m_pCurScene;
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];
	vector<CGameObject*> m_player;

	map<int, minioninfo> m_minion;

public:
	void Init();
	void Update();
	void Update_Tool();
	void Init(SCENE_TYPE _eType);
	
	
	void net_enterClient(int id, float x, float y, float z);
	void net_setMainClient(int id, float x, float y, float z);
	void net_setEnableClient(int id);	//지울가능성 높음
	void net_setLerpMoveByID(int id, float x, float y , float z);
	void net_setLocalPosByID(int id, float x, float y, float z);
	void net_setRotationByID(int id, float x, float y, float z);
	void net_setAnimationByID(int id , int state);
	void net_spawnMinion_blue(int id, float x, float y, float z);
	void net_spawnMinion_red(int id, float x, float y, float z);
	void net_moveMinion(int id, float x, float y, float z, int state);

	minioninfo get_minioninfo(int id) { return m_minion[id]; };

	CScene* GetCurScene();
	void ChangeScene(CScene* _pNextScene);
	void ChangeScene(SCENE_TYPE _Type);
	void EnterGame();
	void FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj);

	void net_lobby_enterClient(int id, CAMP_STATE, bool isHost);

};

