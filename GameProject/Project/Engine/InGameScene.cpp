#include "pch.h"
#include "InGameScene.h"
#include "Layer.h"
#include "GameObject.h"

#include "ResMgr.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "MeshRender.h"
#include "Animator2D.h"
#include "Animation2D.h"
#include "Light2D.h"
#include "Light3D.h"
#include "MeshData.h"
#include "Collider3D.h"
#include "Animator3D.h"
#include "Terrain.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Camera.h"

#include "CollisionMgr.h"
#include "EventMgr.h"
#include "RenderMgr.h"
#include "Device.h"
#include "SensorMgr.h"

#include "GridScript.h"
#include "CameraScript.h"
#include "PlayerScript.h"
#include "ParticleSystem.h"
#include "ArrowScript.h"
#include "MinionScript.h"
#include "GameFramework.h"
#include "Animation3D.h"
#include "SpawnScript.h"
#include "Sensor.h"
#include "ProjectileScript.h"
#include "TowerScript.h"
#include "EmptyPlayerScript.h"
#include "BowScript.h"
#include "WaterSkill0Script.h"
#include "SkillMgr.h" 
#include "CrossHairScript.h"
#include "Font.h"
#include "FontMgr.h"

void CInGameScene::Init()
{
	ShowCursor(false);
	Ptr<CTexture> pColor = CResMgr::GetInst()->FindRes<CTexture>(L"Tile");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->FindRes<CTexture>(L"Tile_n");

	GetLayer(0)->SetName(L"Default");
	GetLayer(3)->SetName(L"Blue");
	GetLayer(4)->SetName(L"Red");
	GetLayer(5)->SetName(L"Cover");
	GetLayer(6)->SetName(L"Arrow");
	GetLayer(7)->SetName(L"Terrain");
	GetLayer(8)->SetName(L"HpBar");
	GetLayer(9)->SetName(L"Obstacle");
	GetLayer(30)->SetName(L"UI");
	GetLayer(31)->SetName(L"Tool");

	//-----------------------------------------------------------------------------------------------
	// 카메라
	//-----------------------------------------------------------------------------------------------

	CGameObject* pMainCam = nullptr;

	pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	pMainCam->Camera()->SetFar(30000.f);
	pMainCam->Camera()->SetLayerAllCheck();
	CCollisionMgr::GetInst()->SetCameraObject(pMainCam->Camera());
	CRenderMgr::GetInst()->SetCamera(pMainCam->Camera());

	CGameObject* pObject = nullptr;

	//-----------------------------------------------------------------------------------------------
	// 조명
	//-----------------------------------------------------------------------------------------------

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight3D);
	pObject->Light3D()->SetLightPos(Vec3(0.f, 500.f, 0.f));
	pObject->Light3D()->SetLightType(LIGHT_TYPE::DIR);
	pObject->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
	pObject->Light3D()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
	pObject->Light3D()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
	pObject->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
	pObject->Light3D()->SetLightRange(1000.f);
	pObject->Transform()->SetLocalPos(Vec3(-1000.f, 1000.f, -1500.f));
	FindLayer(L"Default")->AddGameObject(pObject);

	//-----------------------------------------------------------------------------------------------
	// 플레이어 
	//-----------------------------------------------------------------------------------------------

	//Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\player_red.fbx");
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player_blue.mdat", L"MeshData\\player_blue.mdat");
	//pMeshData->Save(pMeshData->GetPath());

	pObject = new CGameObject;
	pObject = pMeshData->Instantiate();
	pObject->SetName(L"Monster");
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CSensor);
	pObject->AddComponent(new CPlayerScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(80.f, 80.f, 200.f));      // 80.f, 200.f, 80.f ?????
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(-0.f, 0.f, 1000.f));
	pObject->Transform()->SetLocalScale(Vec3(0.4f, 0.4f, 0.5f));
	pObject->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
	pObject->MeshRender()->SetDynamicShadow(true);
	pObject->Sensor()->SetRadius(500.f);
	pObject->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::DARK);

	CGameObject* pPlayer = pObject;
	pObject->GetScript<CPlayerScript>()->SetCurHp(50);
	// pMainCam->Transform()->SetLocalPos(Vec3(0.f, 100.f, 130.f));

	pMainCam->Camera()->SetPlayer(pObject);
	pMainCam->Camera()->SetbPlay(true);

	CAnimation* pNewAnimation = new CAnimation;
	pNewAnimation->InsertAnimClip(L"IDLE", 0, 37);
	pNewAnimation->InsertAnimClip(L"WALK", 44, 73);
	pNewAnimation->InsertAnimClip(L"JUMP", 81, 100);
	pNewAnimation->InsertAnimClip(L"DIE", 242, 261);      // 결과씬
	pNewAnimation->InsertAnimClip(L"ATTACK_READY", 145, 167);
	pNewAnimation->InsertAnimClip(L"ATTACK", 168, 175);
	pNewAnimation->InsertAnimClip(L"ATTACK_READY_HIGH_1", 202, 207);
	pNewAnimation->InsertAnimClip(L"ATTACK_READY_HIGH_2", 202, 208);
	pNewAnimation->InsertAnimClip(L"ATTACK_READY_HIGH_3", 202, 209);
	pNewAnimation->InsertAnimClip(L"ATTACK_READY_HIGH_4", 202, 210);
	pNewAnimation->InsertAnimClip(L"ATTACK_READY_HIGH_5", 202, 212);
	pNewAnimation->InsertAnimClip(L"DEMAGED", 235, 240);
	pNewAnimation->InsertAnimClip(L"RUN", 298, 319);      // 305, 320
	//pNewAnimation->InsertAnimation(L"DIE", 269, 289, false, false);

	pObject->Animator3D()->SetAnimation(pNewAnimation);
	pObject->Animator3D()->SetAnimClip(pNewAnimation->GetAnimClip());
	pObject->GetScript<CPlayerScript>()->Init();

	FindLayer(L"Blue")->AddGameObject(pObject, false);

	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\bow_big.mdat", L"MeshData\\bow_big.mdat");
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\bow_big.fbx");
	CGameObject* pBow;
	pBow = pMeshData->Instantiate();
	pBow->SetName(L"Bow");
	pBow->AddComponent(new CBowScript);
	pBow->FrustumCheck(false);
	pBow->Transform()->SetLocalPos(Vec3(0.0f, 0.0f, 0.0f));
	pBow->Transform()->SetLocalScale(Vec3(2.4f, 1.2f, 2.4f));
	pBow->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, 0.0f));
	pBow->MeshRender()->SetDynamicShadow(true);
	pBow->GetScript<CBowScript>()->SetTarget(pObject);
	pBow->GetScript<CBowScript>()->SetBoneIdx(14);

	Ptr<CTexture> pBowTex = CResMgr::GetInst()->FindRes<CTexture>(L"bow_big");
	pBow->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBowTex.GetPointer());

	pNewAnimation = new CAnimation;
	pNewAnimation->InsertAnimClip(L"IDLE", 0, 1);         // 0, 13
	pNewAnimation->InsertAnimClip(L"ATTACK_READY", 1, 12);
	pNewAnimation->InsertAnimClip(L"ATTACK", 13, 18);

	pBow->Animator3D()->SetAnimation(pNewAnimation);
	pBow->Animator3D()->SetAnimClip(pNewAnimation->GetAnimClip());
	pBow->GetScript<CBowScript>()->Init();
	pObject->AddChild(pBow);
	pBow->GetScript<CBowScript>()->SetPlayer(pObject);
	pObject->GetScript<CPlayerScript>()->SetBowObject(pBow);

	CGameObject* pEmptyPlayer = new CGameObject;
	pEmptyPlayer->AddComponent(new CTransform);
	pEmptyPlayer->AddComponent(new CEmptyPlayerScript);
	pEmptyPlayer->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(0.f), 0.f));

	pMainCam->Transform()->SetLocalPos(Vec3(-200.f, 100.f, -30.f));		// -300, 130, -50
	pMainCam->Transform()->SetLocalRot(Vec3(0, XMConvertToRadians(90.f), XMConvertToRadians(0.f)));		// -15.f
	pMainCam->GetScript<CCameraScript>()->Init();

	pEmptyPlayer->AddChild(pMainCam);

	FindLayer(L"Default")->AddGameObject(pEmptyPlayer);

//-----------------------------------------------------------------------------------------------
// 맵 (baseMap + 타워 + 넥서스)
//-----------------------------------------------------------------------------------------------

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\ingame.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\ingame.mdat", L"MeshData\\ingame.mdat");
	//pMeshData->Save(pMeshData->GetPath());

	pObject = pMeshData->Instantiate();

	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(0.f, -90.f, 0.f));		//0.f, 370.f, 0.f
	pObject->Transform()->SetLocalRot(Vec3(-PI / 2, PI, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(0.8f, 0.8f, 0.8f));
	pObject->MeshRender()->SetDynamicShadow(false);

	FindLayer(L"Default")->AddGameObject(pObject);

	// 레드 1차타워
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\LowPoly_Tower.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\LowPoly_Tower.mdat", L"MeshData\\LowPoly_Tower.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	CGameObject* pRedFirstTower;
	pRedFirstTower = pMeshData->Instantiate();
	pRedFirstTower->SetName(L"FirstTower");
	pRedFirstTower->AddComponent(new CCollider3D);
	pRedFirstTower->AddComponent(new CSensor);
	pRedFirstTower->AddComponent(new CTowerScript);
	pRedFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
	pRedFirstTower->Sensor()->SetRadius(300.f);
	pRedFirstTower->Transform()->SetLocalPos(Vec3(0.f, 0.f, 11500.f));
	pRedFirstTower->Transform()->SetLocalRot(Vec3(-PI/2, 0.f, 0.f));
	pRedFirstTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedFirstTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRedFirstTower->Collider3D()->SetOffsetScale(Vec3(300.f, 300.f, 800.f));
	pRedFirstTower->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 400.f));
	pRedFirstTower->FrustumCheck(false);
	pRedFirstTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::RED);		// 에이치피바
	pRedFirstTower->GetScript<CTowerScript>()->Init();
	pRedFirstTower->MeshRender()->SetDynamicShadow(true);

	FindLayer(L"Red")->AddGameObject(pRedFirstTower);

	// 레드 2차타워
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\LowPoly_Tower.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\LowPoly_Tower.mdat", L"MeshData\\LowPoly_Tower.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	CGameObject* pRedSecondTower;
	pRedSecondTower = pMeshData->Instantiate();
	pRedSecondTower->SetName(L"SecondTower");
	pRedSecondTower->AddComponent(new CCollider3D);
	pRedSecondTower->AddComponent(new CSensor);
	pRedSecondTower->AddComponent(new CTowerScript);
	pRedSecondTower->Sensor()->SetRadius(300.f);
	pRedSecondTower->Transform()->SetLocalPos(Vec3(2500.f, 0.f, 14500.f));
	pRedSecondTower->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
	pRedSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
	pRedSecondTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedSecondTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRedSecondTower->Collider3D()->SetOffsetScale(Vec3(300.f, 300.f, 800.f));
	pRedSecondTower->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 400.f));
	pRedSecondTower->FrustumCheck(false);
	pRedSecondTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::RED);
	pRedSecondTower->GetScript<CTowerScript>()->Init();
	pRedSecondTower->MeshRender()->SetDynamicShadow(true);

	FindLayer(L"Red")->AddGameObject(pRedSecondTower);

	// 블루 1차타워
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\LowPoly_Tower_Blue.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\LowPoly_Tower_Blue.mdat", L"MeshData\\LowPoly_Tower_Blue.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	CGameObject* pBlueFirstTower;
	pBlueFirstTower = pMeshData->Instantiate();
	pBlueFirstTower->AddComponent(new CCollider3D);
	pBlueFirstTower->AddComponent(new CSensor);
	pBlueFirstTower->AddComponent(new CTowerScript);
	pBlueFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
	pBlueFirstTower->Sensor()->SetRadius(300.f);
	pBlueFirstTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pBlueFirstTower->Collider3D()->SetOffsetScale(Vec3(300.f, 300.f, 800.f));
	pBlueFirstTower->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 400.f));
	pBlueFirstTower->FrustumCheck(false);
	pBlueFirstTower->Transform()->SetLocalPos(Vec3(2000.f, 0.f, 9000.f));
	pBlueFirstTower->Transform()->SetLocalRot(Vec3(-PI/2, 3.14f, 0.f));
	pBlueFirstTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueFirstTower->MeshRender()->SetDynamicShadow(true);
	pBlueFirstTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::BLUE);
	pBlueFirstTower->GetScript<CTowerScript>()->Init();
	FindLayer(L"Blue")->AddGameObject(pBlueFirstTower);

	// 블루 2차타워	
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\BlueArrow.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\LowPoly_Tower_Blue.mdat", L"MeshData\\LowPoly_Tower_Blue.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	CGameObject* pBlueSecondTower;
	pBlueSecondTower = pMeshData->Instantiate();
	pBlueSecondTower->AddComponent(new CCollider3D);
	pBlueSecondTower->AddComponent(new CSensor);
	pBlueSecondTower->AddComponent(new CTowerScript);
	pBlueSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
	pBlueSecondTower->Sensor()->SetRadius(300.f);
	pBlueSecondTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pBlueSecondTower->Collider3D()->SetOffsetScale(Vec3(300.f, 300.f, 800.f));
	pBlueSecondTower->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 400.f));
	pBlueSecondTower->FrustumCheck(false);
	pBlueSecondTower->Transform()->SetLocalPos(Vec3(-500.f, 0.f, 5500.f));
	pBlueSecondTower->Transform()->SetLocalRot(Vec3(-PI / 2, 3.14f, 0.f));
	pBlueSecondTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueSecondTower->MeshRender()->SetDynamicShadow(true);
	pBlueSecondTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::BLUE);
	pBlueSecondTower->GetScript<CTowerScript>()->Init();
	FindLayer(L"Blue")->AddGameObject(pBlueSecondTower);


	pRedFirstTower->GetScript<CTowerScript>()->SetSecondTower(pRedSecondTower);
	pBlueFirstTower->GetScript<CTowerScript>()->SetSecondTower(pBlueSecondTower);
	pRedSecondTower->GetScript<CTowerScript>()->SetFirstTower(pRedFirstTower);
	pBlueSecondTower->GetScript<CTowerScript>()->SetFirstTower(pBlueFirstTower);

	// 블루 넥서스
	CGameObject* pBlueNexus;	
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\LowPoly_Nexus.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\LowPoly_Nexus.mdat", L"MeshData\\LowPoly_Nexus.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	pBlueNexus = pMeshData->Instantiate();
	pBlueNexus->AddComponent(new CCollider3D);
	pBlueNexus->AddComponent(new CTowerScript);
	pBlueNexus->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pBlueNexus->GetScript<CTowerScript>()->SetType(TOWER_TYPE::NEXUS);
	pBlueNexus->Collider3D()->SetOffsetScale(Vec3(500.f, 500.f, 500.f));
	pBlueNexus->Collider3D()->SetOffsetPos(Vec3(0.f, -0.f, 250.f));

	pBlueNexus->FrustumCheck(false);
	pBlueNexus->Transform()->SetLocalPos(Vec3(1000.f, -10.f, 2000.f));
	pBlueNexus->Transform()->SetLocalRot(Vec3(-PI/2, PI, 0.f));

	pBlueNexus->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueNexus->MeshRender()->SetDynamicShadow(true);
	pBlueNexus->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::BLUE);
	pBlueNexus->GetScript<CTowerScript>()->Init();
	pBlueNexus->GetScript<CTowerScript>()->SetFirstTower(pBlueFirstTower);
	pBlueNexus->GetScript<CTowerScript>()->SetSecondTower(pBlueSecondTower);

	FindLayer(L"Blue")->AddGameObject(pBlueNexus);

	CGameObject* pBlueSpawnPlace = new CGameObject;
	pBlueSpawnPlace->SetName(L"Spawn_Place");
	pBlueSpawnPlace->AddComponent(new CTransform);
	pBlueSpawnPlace->AddComponent(new CSpawnScript);
	pBlueSpawnPlace->Transform()->SetLocalPos(Vec3(1000.f, 0.f, 2000.f));
	pBlueSpawnPlace->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueSpawnPlace->FrustumCheck(false);
	pBlueSpawnPlace->GetScript<CSpawnScript>()->SetSpawnState(CAMP_STATE::BLUE);

	FindLayer(L"Blue")->AddGameObject(pBlueSpawnPlace);

	// 레드 넥서스
	CGameObject* pRedNexus;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\LowPoly_Nexus.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\LowPoly_Nexus.mdat", L"MeshData\\LowPoly_Nexus.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	pRedNexus = pMeshData->Instantiate();
	pRedNexus->AddComponent(new CCollider3D);
	pRedNexus->AddComponent(new CTowerScript);
	pRedNexus->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRedNexus->GetScript<CTowerScript>()->SetType(TOWER_TYPE::NEXUS);
	pRedNexus->Collider3D()->SetOffsetScale(Vec3(500.f, 500.f, 500.f));
	pRedNexus->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 250.f));
	pRedNexus->FrustumCheck(false);
	pRedNexus->Transform()->SetLocalPos(Vec3(1000.f, -10.f, 17600.f));
	pRedNexus->Transform()->SetLocalRot(Vec3(-PI/2, 0.f, 0.f));

	pRedNexus->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedNexus->MeshRender()->SetDynamicShadow(true);
	pRedNexus->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::RED);
	pRedNexus->GetScript<CTowerScript>()->Init();
	pRedNexus->GetScript<CTowerScript>()->SetFirstTower(pRedFirstTower);
	pRedNexus->GetScript<CTowerScript>()->SetSecondTower(pRedSecondTower);
	FindLayer(L"Red")->AddGameObject(pRedNexus);

	CGameObject* pRedSpawnPlace = new CGameObject;
	pRedSpawnPlace->SetName(L"Spawn_Place");
	pRedSpawnPlace->AddComponent(new CTransform);
	pRedSpawnPlace->AddComponent(new CSpawnScript);
	pRedSpawnPlace->FrustumCheck(false);
	pRedSpawnPlace->Transform()->SetLocalPos(Vec3(1000.f, 0.f, 17600.f));
	pRedSpawnPlace->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedSpawnPlace->GetScript<CSpawnScript>()->SetSpawnState(CAMP_STATE::RED);

	pBlueSpawnPlace->GetScript<CSpawnScript>()->SetEnemyNexus(pRedNexus);
	pRedSpawnPlace->GetScript<CSpawnScript>()->SetEnemyNexus(pBlueNexus);
	pBlueSpawnPlace->GetScript<CSpawnScript>()->SetFirstTower(pRedFirstTower);

	pRedSpawnPlace->GetScript<CSpawnScript>()->SetFirstTower(pBlueFirstTower);
	pBlueSpawnPlace->GetScript<CSpawnScript>()->SetSecondTower(pRedSecondTower);
	pRedSpawnPlace->GetScript<CSpawnScript>()->SetSecondTower(pBlueSecondTower);

	FindLayer(L"Red")->AddGameObject(pRedSpawnPlace);

//-----------------------------------------------------------------------------------------------
// 장애물
//-----------------------------------------------------------------------------------------------

	CGameObject* pObstacle = new CGameObject;
	Quaternion qRot;

	for (int i = 0; i < 8; ++i) {
	switch (i) {
		case 0:
			// R2 앞
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Obstacle01.mdat", L"MeshData\\Obstacle01.mdat");
			pObstacle = pMeshData->Instantiate();
			pObstacle->AddComponent(new CCollider3D);
			pObstacle->Transform()->SetLocalPos(Vec3(2725, 80.f, 13400.f));		// R2기준 (125, -1100)
			pObstacle->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObstacle->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 230.f));
			pObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 80.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-39.f));
			break;
		case 1:	
			// R1 앞
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Obstacle01.mdat", L"MeshData\\Obstacle01.mdat");
			pObstacle = pMeshData->Instantiate();
			pObstacle->AddComponent(new CCollider3D);
			pObstacle->Transform()->SetLocalPos(Vec3(-350.f, 80.f, 10600.f));		// R1 기준 (-300, -900)
			pObstacle->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObstacle->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 230.f));
			pObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 80.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-43.f)); 
			break;
		case 2:
			// B1 앞
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Obstacle01.mdat", L"MeshData\\Obstacle01.mdat");
			pObstacle = pMeshData->Instantiate();
			pObstacle->AddComponent(new CCollider3D);
			pObstacle->Transform()->SetLocalPos(Vec3(2500.f, 80.f, 9700.f));			// B1 기준 (500, 700)
			pObstacle->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObstacle->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 230.f));
			pObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 80.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-35.f)); 
			break;
		case 3:	
			// B2 앞
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Obstacle01.mdat", L"MeshData\\Obstacle01.mdat");
			pObstacle = pMeshData->Instantiate();
			pObstacle->AddComponent(new CCollider3D);
			pObstacle->Transform()->SetLocalPos(Vec3(-1100.f, 80.f, 6800.f));		// B2 기준 (-600, 1300)
			pObstacle->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObstacle->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 230.f));
			pObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 80.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(31.f));
			break;
		case 4:
			// R2 뒤
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\obstacle_02.mdat", L"MeshData\\obstacle_02.mdat");
			pObstacle = pMeshData->Instantiate();
			pObstacle->AddComponent(new CCollider3D);
			pObstacle->Transform()->SetLocalPos(Vec3(3000.f, 127.f, 15600.f));		// R2 기준 (525, 1100)
			pObstacle->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(3.f, 2.f, 2.f));
			pObstacle->Collider3D()->SetOffsetScale(Vec3(210.f, 120.f, 70.f));
			pObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, -55.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(75.f));
			break;
		case 5:
			// R1 뒤
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\obstacle_02.mdat", L"MeshData\\obstacle_02.mdat");
			pObstacle = pMeshData->Instantiate();
			pObstacle->AddComponent(new CCollider3D);
			pObstacle->Transform()->SetLocalPos(Vec3(-500.f, 127.f, 12700.f));		// R1 기준 (-500, 1200)
			pObstacle->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(3.f, 2.f, 2.f));
			pObstacle->Collider3D()->SetOffsetScale(Vec3(210.f, 120.f, 70.f));
			pObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, -55.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(72.f));
			break;
		case 6:
			// B1 뒤
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\obstacle_02.mdat", L"MeshData\\obstacle_02.mdat");
			pObstacle = pMeshData->Instantiate();
			pObstacle->AddComponent(new CCollider3D);
			pObstacle->Transform()->SetLocalPos(Vec3(2700.f, 127.f, 7700.f));		// B1 기준 (700, -1300)
			pObstacle->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(3.f, 2.f, 2.f));
			pObstacle->Collider3D()->SetOffsetScale(Vec3(210.f, 120.f, 70.f));
			pObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, -55.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(60.f));
			break;
		case 7:
			// B2 뒤
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\obstacle_02.mdat", L"MeshData\\obstacle_02.mdat");
			pObstacle = pMeshData->Instantiate();
			pObstacle->AddComponent(new CCollider3D);
			pObstacle->Transform()->SetLocalPos(Vec3(-1200.f, 127.f, 4700.f));		// B2 기준 (-675, -800)
			pObstacle->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(3.f, 2.f, 2.f));
			pObstacle->Collider3D()->SetOffsetScale(Vec3(210.f, 120.f, 70.f));
			pObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, -55.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-27.f));
			break;
		}
		pObstacle->SetName(L"obstacle");
		pObstacle->FrustumCheck(false);
		pObstacle->MeshRender()->SetDynamicShadow(true);
		pObstacle->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObstacle->Transform()->SetQuaternion(qRot);
		FindLayer(L"Obstacle")->AddGameObject(pObstacle);
	}


//-----------------------------------------------------------------------------------------------
// 맵 콜라이더
//-----------------------------------------------------------------------------------------------

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TransparencyMtrl");
	Ptr<CTexture> pTransparency = CResMgr::GetInst()->FindRes<CTexture>(L"Transparency");

	pObject = new CGameObject;
	pObject->SetName(L"Cover");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(550.f, 0.f, 40.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(4000.f, 200.f, 30.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->AddComponent(new CMeshRender);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	FindLayer(L"Cover")->AddGameObject(pObject);



	//
	pObject = new CGameObject;
	pObject->SetName(L"Cover");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);

	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(-1600, 0.f, 750.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(1700.f, 200.f, 1800.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->AddComponent(new CMeshRender);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	FindLayer(L"Cover")->AddGameObject(pObject);
	//
	//
	pObject = new CGameObject;
	pObject->SetName(L"Cover");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(3400.f, 0.f, 750.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(1700.f, 200.f, 1800.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->AddComponent(new CMeshRender);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	FindLayer(L"Cover")->AddGameObject(pObject);
	//
	//
	pObject = new CGameObject;
	pObject->SetName(L"Cover");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(-2450.f, 0.f, 9500.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(20.f, 200.f, 18000.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->AddComponent(new CMeshRender);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	FindLayer(L"Cover")->AddGameObject(pObject);
	//
	//
	pObject = new CGameObject;
	pObject->SetName(L"Cover");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(4300.f, 0.f, 9500.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(20.f, 200.f, 18000.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->AddComponent(new CMeshRender);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	FindLayer(L"Cover")->AddGameObject(pObject);
	//
	pObject = new CGameObject;
	pObject->SetName(L"Cover");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(550.f, 0.f, 19790.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(4000.f, 200.f, 30.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->AddComponent(new CMeshRender);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	FindLayer(L"Cover")->AddGameObject(pObject);



	//
	pObject = new CGameObject;
	pObject->SetName(L"Cover");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);

	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(-1600, 0.f, 19000));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(1700.f, 200.f, 1800.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->AddComponent(new CMeshRender);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	FindLayer(L"Cover")->AddGameObject(pObject);
	//
	//
	pObject = new CGameObject;
	pObject->SetName(L"Cover");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(3400.f, 0.f, 19000));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(1700.f, 200.f, 1800.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->AddComponent(new CMeshRender);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	FindLayer(L"Cover")->AddGameObject(pObject);

//-----------------------------------------------------------------------------------------------
// 스카이박스
//-----------------------------------------------------------------------------------------------
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->FindRes<CTexture>(L"Sky01");

	pObject = new CGameObject;
	pObject->SetName(L"SkyBox");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->FrustumCheck(false);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky01.GetPointer());

	FindLayer(L"Default")->AddGameObject(pObject);

//-----------------------------------------------------------------------------------------------
// UI
//-----------------------------------------------------------------------------------------------

	// UI Camera
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICam");
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);
	pUICam->AddComponent(new CCameraScript);
	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHGRAPHIC);

	pUICam->Camera()->SetFar(1000.f);
	pUICam->Camera()->SetLayerCheck(30, true);

	pUICam->Camera()->SetWidth(CRenderMgr::GetInst()->GetResolution().fWidth);
	pUICam->Camera()->SetHeight(CRenderMgr::GetInst()->GetResolution().fHeight);
	FindLayer(L"Default")->AddGameObject(pUICam, false);

	// 크로스헤어
	CGameObject* pUICrossHair = new CGameObject;
	pUICrossHair->SetName(L"UICrossHair");
	pUICrossHair->FrustumCheck(false);
	pUICrossHair->AddComponent(new CTransform);
	pUICrossHair->AddComponent(new CMeshRender);
	pUICrossHair->AddComponent(new CCrossHairScript);

	tResolution res = CRenderMgr::GetInst()->GetResolution();
	Vec3 vScale = Vec3(res.fWidth / 13, res.fWidth/13, 1.f);

	pUICrossHair->Transform()->SetLocalPos(Vec3(0.f, res.fHeight/4, 1.f));
	pUICrossHair->Transform()->SetLocalScale(vScale);

	pUICrossHair->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pCrossHairMtrl = new CMaterial;
	pCrossHairMtrl->DisableFileSave();
	pCrossHairMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	pUICrossHair->MeshRender()->SetMaterial(pCrossHairMtrl);
	pUICrossHair->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl"));
	Ptr<CTexture> pCrossHairTex = CResMgr::GetInst()->FindRes<CTexture>(L"BaseLine");
	pUICrossHair->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pCrossHairTex.GetPointer());

	pUICrossHair->GetScript<CCrossHairScript>()->SetMainCam(pMainCam);
	pUICrossHair->GetScript<CCrossHairScript>()->Init();

	FindLayer(L"UI")->AddGameObject(pUICrossHair);


	CGameObject* pDarkUI = new CGameObject;
	pDarkUI->SetName(L"UIDark");
	pDarkUI->FrustumCheck(false);
	pDarkUI->AddComponent(new CTransform);
	pDarkUI->AddComponent(new CMeshRender);

	pDarkUI->Transform()->SetLocalPos(Vec3(0.f, 0.f, 1.f));
	pDarkUI->Transform()->SetLocalScale(Vec3(res.fWidth, res.fHeight, 1.f));
	pDarkUI->SetActive(false);

	pDarkUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pDarkUIMtrl = new CMaterial;
	pDarkUIMtrl->DisableFileSave();
	pDarkUIMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	pDarkUI->MeshRender()->SetMaterial(pDarkUIMtrl);
	Ptr<CTexture> pDarkUITex = CResMgr::GetInst()->FindRes<CTexture>(L"DarkUITex");
	pDarkUI->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pDarkUITex.GetPointer());

	FindLayer(L"UI")->AddGameObject(pDarkUI);


//-----------------------------------------------------------------------------------------------
// Arrow Test
//-----------------------------------------------------------------------------------------------


	CGameObject* m_pArrow = new CGameObject;
	m_pArrow->SetName(L"Arrow");


	m_pArrow->AddComponent(new CTransform);
	m_pArrow->Transform()->SetLocalPos(Vec3(-50.f, 50.f, -50.f));
	m_pArrow->Transform()->SetLocalScale(Vec3(100.f, 1.f, 1.f));
	m_pArrow->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	m_pArrow->AddComponent(new CMeshRender);
	m_pArrow->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	m_pArrow->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//	m_pArrow[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlackTex.GetPointer());

	m_pArrow->AddComponent(new CCollider3D);
	m_pArrow->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);

	m_pArrow->AddComponent(new CArrowScript(ELEMENT_TYPE::WATER));
	m_pArrow->GetScript<CArrowScript>()->SetMove(false);
	m_pArrow->GetScript<CArrowScript>()->SetSkill(CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::WATER_0));
	m_pArrow->GetScript<CArrowScript>()->SetLayerIdx(3);
	//m_pArrow->GetScript<CArrowScript>()->SetPlayer(pPlayer);
	//pPlayer
	FindLayer(L"Arrow")->AddGameObject(m_pArrow);


//-----------------------------------------------------------------------------------------------
// 파티창 
//-----------------------------------------------------------------------------------------------
	
	Vec3 vPlayerTexScale = Vec3(res.fWidth / 25.f, res.fHeight / 14.f, 1.f);
	Vec3 vHPBarScale = Vec3(res.fWidth / 15.f, res.fHeight / 44.f, 1.f);
	CGameObject* pPlayerInfoTex;
	CGameObject* pHpBar;

	for (int i = 0; i < 3; ++i) {
		// 사진
		pPlayerInfoTex = new CGameObject;
		pPlayerInfoTex->SetName(L"PlayerInfoTex");
		pPlayerInfoTex->FrustumCheck(false);
		pPlayerInfoTex->AddComponent(new CTransform);
		pPlayerInfoTex->AddComponent(new CMeshRender);

		pPlayerInfoTex->Transform()->SetLocalPos(Vec3(-res.fWidth / 2.f + vPlayerTexScale.x / 2 + 10.f, 0.f + vPlayerTexScale.y/2 - res.fHeight/11 * i, 1.f));
		pPlayerInfoTex->Transform()->SetLocalScale(vPlayerTexScale);

		pPlayerInfoTex->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
		Ptr<CMaterial> pPlayerTexMtrl = new CMaterial;
		pPlayerTexMtrl->DisableFileSave();
		pPlayerTexMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
		pPlayerInfoTex->MeshRender()->SetMaterial(pPlayerTexMtrl);
		int iType = pPlayer->GetScript<CPlayerScript>()->GetType();
		Ptr<CTexture> pPlayerTexTex;

		// 주작용
		switch (i) {
		case 0:
			pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_01_Blue");
			break;
		case 1:
			pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_02_Blue");
			break;
		case 2:
			pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_04_Blue");
			break;
		}

		//if (pPlayer->GetLayerIdx() == 3) {
		//	switch (iType) {
		//	case 0:
		//		pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_01_Blue");
		//		break;
		//	case 1:
		//		pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_02_Blue");
		//		break;
		//	case 2:
		//		pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_03_Blue");
		//		break;
		//	case 3:
		//		pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_04_Blue");
		//		break;													
		//	case 4:														
		//		pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_05_Blue");
		//		break;													
		//																
		//	}															
		//}																
		//else if (pPlayer->GetLayerIdx() == 4) {							
		//	switch (iType) {											
		//	case 0:														
		//		pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_01_Red");
		//		break;													
		//	case 1:														
		//		pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_02_Red");
		//		break;												
		//	case 2:													
		//		pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_03_Red");
		//		break;													
		//	case 3:														
		//		pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_04_Red");
		//		break;												
		//	case 4:													
		//		pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_05_Red");
		//		break;
		//	}
		//}
		pPlayerInfoTex->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pPlayerTexTex.GetPointer());
		FindLayer(L"UI")->AddGameObject(pPlayerInfoTex);

		// 플레이어 id
		//CFontMgr::GetInst()->AddText(wstring(L"PlayerID") + to_wstring(i), wstring(L"PlayerID") + to_wstring(i), Vec2(0.05f, 0.44f + 0.09f * i), Vec2(1.5f, 1.5f), Vec2(0.5f, 0.f), Vec4(1.f, 0.f, 1.f, 1.f));

		// 주작용
		switch (i) {
		case 0:
			CFontMgr::GetInst()->AddText(wstring(L"PlayerID") + to_wstring(i), L"sangjun", Vec2(0.05f, 0.44f + 0.09f * i), Vec2(1.5f, 1.5f), Vec2(0.5f, 0.f), Vec4(1.f, 0.f, 1.f, 1.f));
			break;
		case 1:
			CFontMgr::GetInst()->AddText(wstring(L"PlayerID") + to_wstring(i), L"dongsu", Vec2(0.05f, 0.44f + 0.09f * i), Vec2(1.5f, 1.5f), Vec2(0.5f, 0.f), Vec4(1.f, 0.f, 1.f, 1.f));
			break;
		case 2:
			CFontMgr::GetInst()->AddText(wstring(L"PlayerID") + to_wstring(i), L"sumin", Vec2(0.05f, 0.44f + 0.09f * i), Vec2(1.5f, 1.5f), Vec2(0.5f, 0.f), Vec4(1.f, 0.f, 1.f, 1.f));
			break;
		}


		// 에이치피바
		pHpBar = new CGameObject;
		pHpBar->SetName(wstring(L"UI HPBar ") + to_wstring(i));
		pHpBar->AddComponent(new CTransform);
		pHpBar->AddComponent(new CMeshRender);
		pHpBar->FrustumCheck(false);
		pHpBar->MeshRender()->SetDynamicShadow(false);
		
		pHpBar->Transform()->SetLocalScale(vHPBarScale);
		pHpBar->Transform()->SetLocalPos(Vec3(-res.fWidth / 2 + vHPBarScale.x / 2 + vPlayerTexScale.x + 15.f, 0.f + vPlayerTexScale.y / 2 - res.fHeight / 11 * i - 15.f, 1.f));
		pHpBar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pUIHPBarMtrl = new CMaterial;
		pUIHPBarMtrl->DisableFileSave();
		pUIHPBarMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"UIHPBarShader"));
		pHpBar->MeshRender()->SetMaterial(pUIHPBarMtrl);
		pPlayer->GetScript<CPlayerScript>()->AddUIHpBarObj(pHpBar);
		FindLayer(L"UI")->AddGameObject(pHpBar);
	}

//-----------------------------------------------------------------------------------------------
// 조명 추가
//----------------------------------------------------------------------0-------------------------

	for (int i = 0; i < 2; i++) {
		pObject = new CGameObject;
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CLight3D);
		pObject->Light3D()->SetLightPos(Vec3(1000.f + 2000.f * i, 1000.f, -1500.f));
		pObject->Light3D()->SetLightType(LIGHT_TYPE::DIR);
		pObject->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
		pObject->Light3D()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
		pObject->Light3D()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		pObject->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
		pObject->Light3D()->SetLightRange(1000.f);
		FindLayer(L"Default")->AddGameObject(pObject);
	}	

//-----------------------------------------------------------------------------------------------
// 충돌 처리
//-----------------------------------------------------------------------------------------------

	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Blue");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Red");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Red");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Cover");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Cover");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Arrow");

	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Arrow");

	
	CSensorMgr::GetInst()->CheckSensorLayer(L"Blue", L"Red");
	CSensorMgr::GetInst()->CheckSensorLayer(L"Blue", L"Blue");
	CSensorMgr::GetInst()->CheckSensorLayer(L"Red", L"Red");
}

void CInGameScene::Exit()
{
	for (int i = 0; i < 3; ++i) {
		CFontMgr::GetInst()->DeleteText(wstring(L"PlayerID") + to_wstring(i));
	}
}