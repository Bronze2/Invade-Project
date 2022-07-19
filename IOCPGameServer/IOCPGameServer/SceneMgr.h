#pragma once
class CScene;
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
	//CScene* m_pCurScene;
	map<int, CScene*> m_pCurScene;
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];
	vector<CGameObject*> m_player;

public:
	void Init(int index);
	void Update();
	void Init(SCENE_TYPE _eType);
	

	CScene* GetCurScene(int index);
	void ChangeScene(CScene* _pNextScene , int index);
	void ChangeScene(SCENE_TYPE _Type, int index);
	void EnterGame(int index);
	void FindGameObjectByTag(int index , const wstring& _strTag, vector<CGameObject*>& _vecFindObj);
	void InitArrowByPlayerId(int index, int ClientId,int ArrowId ,Vec3 Pos, Vec3 Rot, Vec3 Dir, float Power, CAMP_STATE camp , PACKET_SKILL skill);
	void collisionArrow(int index, int coll_id, PACKET_COLLTYPE coll_type, CAMP_STATE camp);
};

