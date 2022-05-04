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
	map<int, Vec3> m_tower;
	map<int, Vec3> m_projectile;


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
	void net_spawnMinion_blue(int id, int mtype,float x, float y, float z);
	void net_spawnMinion_red(int id, int mtype,float x, float y, float z);
	void net_moveMinion(int id, float x, float y, float z, 
							float r_x, float r_y, float r_z, int state);
	void net_animMinion(int id, float x, float y, float z,
		float r_x, float r_y, float r_z, int state);
	void net_setRotTower(int id, Vec3 rot);
	void net_spawnProjectile(int id, Vec3 pos);
	void net_moveProjectile(int id, Vec3 pos);

	
	
	
	
	Vec3 get_towerRot(int id) { return m_tower[id]; }
	void set_towerRot(int id, Vec3 rot) { m_tower[id] = rot; };
	Vec3 get_ProjectilePos(int id) { return m_projectile[id]; }



	minioninfo get_minioninfo(int id) { return m_minion[id]; };
	void set_minioninfoState(int id, MINION_STATE state) { m_minion[id].state = state; }
	CScene* GetCurScene();
	void ChangeScene(CScene* _pNextScene);
	void ChangeScene(SCENE_TYPE _Type);
	void EnterGame();
	void FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj);

	void net_lobby_enterClient(int id, CAMP_STATE, bool isHost);

};

