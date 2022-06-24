#pragma once
#include "Entity.h"
class CLayer;
class CGameObject;
class CCamera;

class CScene :public CEntity
{
private:
	CLayer* m_arrLayer[MAX_LAYER];
	int index;
public:
	virtual void Init(int m_index) { index = m_index; }
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	void SetLayerName(int _iIdx, const wstring& _strName);
	void AddGameObject(int index ,const wstring& _strLayerName, CGameObject* _pObject, bool _bMoveAll);
	void AddGameObject(int index, int _iLayerIdx, CGameObject* _pObject, bool _bMoveAll);
	CLayer* FindLayer(const wstring& _strLayerName);
	CLayer* GetLayer(int _iIdx) { return m_arrLayer[_iIdx]; }

private:
	void AddLayer(const wstring& _strLayerName, int _iIdx = 1);
public:
	CScene();
	virtual ~CScene();
};