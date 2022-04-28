#include "pch.h"
#include "InGameScene.h"
#include "Layer.h"
#include "GameObject.h"

#include "Transform.h"
#include "EventMgr.h"
#include "Collider3D.h"
#include "CollisionMgr.h"
//#include "TimeMgr.h"
//#include "SensorMgr.h"

#include "PlayerScript.h"
//#include "MonsterScript.h"
//#include "ParticleSystem.h"
//#include "ArrowScript.h"
//#include "MinionScript.h"
//#include "GameFramework.h"
//#include "Animation3D.h"
#include "SpawnScript.h"
//#include "Sensor.h"
//#include "TowerScript.h"
//#include "EmptyPlayerScript.h"

void CInGameScene::Init()
{
	GetLayer(0)->SetName(L"Red");
	GetLayer(1)->SetName(L"Blue");
	GetLayer(2)->SetName(L"RedSpawnPlace");
	GetLayer(3)->SetName(L"BlueSpawnPlace");


	//CGameObject* pEmptyPlayer = new CGameObject;
	//pEmptyPlayer->AddComponent(new CTransform);
	//pEmptyPlayer->AddComponent(new CPlayerScript);
	//pEmptyPlayer->AddComponent(new CCollider3D);
	//pEmptyPlayer->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	//pEmptyPlayer->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));      
	//pEmptyPlayer->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	//pEmptyPlayer->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
	//pEmptyPlayer->Transform()->SetLocalPos(Vec3(100, 100, 100));
	//FindLayer(L"Red")->AddGameObject(pEmptyPlayer, false);

	//CGameObject* pEmptyPlayer2 = new CGameObject;
	//pEmptyPlayer2->AddComponent(new CTransform);
	//pEmptyPlayer2->AddComponent(new CPlayerScript);
	//pEmptyPlayer2->AddComponent(new CCollider3D);
	//pEmptyPlayer2->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	//pEmptyPlayer2->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));      
	//pEmptyPlayer2->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	//pEmptyPlayer2->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
	//pEmptyPlayer2->Transform()->SetLocalPos(Vec3(100, 100, 100));
	//FindLayer(L"Blue")->AddGameObject(pEmptyPlayer2, false);
	
	CGameObject* pNexus = nullptr;

	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));
	pObject->Transform()->SetLocalPos(Vec3(-50.f, 35.f, 150.f));
	pObject->Transform()->SetLocalRot(Vec3(-3.14f / 6, 0.F, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(70.f, 70.f, 70.f));
	FindLayer(L"Blue")->AddGameObject(pObject);
	pNexus = pObject;
	pObject = new CGameObject;
	pObject->SetName(L"Spawn_Place");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CSpawnScript);
	pObject->Transform()->SetLocalPos(Vec3(-50.f, 0.f, 4150.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->FrustumCheck(false);
	pObject->GetScript<CSpawnScript>()->SetSpawnState(CAMP_STATE::RED);
	pObject->GetScript<CSpawnScript>()->SetEnemyNexus(pNexus);
	FindLayer(L"RedSpawnPlace")->AddGameObject(pObject);

	CGameObject* pObject1 = new CGameObject;
	pObject1->AddComponent(new CTransform);
	pObject1->AddComponent(new CCollider3D);
	pObject1->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject1->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pObject1->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));
	pObject1->Transform()->SetLocalPos(Vec3(50.f, 35.f, 4400.f));
	pObject1->Transform()->SetLocalRot(Vec3(-3.14f / 6, 3.14f, 0.f));
	pObject1->Transform()->SetLocalScale(Vec3(70.f, 70.f, 70.f));
	pNexus = pObject1;
	FindLayer(L"Red")->AddGameObject(pObject1);
	pNexus = pObject1;
	pObject1 = new CGameObject;
	pObject1->SetName(L"Spawn_Place");
	pObject1->AddComponent(new CTransform);
	pObject1->AddComponent(new CSpawnScript);
	pObject1->FrustumCheck(false);
	pObject1->Transform()->SetLocalPos(Vec3(-50.f, 0.f, 400.f));
	pObject1->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject1->GetScript<CSpawnScript>()->SetSpawnState(CAMP_STATE::BLUE);
	pObject1->GetScript<CSpawnScript>()->SetEnemyNexus(pNexus);
	FindLayer(L"BlueSpawnPlace")->AddGameObject(pObject1);
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Blue");


	Awake();
	Start();
}
