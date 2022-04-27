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
	CScene* m_pCurScene;
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];
	vector<CGameObject*> m_player;

public:
	void Init();
	void Update();
	void Init(SCENE_TYPE _eType);
	

	CScene* GetCurScene();
	void ChangeScene(CScene* _pNextScene);
	void ChangeScene(SCENE_TYPE _Type);
	void EnterGame();
	void FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj);

};

