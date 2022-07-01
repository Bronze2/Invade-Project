#pragma once
//#include "pch.h"

//#include "Network.h"
class CScene;
class CCamera;
class CGameObject;

enum class SCENE_TYPE {
	TITLE,
	LOGIN,
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

struct arrowId {
	int p_id;
	int m_id;
};

struct arrowMove {
	Vec3 Pos;
	Vec3 Rot;
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
	map<int, bool> m_projectile_die;

	map<int, map<int,arrowMove>> m_arrow;

public:

	void Init();
	void Update();
	void Update_Tool();
	void Init(SCENE_TYPE _eType);
	
	CScene* GetCurScene();
	void ChangeScene(CScene* _pNextScene);
	void ChangeScene(SCENE_TYPE _Type);

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

	void net_initArrow(int parentid, int id, Vec3 pos, Vec3 rot);
	void net_moveArrow(int parentid,int id, Vec3 pos,Vec3 rot);
	void net_animUpdate(int id, int state);
	void net_deleteMinion(int id);
	void net_deleteArrow(int client_id, int arrow_id);
	void net_DamagedByArrow(int coll_type, int coll_id, int damage);
	void net_deletProjectile(int id);


	
	
	
	
	Vec3 get_arrowPos(int p_id, int m_id) { return m_arrow[p_id][m_id].Pos; }
	Vec3 get_arrowRot(int p_id, int m_id) { return m_arrow[p_id][m_id].Rot; }
	void set_arrowPos(int p_id, int m_id,Vec3 pos) {  m_arrow[p_id][m_id].Pos = pos; }
	void set_arrowRot(int p_id, int m_id,Vec3 rot) {  m_arrow[p_id][m_id].Rot = rot; }


	Vec3 get_towerRot(int id) { return m_tower[id]; }
	void set_towerRot(int id, Vec3 rot) { m_tower[id] = rot; };
	Vec3 get_ProjectilePos(int id) { return m_projectile[id]; }
	bool get_ProjectileDelete(int id) { return m_projectile_die[id]; }



	minioninfo get_minioninfo(int id) { return m_minion[id]; };
	void set_minioninfoState(int id, MINION_STATE state) { m_minion[id].state = state; }

	void EnterGame();
	void FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj);

	//void net_lobby_enterClient(int id, CAMP_STATE camp, bool isHost);

};

