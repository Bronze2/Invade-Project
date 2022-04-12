#pragma once

#include "Scene.h"

class CLayer;

class CGamePlayScene : public CScene
{
private:
	CLayer* m_arrLayer[MAX_LAYER];

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FinalUpdate() override;

public:
	CGamePlayScene();
	virtual ~CGamePlayScene();
};

