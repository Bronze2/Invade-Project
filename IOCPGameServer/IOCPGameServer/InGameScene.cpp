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


void CInGameScene::Init(int index)
{

	GetLayer(0)->SetName(L"Red");
	GetLayer(0)->SetIndex(index);
	GetLayer(1)->SetName(L"Player");
	GetLayer(1)->SetIndex(index);
	GetLayer(2)->SetName(L"Blue");
	GetLayer(2)->SetIndex(index);
	GetLayer(3)->SetName(L"RedSpawnPlace");
	GetLayer(3)->SetIndex(index);
	GetLayer(4)->SetName(L"BlueSpawnPlace");
	GetLayer(4)->SetIndex(index);
	GetLayer(5)->SetName(L"Arrow");
	GetLayer(5)->SetIndex(index);
	GetLayer(10)->SetName(L"Obstacle");
	GetLayer(10)->SetIndex(index);



	for ( auto &cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == index) {
			CGameObject* pObject = new CGameObject;
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CCollider3D);
			pObject->AddComponent(new CPlayerScript);

			pObject->AddComponent(new CSensor);
			pObject->Sensor()->SetRadius(300.f);

			pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 10.f, 10.f));    // 80.f, 200.f, 80.f ?????
			pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
			pObject->GetScript<CPlayerScript>()->m_SetId(cl.second.m_id);
			pObject->Transform()->SetLocalScale(Vec3(0.5f, 0.5f, 0.5f));
			pObject->GetScript<CPlayerScript>()->Init();
			pObject->GetScript<CPlayerScript>()->SetIndex(index);
			if (cl.second.m_camp == CAMP_STATE::BLUE) {
				pObject->GetScript<CPlayerScript>()->SetCamp(CAMP_STATE::BLUE);
				FindLayer(L"Blue")->AddGameObject(pObject, false);
				//pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 1125.f));
				pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 500.f));
				cout << "BLUE - " << cl.second.m_id << endl;

			}
			else if(cl.second.m_camp == CAMP_STATE::RED) {
				pObject->GetScript<CPlayerScript>()->SetCamp(CAMP_STATE::RED);
				FindLayer(L"Red")->AddGameObject(pObject, false);
				//pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 5800.f));
				pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
				cout << "RED - " << cl.second.m_id << endl;


			}

			//if (i % 2 == 0) {
			//	FindLayer(L"Blue")->AddGameObject(pObject, false);
			//	pObject->GetScript<CPlayerScript>()->SetCamp(CAMP_STATE::BLUE);

			//}
			//else {
			//	FindLayer(L"Red")->AddGameObject(pObject, false);
			//	pObject->GetScript<CPlayerScript>()->SetCamp(CAMP_STATE::RED);
			//}
			std::cout << "플레이어 [" << cl.first << "] 생성" << endl;
		}
	}

	



	///////////////////--타워
	CGameObject* pRedFirstTower = new CGameObject;
	pRedFirstTower->SetName(L"FirstTower");
	pRedFirstTower->AddComponent(new CTransform);
	pRedFirstTower->AddComponent(new CCollider3D);
	pRedFirstTower->AddComponent(new CSensor);
	pRedFirstTower->AddComponent(new CTowerScript);
	pRedFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
	pRedFirstTower->GetScript<CTowerScript>()->m_SetId(0);
	pRedFirstTower->GetScript<CTowerScript>()->SetCamp(CAMP_STATE::RED);
	pRedFirstTower->GetScript<CTowerScript>()->SetIndex(index);
	pRedFirstTower->Sensor()->SetRadius(1000.f);
	pRedFirstTower->Transform()->SetLocalPos(Vec3(-1625, 0.f, 3000.f));
	pRedFirstTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedFirstTower->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
	pRedFirstTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRedFirstTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pRedFirstTower->Collider3D()->SetOffsetPos(Vec3(0.f, 330, 25.f));
	FindLayer(L"Red")->AddGameObject(pRedFirstTower);

	CGameObject* pRedSecondTower = new CGameObject;
	pRedSecondTower->SetName(L"SecondTower");
	pRedSecondTower->AddComponent(new CTransform);
	pRedSecondTower->AddComponent(new CCollider3D);
	pRedSecondTower->AddComponent(new CSensor);
	pRedSecondTower->AddComponent(new CTowerScript);
	pRedSecondTower->Sensor()->SetRadius(1500.f);
	pRedSecondTower->Transform()->SetLocalPos(Vec3(875.f, 0.f, 5500.f));
	pRedSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
	pRedSecondTower->GetScript<CTowerScript>()->m_SetId(1);
	pRedSecondTower->GetScript<CTowerScript>()->SetFirstTower(pRedFirstTower);
	pRedSecondTower->GetScript<CTowerScript>()->SetCamp(CAMP_STATE::RED);
	pRedSecondTower->GetScript<CTowerScript>()->SetIndex(index);

	pRedSecondTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedSecondTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRedSecondTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pRedSecondTower->Collider3D()->SetOffsetPos(Vec3(0.f, 330.f, 25.f));
	FindLayer(L"Red")->AddGameObject(pRedSecondTower);

	CGameObject* pBlueFirstTower = new CGameObject;
	pBlueFirstTower->AddComponent(new CTransform);
	pBlueFirstTower->AddComponent(new CCollider3D);
	pBlueFirstTower->AddComponent(new CSensor);
	pBlueFirstTower->AddComponent(new CTowerScript);
	pBlueFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
	pBlueFirstTower->GetScript<CTowerScript>()->m_SetId(2);
	pBlueFirstTower->GetScript<CTowerScript>()->SetCamp(CAMP_STATE::BLUE);
	pBlueFirstTower->GetScript<CTowerScript>()->SetIndex(index);
	pBlueFirstTower->Sensor()->SetRadius(1000.f);
	pBlueFirstTower->Transform()->SetLocalPos(Vec3(375.f, 0.f, 300.f));
	pBlueFirstTower->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pBlueFirstTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueFirstTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pBlueFirstTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pBlueFirstTower->Collider3D()->SetOffsetPos(Vec3(0.f, 110.f, 25.f));
	FindLayer(L"Blue")->AddGameObject(pBlueFirstTower);
	
	CGameObject* pBlueSecondTower = new CGameObject;
	pBlueSecondTower->AddComponent(new CTransform);
	pBlueSecondTower->AddComponent(new CCollider3D);
	pBlueSecondTower->AddComponent(new CSensor);
	pBlueSecondTower->AddComponent(new CTowerScript);
	pBlueSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
	pBlueSecondTower->GetScript<CTowerScript>()->SetCamp(CAMP_STATE::BLUE);
	pBlueSecondTower->GetScript<CTowerScript>()->m_SetId(3);
	pBlueSecondTower->GetScript<CTowerScript>()->SetIndex(index);

	pBlueSecondTower->Sensor()->SetRadius(1500.f);
	pBlueSecondTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pBlueSecondTower->Transform()->SetLocalPos(Vec3(-2125.f, 0.f, -2200.f));
	pBlueSecondTower->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pBlueSecondTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueSecondTower->GetScript<CTowerScript>()->SetFirstTower(pBlueFirstTower);
	pBlueSecondTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pBlueSecondTower->Collider3D()->SetOffsetPos(Vec3(0.f, 110.f, 25.f));
	FindLayer(L"Blue")->AddGameObject(pBlueSecondTower);
	////////


		// 블루 넥서스
	CGameObject* pBlueNexus = new CGameObject;
	//   pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Nexus01.fbx");
	//pMeshData->Save(pMeshData->GetPath());
	pBlueNexus->AddComponent(new CTransform);
	pBlueNexus->AddComponent(new CTowerScript);
	pBlueNexus->AddComponent(new CCollider3D);
	pBlueNexus->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pBlueNexus->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pBlueNexus->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));

	pBlueNexus->Transform()->SetLocalPos(Vec3(-625.f, 35.f, -4700.f));
	pBlueNexus->Transform()->SetLocalRot(Vec3(-3.14f / 6, 0.F, 0.f));

	pBlueNexus->Transform()->SetLocalScale(Vec3(130.f, 130.f, 130.f));
	pBlueNexus->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::BLUE);
	pBlueNexus->GetScript<CTowerScript>()->SetType(TOWER_TYPE::NEXUS);
	pBlueNexus->GetScript<CTowerScript>()->SetSecondTower(pBlueSecondTower);
	pBlueNexus->GetScript<CTowerScript>()->Init();
	pBlueSecondTower->GetScript<CTowerScript>()->SetNexus(pBlueNexus);
	FindLayer(L"Blue")->AddGameObject(pBlueNexus);

	CGameObject* pBlueSpawnPlace = new CGameObject;
	pBlueSpawnPlace->SetName(L"Spawn_Place");
	pBlueSpawnPlace->AddComponent(new CTransform);
	pBlueSpawnPlace->AddComponent(new CSpawnScript);
	pBlueSpawnPlace->Transform()->SetLocalPos(Vec3(-625.f, 0.f, -4700.f));
	pBlueSpawnPlace->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueSpawnPlace->GetScript<CSpawnScript>()->SetSpawnState(CAMP_STATE::BLUE);

	FindLayer(L"Blue")->AddGameObject(pBlueSpawnPlace);

	// 레드 넥서스
	CGameObject* pRedNexus = new CGameObject;
	pRedNexus->AddComponent(new CTransform);
	pRedNexus->AddComponent(new CCollider3D);
	pRedNexus->AddComponent(new CTowerScript);
	pRedNexus->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRedNexus->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pRedNexus->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));
	pRedNexus->Transform()->SetLocalPos(Vec3(-625.f, 35.f, 8000.f));
	pRedNexus->Transform()->SetLocalRot(Vec3(-3.14f / 6, 3.14f, 0.f));

	pRedNexus->Transform()->SetLocalScale(Vec3(130.f, 130.f, 130.f));
	pRedNexus->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::RED);
	pRedNexus->GetScript<CTowerScript>()->SetType(TOWER_TYPE::NEXUS);
	pRedNexus->GetScript<CTowerScript>()->SetSecondTower(pRedSecondTower);
	pRedNexus->GetScript<CTowerScript>()->Init();
	pRedSecondTower->GetScript<CTowerScript>()->SetNexus(pRedNexus);
	FindLayer(L"Red")->AddGameObject(pRedNexus);

	CGameObject* pRedSpawnPlace = new CGameObject;
	pRedSpawnPlace->SetName(L"Spawn_Place");
	pRedSpawnPlace->AddComponent(new CTransform);
	pRedSpawnPlace->AddComponent(new CSpawnScript);
	pRedSpawnPlace->Transform()->SetLocalPos(Vec3(-625.f, 0.f, 8000.f));
	pRedSpawnPlace->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	pRedSpawnPlace->GetScript<CSpawnScript>()->SetSpawnState(CAMP_STATE::RED);
	pBlueSpawnPlace->GetScript<CSpawnScript>()->SetEnemyNexus(pRedNexus);
	pRedSpawnPlace->GetScript<CSpawnScript>()->SetEnemyNexus(pBlueNexus);
	pBlueSpawnPlace->GetScript<CSpawnScript>()->SetFirstTower(pRedFirstTower);

	pRedSpawnPlace->GetScript<CSpawnScript>()->SetFirstTower(pBlueFirstTower);
	pBlueSpawnPlace->GetScript<CSpawnScript>()->SetSecondTower(pRedSecondTower);
	pRedSpawnPlace->GetScript<CSpawnScript>()->SetSecondTower(pBlueSecondTower);
	FindLayer(L"Red")->AddGameObject(pRedSpawnPlace);







	//CGameObject* pNexus = nullptr;
	//CGameObject* pObject = new CGameObject;
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CCollider3D);
	//pObject->AddComponent(new CTowerScript);
	//pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	//pObject->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	//pObject->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));
	//pObject->Transform()->SetLocalPos(Vec3(-625.f, 35.f, -4700.f));
	//pObject->Transform()->SetLocalRot(Vec3(-3.14f / 6, 0.F, 0.f));
	//pObject->Transform()->SetLocalScale(Vec3(70.f, 70.f, 70.f));
	//pObject->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::BLUE);
	//pObject->GetScript<CTowerScript>()->SetType(TOWER_TYPE::NEXUS);
	//pObject->GetScript<CTowerScript>()->SetSecondTower(pBlueSecondTower);
	//pObject->GetScript<CTowerScript>()->Init();

	//FindLayer(L"Blue")->AddGameObject(pObject);
	//pNexus = pObject;
	//pObject = new CGameObject;
	//pObject->SetName(L"Spawn_Place");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CSpawnScript);
	//pObject->Transform()->SetLocalPos(Vec3(-625.f, 0.f, -4700.f));
	//pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject->FrustumCheck(false);
	//pObject->GetScript<CSpawnScript>()->SetSpawnState(CAMP_STATE::RED);
	//pObject->GetScript<CSpawnScript>()->SetEnemyNexus(pNexus);
	//pObject->GetScript<CSpawnScript>()->SetIndex(index);
	//FindLayer(L"Red")->AddGameObject(pObject);


	//CGameObject* pObject1 = new CGameObject;
	//pObject1->AddComponent(new CTransform);
	//pObject1->AddComponent(new CCollider3D);
	//pObject1->AddComponent(new CTowerScript);
	//pObject1->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	//pObject1->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	//pObject1->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));
	//pObject1->Transform()->SetLocalPos(Vec3(-625.f, 35.f, 8000.f));
	//pObject1->Transform()->SetLocalRot(Vec3(-3.14f / 6, 3.14f, 0.f));
	//pObject1->Transform()->SetLocalScale(Vec3(70.f, 70.f, 70.f));
	//pObject1->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::RED);
	//pObject1->GetScript<CTowerScript>()->SetType(TOWER_TYPE::NEXUS);
	//pObject1->GetScript<CTowerScript>()->SetSecondTower(pRedSecondTower);
	//pObject1->GetScript<CTowerScript>()->Init();
	//pNexus = pObject1;
	//FindLayer(L"Red")->AddGameObject(pObject1);
	//pNexus = pObject1;
	//pObject1 = new CGameObject;
	//pObject1->SetName(L"Spawn_Place");
	//pObject1->AddComponent(new CTransform);
	//pObject1->AddComponent(new CSpawnScript);
	//pObject1->FrustumCheck(false);
	//pObject1->Transform()->SetLocalPos(Vec3(-625.f, 35.f, 8000.f));
	//pObject1->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//pObject1->GetScript<CSpawnScript>()->SetSpawnState(CAMP_STATE::BLUE);
	//pObject1->GetScript<CSpawnScript>()->SetEnemyNexus(pNexus);
	//pObject1->GetScript<CSpawnScript>()->SetIndex(index);
	//FindLayer(L"Blue")->AddGameObject(pObject1);













	CCollisionMgr::GetInst()->CheckCollisionLayer(index,L"Red", L"Red");
	CCollisionMgr::GetInst()->CheckCollisionLayer(index,L"Blue", L"Blue");
	CCollisionMgr::GetInst()->CheckCollisionLayer(index,L"Red", L"Blue");
	CCollisionMgr::GetInst()->CheckCollisionLayer(index,L"Red", L"Arrow");
	CCollisionMgr::GetInst()->CheckCollisionLayer(index,L"Blue", L"Arrow");




	CSensorMgr::GetInst()->CheckSensorLayer(index, L"Blue", L"Red");

	Awake();
	Start(); 
	isinit = true;
}
