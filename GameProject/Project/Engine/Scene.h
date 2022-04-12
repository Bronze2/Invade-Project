#pragma once
#include "Entity.h"
class CLayer;
class CGameObject;
class CCamera;

class CScene:public CEntity
{
private:
	CLayer* m_arrLayer[MAX_LAYER];

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();

	void SetLayerName(int _iIdx, const wstring& _strName);
	void AddGameObject(const wstring& _strLayerName, CGameObject* _pObject, bool _bMoveAll);
	void AddGameObject(int _iLayerIdx, CGameObject* _pObject, bool _bMoveAll);
	CLayer* FindLayer(const wstring& _strLayerName);
	CLayer* GetLayer(int _iIdx) { return m_arrLayer[_iIdx]; }

protected:
	void AddLayer(const wstring& _strLayerName, int _iIdx = 1);

public:
	CScene();
	virtual ~CScene();
};

