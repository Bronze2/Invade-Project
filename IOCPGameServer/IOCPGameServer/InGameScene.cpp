#include "pch.h"
#include "InGameScene.h"
#include "Layer.h"
#include "GameObject.h"

#include "Transform.h"
#include "EventMgr.h"
#include "Collider3D.h"
#include "CollisionMgr.h"
//#include "TimeMgr.h"
#include "SensorMgr.h"

#include "PlayerScript.h"
//#include "MonsterScript.h"
//#include "ParticleSystem.h"
//#include "ArrowScript.h"
//#include "MinionScript.h"
//#include "GameFramework.h"
//#include "Animation3D.h"
#include "SpawnScript.h"
#include "Sensor.h"
#include "TowerScript.h"
//#include "EmptyPlayerScript.h"


void CInGameScene::Init()
{
	GetLayer(0)->SetName(L"Red");
	GetLayer(1)->SetName(L"Blue");
	GetLayer(2)->SetName(L"RedSpawnPlace");
	GetLayer(3)->SetName(L"BlueSpawnPlace");

	for (int i = 0; i < SHARED_DATA::current_user; ++i) {
		CGameObject* pObject = new CGameObject;
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
		pObject->AddComponent(new CPlayerScript);
		//pObject->AddComponent(new CSensor);
		//pObject->Sensor()->SetRadius(300.f);
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 100.f, 200.f));    // 80.f, 200.f, 80.f ?????
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
		pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		pObject->GetScript<CPlayerScript>()->m_SetId(i);
		pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		pObject->Transform()->SetLocalScale(Vec3(0.5f, 0.5f, 0.5f));
		pObject->GetScript<CPlayerScript>()->Init();
		FindLayer(L"Blue")->AddGameObject(pObject, false);

		//if (i % 2 == 0)
		//	FindLayer(L"Blue")->AddGameObject(pObject, false);
		//else
		//	FindLayer(L"Red")->AddGameObject(pObject, false);

		std::cout << "플레이어 [" << i << "] 생성" << endl;
	}

	
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
	FindLayer(L"Red")->AddGameObject(pObject);



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
	FindLayer(L"Blue")->AddGameObject(pObject1);


	///////////////////--타워
	CGameObject* pRedFirstTower = new CGameObject;
	pRedFirstTower->SetName(L"FirstTower");
	pRedFirstTower->AddComponent(new CTransform);
	pRedFirstTower->AddComponent(new CCollider3D);
	pRedFirstTower->AddComponent(new CSensor);
	pRedFirstTower->AddComponent(new CTowerScript);
	pRedFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
	pRedFirstTower->GetScript<CTowerScript>()->m_SetId(0);
	pRedFirstTower->Sensor()->SetRadius(400.f);
	pRedFirstTower->Transform()->SetLocalPos(Vec3(-200.f, 0.f, 3550.f));
	pRedFirstTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedFirstTower->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
	pRedFirstTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRedFirstTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pRedFirstTower->Collider3D()->SetOffsetPos(Vec3(0.f, 110.f, 25.f));
	FindLayer(L"Red")->AddGameObject(pRedFirstTower);

	CGameObject* pRedSecondTower = new CGameObject;
	pRedSecondTower->SetName(L"SecondTower");
	pRedSecondTower->AddComponent(new CTransform);
	pRedSecondTower->AddComponent(new CCollider3D);
	pRedSecondTower->AddComponent(new CSensor);
	pRedSecondTower->AddComponent(new CTowerScript);
	pRedSecondTower->Sensor()->SetRadius(400.f);
	pRedSecondTower->Transform()->SetLocalPos(Vec3(200.f, 0.f, 2700.f));
	pRedSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
	pRedSecondTower->GetScript<CTowerScript>()->m_SetId(1);
	pRedSecondTower->GetScript<CTowerScript>()->SetFirstTower(pRedFirstTower);
	pRedSecondTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedSecondTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRedSecondTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pRedSecondTower->Collider3D()->SetOffsetPos(Vec3(0.f, 110.f, 25.f));
	FindLayer(L"Red")->AddGameObject(pRedSecondTower);

	CGameObject* pBlueFirstTower = new CGameObject;
	pBlueFirstTower->AddComponent(new CTransform);
	pBlueFirstTower->AddComponent(new CCollider3D);
	pBlueFirstTower->AddComponent(new CSensor);
	pBlueFirstTower->AddComponent(new CTowerScript);
	pBlueFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
	pBlueFirstTower->GetScript<CTowerScript>()->m_SetId(2);
	pBlueFirstTower->Sensor()->SetRadius(400.f);
	pBlueFirstTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pBlueFirstTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pBlueFirstTower->Collider3D()->SetOffsetPos(Vec3(0.f, 110.f, 25.f));
	pBlueFirstTower->Transform()->SetLocalPos(Vec3(-200.f, 0.f, 1850.f));
	pBlueFirstTower->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pBlueFirstTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	FindLayer(L"Blue")->AddGameObject(pBlueFirstTower);
	
	CGameObject* pBlueSecondTower = new CGameObject;
	pBlueSecondTower->AddComponent(new CTransform);
	pBlueSecondTower->AddComponent(new CCollider3D);
	pBlueSecondTower->AddComponent(new CSensor);
	pBlueSecondTower->AddComponent(new CTowerScript);
	pBlueSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
	pBlueSecondTower->GetScript<CTowerScript>()->m_SetId(3);
	pBlueSecondTower->Sensor()->SetRadius(400.f);
	pBlueSecondTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pBlueSecondTower->GetScript<CTowerScript>()->SetFirstTower(pBlueFirstTower);
	pBlueSecondTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pBlueSecondTower->Collider3D()->SetOffsetPos(Vec3(0.f, 110.f, 25.f));
	pBlueSecondTower->Transform()->SetLocalPos(Vec3(200.f, 0.f, 1000.f));
	pBlueSecondTower->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pBlueSecondTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	FindLayer(L"Blue")->AddGameObject(pBlueSecondTower);
	////////


	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Red");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Blue");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Blue");

	CSensorMgr::GetInst()->CheckSensorLayer(L"Blue", L"Red");

	Awake();
	Start();
}
