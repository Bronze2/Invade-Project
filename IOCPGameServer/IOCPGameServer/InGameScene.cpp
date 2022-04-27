#include "pch.h"
#include "InGameScene.h"
#include "Layer.h"
#include "GameObject.h"

#include "Transform.h"
#include "EventMgr.h"
#include "Collider3D.h"
#include "CollisionMgr.h"
//#include "TimeMgr.h"
//#include "Device.h"
//#include "SensorMgr.h"

#include "PlayerScript.h"
//#include "MonsterScript.h"
//#include "ParticleSystem.h"
//#include "ArrowScript.h"
//#include "MinionScript.h"
//#include "GameFramework.h"
//#include "Animation3D.h"
//#include "SpawnScript.h"
//#include "Sensor.h"
//#include "TowerScript.h"
//#include "EmptyPlayerScript.h"

void CInGameScene::Init()
{
	GetLayer(0)->SetName(L"red");
	GetLayer(1)->SetName(L"blue");


	CGameObject* pEmptyPlayer = new CGameObject;
	pEmptyPlayer->AddComponent(new CTransform);
	pEmptyPlayer->AddComponent(new CPlayerScript);
	pEmptyPlayer->AddComponent(new CCollider3D);
	pEmptyPlayer->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pEmptyPlayer->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));      // 80.f, 200.f, 80.f ?????
	pEmptyPlayer->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pEmptyPlayer->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
	pEmptyPlayer->Transform()->SetLocalPos(Vec3(100, 100, 100));
	FindLayer(L"red")->AddGameObject(pEmptyPlayer, false);

	CGameObject* pEmptyPlayer2 = new CGameObject;
	pEmptyPlayer2->AddComponent(new CTransform);
	pEmptyPlayer2->AddComponent(new CPlayerScript);
	pEmptyPlayer2->AddComponent(new CCollider3D);
	pEmptyPlayer2->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pEmptyPlayer2->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));      // 80.f, 200.f, 80.f ?????
	pEmptyPlayer2->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pEmptyPlayer2->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
	pEmptyPlayer2->Transform()->SetLocalPos(Vec3(100, 100, 100));
	FindLayer(L"blue")->AddGameObject(pEmptyPlayer2, false);


	CCollisionMgr::GetInst()->CheckCollisionLayer(L"red", L"blue");
	Awake();
	Start();
}
