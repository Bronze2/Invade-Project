#pragma once
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

	CScene* GetCurScene();
	void ChangeScene(CScene* _pNextScene);
	void FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj);
};

