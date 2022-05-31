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
#include "EmptyCameraScript.h"
#include "StaticUI.h"
#include "CrossHairScript.h"

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
	GetLayer(8)->SetName(L"Tile");
	GetLayer(30)->SetName(L"UI");
	GetLayer(31)->SetName(L"Tool");

	CGameObject* pMainCam = nullptr;

	// Camera Object
	pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	//pMainCam->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pMainCam->Camera()->SetFar(10000.f);
	pMainCam->Camera()->SetLayerAllCheck();
	CCollisionMgr::GetInst()->SetCameraObject(pMainCam->Camera());

	CGameObject* pEmptyCam = nullptr;
	pEmptyCam = new CGameObject;
	pEmptyCam->SetName(L"EmptyCam");
	pEmptyCam->AddComponent(new CTransform);
	pEmptyCam->AddComponent(new CCamera);
	pEmptyCam->AddComponent(new CEmptyCameraScript);
	pEmptyCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	//m_pCurScene->FindLayer(L"Camera")->AddGameObject(pMainCam);

	CGameObject* pObject = nullptr;

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



	pObject = new CGameObject;
	pObject->SetName(L"Tile");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	// Transform ����
	pObject->Transform()->SetLocalPos(Vec3(0.f, -1.f, 2300.f));
	pObject->Transform()->SetLocalScale(Vec3(2000.f, 10000.f, 1.f));
	pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f,0.f));

	// MeshRender ����
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());
	pObject->MeshRender()->SetDynamicShadow(true);

	// AddGameObject
	FindLayer(L"Tile")->AddGameObject(pObject);

	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player_without_bow01.mdat", L"MeshData\\player_without_bow01.mdat");


	pObject = pMeshData->Instantiate();
	pObject->SetName(L"Monster");
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CSensor);
	pObject->AddComponent(new CPlayerScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(80.f, 80.f, 200.f));      // 80.f, 200.f, 80.f ?????
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(0.4f, 0.4f, 0.5f));
	pObject->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
	pObject->MeshRender()->SetDynamicShadow(true);
	pObject->Sensor()->SetRadius(500.f);
	pObject->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::WATER);
	pMainCam->GetScript<CCameraScript>()->SetDistanceOffset(pObject);
	pObject->GetScript<CPlayerScript>()->SetCurHp(50);
	pMainCam->Transform()->SetLocalPos(Vec3(0.f, 100.f, 130.f));
	pMainCam->Camera()->SetPlayer(pObject);
	pMainCam->Camera()->SetbPlay(true);
	pObject->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::FROZEN);

	// Specular
	/*int isSpecular = 1;
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_1, &isSpecular);*/
	
	//pMainCam->Transform()->SetLocalPos(Vec3(0.f, 100.f, 130.f));

	CAnimation* pNewAnimation = new CAnimation;
	pNewAnimation->InsertAnimClip(L"IDLE", 0, 37);
	pNewAnimation->InsertAnimClip(L"WALK", 44, 73);         // 45, 69
	pNewAnimation->InsertAnimClip(L"JUMP", 81, 108); // ���� �� �ȹ����� 81, 125
	pNewAnimation->InsertAnimClip(L"ATTACK_READY", 145, 167);      ///145 167
	pNewAnimation->InsertAnimClip(L"ATTACK", 168, 175); // 168 175
	pNewAnimation->InsertAnimClip(L"DEMAGED", 231, 242);
	pNewAnimation->InsertAnimClip(L"DIE", 242, 269);      // ������ �� 240, 261
	pNewAnimation->InsertAnimClip(L"RUN", 298, 320);      // 305, 320
	//pNewAnimation->InsertAnimation(L"DIE", 269, 289, false, false);

	pObject->Animator3D()->SetAnimation(pNewAnimation);
	pObject->Animator3D()->SetAnimClip(pNewAnimation->GetAnimClip());
	pObject->GetScript<CPlayerScript>()->Init();

	FindLayer(L"Blue")->AddGameObject(pObject, false);

	CGameObject* pEmptyPlayer = new CGameObject;
	pEmptyPlayer->AddComponent(new CTransform);		
	pEmptyPlayer->AddComponent(new CEmptyPlayerScript);
	pEmptyPlayer->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(0.f), 0.f));

	pMainCam->Transform()->SetLocalPos(Vec3(-300, 130, -50));
	pMainCam->Transform()->SetLocalRot(Vec3(0, XMConvertToRadians(90.f), XMConvertToRadians(-15.f)));

	pEmptyCam->Transform()->SetLocalPos(Vec3(-300, 130, -50));
	pEmptyCam->Transform()->SetLocalRot(Vec3(0, XMConvertToRadians(90.f), XMConvertToRadians(0.f)));

	pEmptyPlayer->AddChild(pMainCam);
	pEmptyPlayer->AddChild(pEmptyCam);
	FindLayer(L"Default")->AddGameObject(pEmptyPlayer, false);


	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\bow_big.mdat", L"MeshData\\bow_big.mdat");
	CGameObject* pBow;
	pBow = pMeshData->Instantiate();
	pBow->SetName(L"Bow");

	pBow->AddComponent(new CTransform);
	pBow->AddComponent(new CCollider3D);
	pBow->AddComponent(new CSensor);
	pBow->AddComponent(new CBowScript);
	pBow->FrustumCheck(false);
	pBow->Transform()->SetLocalPos(Vec3(0.0f, 0.0f, 0.0f));
	pBow->Transform()->SetLocalScale(Vec3(2.4f, 1.2f, 2.4f));
	pBow->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, 0.0f));
	pBow->MeshRender()->SetDynamicShadow(false);
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

	FindLayer(L"Blue")->AddGameObject(pBow);


	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\SecondTower.mdat", L"MeshData\\SecondTower.mdat");
	CGameObject* pRedFirstTower;
	pRedFirstTower = pMeshData->Instantiate();
	pRedFirstTower->SetName(L"FirstTower");
	pRedFirstTower->AddComponent(new CCollider3D);
	pRedFirstTower->AddComponent(new CSensor);
	pRedFirstTower->AddComponent(new CTowerScript);
	pRedFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
	pRedFirstTower->Sensor()->SetRadius(300.f);
	pRedFirstTower->Transform()->SetLocalPos(Vec3(200.f, 0.f, 2700.f));
	///pRedFirstTower->Transform()->SetLocalRot(Vec3(0.f, 3.14f/4, 0.f));
	pRedFirstTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedFirstTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRedFirstTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pRedFirstTower->Collider3D()->SetOffsetPos(Vec3(0.f, 110.f, 25.f));
	pRedFirstTower->FrustumCheck(false);
	pRedFirstTower->GetScript<CTowerScript>()->Init();
	pRedFirstTower->MeshRender()->SetDynamicShadow(false);



	FindLayer(L"Red")->AddGameObject(pRedFirstTower);







	//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\SecondTower.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\SecondTower.mdat", L"MeshData\\SecondTower.mdat");
	CGameObject* pRedSecondTower;
	pRedSecondTower = pMeshData->Instantiate();
	pRedSecondTower->SetName(L"SecondTower");
	pRedSecondTower->AddComponent(new CCollider3D);
	pRedSecondTower->AddComponent(new CSensor);
	pRedSecondTower->AddComponent(new CTowerScript);
	pRedSecondTower->Sensor()->SetRadius(300.f);
	pRedSecondTower->Transform()->SetLocalPos(Vec3(-200.f, 0.f, 3550.f));
	pRedSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
	pRedSecondTower->GetScript<CTowerScript>()->SetFirstTower(pRedFirstTower);
	pRedSecondTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedSecondTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRedSecondTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pRedSecondTower->Collider3D()->SetOffsetPos(Vec3(0.f, 110.f, 25.f));
	pRedSecondTower->FrustumCheck(false);
	pRedSecondTower->GetScript<CTowerScript>()->Init();
	pRedSecondTower->MeshRender()->SetDynamicShadow(true);


	FindLayer(L"Red")->AddGameObject(pRedSecondTower);

	//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\FirstTower01.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\SecondTower01.mdat", L"MeshData\\SecondTower01.mdat");
	//	pMeshData->Save(pMeshData->GetPath());
	CGameObject* pBlueFirstTower;
	pBlueFirstTower = pMeshData->Instantiate();
	pBlueFirstTower->AddComponent(new CCollider3D);
	pBlueFirstTower->AddComponent(new CSensor);
	pBlueFirstTower->AddComponent(new CTowerScript);
	pBlueFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
	pBlueFirstTower->Sensor()->SetRadius(300.f);
	pBlueFirstTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pBlueFirstTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pBlueFirstTower->Collider3D()->SetOffsetPos(Vec3(0.f, 110.f, 25.f));
	pBlueFirstTower->FrustumCheck(false);
	pBlueFirstTower->Transform()->SetLocalPos(Vec3(-200.f, 0.f, 1850.f));
	pBlueFirstTower->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pBlueFirstTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueFirstTower->MeshRender()->SetDynamicShadow(false);
	pBlueFirstTower->GetScript<CTowerScript>()->Init();
	FindLayer(L"Blue")->AddGameObject(pBlueFirstTower);




	





	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\SecondTower01.mdat", L"MeshData\\SecondTower01.mdat");
	CGameObject* pBlueSecondTower;
	pBlueSecondTower = pMeshData->Instantiate();
	pBlueSecondTower->AddComponent(new CCollider3D);
	pBlueSecondTower->AddComponent(new CSensor);
	pBlueSecondTower->AddComponent(new CTowerScript);
	pBlueSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
	pBlueSecondTower->Sensor()->SetRadius(300.f);
	pBlueSecondTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pBlueSecondTower->GetScript<CTowerScript>()->SetFirstTower(pBlueFirstTower);
	pBlueSecondTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
	pBlueSecondTower->Collider3D()->SetOffsetPos(Vec3(0.f, 110.f, 25.f));
	pBlueSecondTower->FrustumCheck(false);
	pBlueSecondTower->Transform()->SetLocalPos(Vec3(200.f, 0.f, 1000.f));
	pBlueSecondTower->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pBlueSecondTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueSecondTower->MeshRender()->SetDynamicShadow(false);
	pBlueSecondTower->GetScript<CTowerScript>()->Init();
	FindLayer(L"Blue")->AddGameObject(pBlueSecondTower);

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\castle007.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\castle007.mdat", L"MeshData\\castle007.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	
	pObject = pMeshData->Instantiate();

	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(0.f, 370.f, 0.f));
//	pObject->Transform()->SetLocalPos(Vec3(0.f, -50.f, 0.f));
	pObject->Transform()->SetLocalRot(Vec3(-PI / 2, PI / 2, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(4.f, 4.f, 4.f));
	pObject->MeshRender()->SetDynamicShadow(true);

// 맵 머테리얼==========================================
//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Red"));
//	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow");
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Blue"));
//	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow");
///	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"RedTransparent"));
//	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow");
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BlueTransparent"));
//	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow");
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MetalSheetRoofRed"));
//	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow");
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"AsphaltStoneRedBaseTrim"));
//	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow");
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"AsphaltStoneBlueBaseTrim"));
//	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow"); 
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());
//
//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Lighy_Gray"));
//	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow");
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Dark_Gray"));
//	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow");
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());
//
//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MetalSheetRoofBlue"));
//	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow");
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Gray"));
//	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow");
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());
//
//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Black"));
//	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Snow");
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());


	FindLayer(L"Default")->AddGameObject(pObject);


	


	CGameObject* pNexus = nullptr;


	//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Nexus01.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Nexus01.mdat", L"MeshData\\Nexus01.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));

	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(50.f, 35.f, 150.f));
	pObject->Transform()->SetLocalRot(Vec3(-3.14f / 6, 0.F, 0.f));

	pObject->Transform()->SetLocalScale(Vec3(70.f, 70.f, 70.f));
	pObject->MeshRender()->SetDynamicShadow(false);

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

	FindLayer(L"Blue")->AddGameObject(pObject);





	//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Nexus.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Nexus.mdat", L"MeshData\\Nexus.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(50.f, 35.f, 4400.f));
	pObject->Transform()->SetLocalRot(Vec3(-3.14f / 6, 3.14f, 0.f));

	pObject->Transform()->SetLocalScale(Vec3(70.f, 70.f, 70.f));
	pObject->MeshRender()->SetDynamicShadow(false);
	pNexus = pObject;
	FindLayer(L"Red")->AddGameObject(pObject);


	pNexus = pObject;
	pObject = new CGameObject;
	pObject->SetName(L"Spawn_Place");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CSpawnScript);
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(-50.f, 0.f, 400.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	pObject->GetScript<CSpawnScript>()->SetSpawnState(CAMP_STATE::BLUE);
	pObject->GetScript<CSpawnScript>()->SetEnemyNexus(pNexus);

	FindLayer(L"Default")->AddGameObject(pObject);







//	 pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\wizard_min.mdat", L"MeshData\\wizard_min.mdat");
//	pObject = pMeshData->Instantiate();
//	pObject->AddComponent(new CTransform);
//	pObject->AddComponent(new CCollider3D);
//	pObject->AddComponent(new CMinionScript);
//
//	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
//	pObject->Collider3D()->SetOffsetScale(Vec3(60.f, 100.f, 60.f));
//	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 10.f, 0.f));
//	pObject->FrustumCheck(false);
//	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
//	pObject->Transform()->SetLocalScale(Vec3(0.3f, 0.3f, 0.3f));
//	pObject->MeshRender()->SetDynamicShadow(true);
//
//	pNewAnimation = new CAnimation;
//	pNewAnimation->InsertAnimation(L"IDLE", 0, 34, true, false);
//	pNewAnimation->InsertAnimation(L"WALK", 36, 59, false, false);
//	pNewAnimation->InsertAnimation(L"ATTACK", 60, 84, false, false);
//	pNewAnimation->InsertAnimation(L"DIE", 110, 159, false, false);
//	pObject->Animator3D()->SetAnimation(pNewAnimation);
//	pObject->Animator3D()->SetAnimationSpeed(2.f);
//	
//	pObject->GetScript<CMinionScript>()->SetAttackType(MINION_ATTACK_TYPE::RANGE);
//	pObject->GetScript<CMinionScript>()->SetCamp(CAMP_STATE::BLUE);
//	pObject->GetScript<CMinionScript>()->Init();
//	pObject->GetScript < CMinionScript>()->SetState(MINION_STATE::ATTACK);
//	FindLayer(L"Blue")->AddGameObject(pObject);



	/* {
		pObject = new CGameObject;
		pObject->SetName(L"Tree");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->Transform()->SetLocalPos(Vec3(-200.f, 0.f, 150.f));
		pObject->Transform()->SetLocalScale(Vec3(100.f, 150.f, 1.f));
		pObject->Transform()->SetBillBoard(true);
		pObject->Transform()->SetCamera(pMainCam);
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
		CResMgr::GetInst()->AddRes(L"Tree1Mtrl1", pMtrl);
		pObject->MeshRender()->SetMaterial(pMtrl);
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTreeTex1.GetPointer());

		FindLayer(L"Default")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"Tree");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->Transform()->SetLocalPos(Vec3(-250.f, 0.f, 150.f));
		pObject->Transform()->SetLocalScale(Vec3(100.f, 100.f, 1.f));
		pObject->Transform()->SetBillBoard(true);
		pObject->Transform()->SetCamera(pMainCam);
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl"));
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
		CResMgr::GetInst()->AddRes(L"Tree1Mtrl2", pMtrl);
		pObject->MeshRender()->SetMaterial(pMtrl);
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTreeTex2.GetPointer());

		pObject->Transform()->SetLocalScale(Vec3(100.f, 100.f, 1.f));
		FindLayer(L"Default")->AddGameObject(pObject);

	}*/

	Ptr<CMaterial>pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TransparencyMtrl");
	Ptr<CTexture> pTransparency = CResMgr::GetInst()->FindRes<CTexture>(L"Transparency");
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->FindRes<CTexture>(L"Sky01");
	{

		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(287.f, 0.f, 578.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(25.f, 300.f, 25.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 150.f, 0.f));
	//	pObject->AddComponent(new CMeshRender);
	//	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//	pObject->MeshRender()->SetMaterial(pMtrl);
	//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());

		FindLayer(L"Cover")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(287.f, 100.f, 578.f + 193.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(25.f, 300.f, 25.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 150.f, 0.f));

		FindLayer(L"Cover")->AddGameObject(pObject);


		for (int i = 0; i < 3; ++i) {



			pObject = new CGameObject;
			pObject->SetName(L"Cover");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CCollider3D);
		
			if (i == 2) {
				pObject->Transform()->SetLocalPos(Vec3(287.f, 100.f, 964.f + (i * 190)));
				pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 150.f, 0.f));
			}
			else {
				pObject->Transform()->SetLocalPos(Vec3(287.f, 0.f, 964.f + (i * 190)));
				pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 150.f, 0.f));
			}

			pObject->FrustumCheck(false);
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObject->Collider3D()->SetOffsetScale(Vec3(25.f, 300.f, 25.f));

		

			FindLayer(L"Cover")->AddGameObject(pObject);



		}


		for (int i = 0; i < 5; ++i) {


			pObject = new CGameObject;
			pObject->SetName(L"Cover");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CCollider3D);
		
			pObject->FrustumCheck(false);
			pObject->Transform()->SetLocalPos(Vec3(-293.f, 0.f, 578.f + (i * 192.5)));
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObject->Collider3D()->SetOffsetScale(Vec3(22.5f, 300.f, 22.5f));
			pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 150.f, 0.f));
		

			FindLayer(L"Cover")->AddGameObject(pObject);

			pObject = new CGameObject;
			pObject->SetName(L"Cover");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CCollider3D);
			
			pObject->FrustumCheck(false);
			pObject->Transform()->SetLocalPos(Vec3(287.f, 0.f, 3986.f - (i * 192.5)));
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObject->Collider3D()->SetOffsetScale(Vec3(22.5f, 300.f, 22.5f));
			pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 150.f, 0.f));
		

			FindLayer(L"Cover")->AddGameObject(pObject);


			pObject = new CGameObject;
			pObject->SetName(L"Cover");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CCollider3D);
			
			pObject->FrustumCheck(false);
			if (i == 1 || i == 4) {
				pObject->Transform()->SetLocalPos(Vec3(-293.f, 100.f, 3986.f - (i * 192.5)));
				pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 150.f, 0.f));
			}
			else {
				pObject->Transform()->SetLocalPos(Vec3(-293.f, 0.f, 3986.f - (i * 192.5)));
				pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 150.f, 0.f));
			}

			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObject->Collider3D()->SetOffsetScale(Vec3(22.5f, 300.f, 22.5f));

		

			FindLayer(L"Cover")->AddGameObject(pObject);
		}



		for (int i = 0; i < 14; ++i) {
			pObject = new CGameObject;
			pObject->SetName(L"Cover");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CCollider3D);
		
			pObject->FrustumCheck(false);
			pObject->Transform()->SetLocalPos(Vec3(-510.f, 0.f, 300.f * (i + 1)));
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 300.f));
			pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
			

			FindLayer(L"Cover")->AddGameObject(pObject);


			pObject = new CGameObject;
			pObject->SetName(L"Cover");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CCollider3D);
		
			pObject->FrustumCheck(false);
			pObject->Transform()->SetLocalPos(Vec3(510.f, 0.f, 300.f * (i + 1)));
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 300.f));
			pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
			

			FindLayer(L"Cover")->AddGameObject(pObject);
		}

		for (int i = 0; i < 4; ++i) {
			pObject = new CGameObject;
			pObject->SetName(L"Cover");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CCollider3D);
			
			pObject->FrustumCheck(false);
			pObject->Transform()->SetLocalPos(Vec3(-135.f, 0.f, -120.f-(i*295)));
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 300.f));
			pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
			

			FindLayer(L"Cover")->AddGameObject(pObject);
		
			pObject = new CGameObject;
			pObject->SetName(L"Cover");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CCollider3D);
			
			pObject->FrustumCheck(false);
			pObject->Transform()->SetLocalPos(Vec3(135.f, 0.f, -120.f - (i * 295)));
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 300.f));
			pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		

			FindLayer(L"Cover")->AddGameObject(pObject);
		}
	


		
		


		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
		
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-225.f, 0.f, -40.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(200.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		
		FindLayer(L"Cover")->AddGameObject(pObject);


		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(225.f, 0.f, -40.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(200.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		

		FindLayer(L"Cover")->AddGameObject(pObject);


		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
		
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-420.f, 0.f, 200.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 200.f, 400.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	

		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-420.f, 0.f, 200.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 200.f, 400.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	

		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(360.f, 0.f, 300.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(200.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	

		FindLayer(L"Cover")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-360.f, 0.f, 300.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(200.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		

		FindLayer(L"Cover")->AddGameObject(pObject);


		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(360.f, 0.f, 200.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(150.f, 200.f, 400.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-360.f, 0.f, 200.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(150.f, 200.f, 400.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
		
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(330.f, 0.f, -1250.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 400.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-300.f, 0.f, -1250.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 400.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);


		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-200.f, 0.f, -1100.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(250.f, 0.f, -1100.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		
		FindLayer(L"Cover")->AddGameObject(pObject);


		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
		
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(0, 0.f, -1500.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(600.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		
		FindLayer(L"Cover")->AddGameObject(pObject);




		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
		
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-105.f, 50.f, -1330.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(50.f, 200.f, 50.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));

		FindLayer(L"Cover")->AddGameObject(pObject);




		



	}



	{
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);

		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-360.f, 0.f, 4200.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(200.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);
		for (int i = 0; i < 4; ++i) {

			pObject = new CGameObject;
			pObject->SetName(L"Cover");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CCollider3D);
		
			pObject->FrustumCheck(false);
			pObject->Transform()->SetLocalPos(Vec3(-125.f, 0.f, 4690.f+(i*300)));
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 300.f));
			pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		
			FindLayer(L"Cover")->AddGameObject(pObject);


			pObject = new CGameObject;
			pObject->SetName(L"Cover");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CCollider3D);
		
			pObject->FrustumCheck(false);
			pObject->Transform()->SetLocalPos(Vec3(125.f, 0.f, 4690.f+(i*300)));
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 300.f));
			pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		
			FindLayer(L"Cover")->AddGameObject(pObject);
		}

		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-225.f, 0.f, 4590.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(200.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);


		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(225.f, 0.f, 4590.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(200.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(360.f, 0.f, 4200.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(200.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));

		FindLayer(L"Cover")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-420.f, 0.f, 4200.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 200.f, 400.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));

		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-420.f, 0.f, 4200.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 200.f, 400.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(310.f, 0.f, 4350.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 400.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		
		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-310.f, 0.f, 4350.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 400.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(0, 0.f, 6050.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(600.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-300.f, 0.f, 5950.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 400.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(330.f, 0.f, 5950.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 300.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(-200.f, 0.f, 5700.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);
		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(250.f, 0.f, 5700.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 200.f, 100.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	
		FindLayer(L"Cover")->AddGameObject(pObject);


		pObject = new CGameObject;
		pObject->SetName(L"Cover");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CCollider3D);
	
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(95.f, 50.f, 5900.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pObject->Collider3D()->SetOffsetScale(Vec3(50.f, 200.f, 50.f));
		pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		
		FindLayer(L"Cover")->AddGameObject(pObject);


	}



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


	// UI

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


	CGameObject* pUICrossHair = new CGameObject;
	pUICrossHair->SetName(L"UICrossHair");
	pUICrossHair->FrustumCheck(false);
	pUICrossHair->AddComponent(new CTransform);
	pUICrossHair->AddComponent(new CMeshRender);
	pUICrossHair->AddComponent(new CStaticUI);
	pUICrossHair->AddComponent(new CCrossHairScript);

	Vec3 vScale = Vec3(100.f, 100.f, 1.f);
	tResolution res = CRenderMgr::GetInst()->GetResolution();

	//Vec3((-res.fWidth / 2, res.fHeight/2, 1.f) -> �»��, vScale/2 ��ŭ ���������
	//pUICrossHair->Transform()->SetLocalPos(Vec3((-res.fWidth / 2 + vScale.x/2), (res.fHeight/2 -vScale.y/2), 1.f));
	pUICrossHair->Transform()->SetLocalPos(Vec3((-vScale.x / 2), (vScale.y / 2), 1.f));
	pUICrossHair->Transform()->SetLocalScale(vScale);
	pUICrossHair->StaticUI()->SetCamera(pUICam->Camera());

	pUICrossHair->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUICrossHair->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl"));
	Ptr<CTexture> pCrossHairTex = CResMgr::GetInst()->FindRes<CTexture>(L"BaseLine");
	pUICrossHair->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pCrossHairTex.GetPointer());

	//pMainCam->Transform()->SetLocalRot(Vec3(0, XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	pUICrossHair->GetScript<CCrossHairScript>()->SetMainCam(pEmptyCam);

	//pMainCam->Transform()->SetLocalRot(Vec3(0, XMConvertToRadians(90.f), XMConvertToRadians(-15.f)));

	//pMainCam->AddChild(pUICrossHair);
	//FindLayer(L"UI")->AddGameObject(pMainCam, false);
	FindLayer(L"UI")->AddGameObject(pUICrossHair);




	CGameObject* m_pArrow = new CGameObject;
	m_pArrow->SetName(L"Arrow");


	m_pArrow->AddComponent(new CTransform);
	m_pArrow->Transform()->SetLocalPos(Vec3(50.f, 50.f, 50.f));
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
	FindLayer(L"Arrow")->AddGameObject(m_pArrow);




	m_pArrow = new CGameObject;
	m_pArrow->SetName(L"Arrow");

	m_pArrow->AddComponent(new CTransform);
	m_pArrow->Transform()->SetLocalPos(Vec3(-10.f, 50.f, -10.f));
	m_pArrow->Transform()->SetLocalScale(Vec3(100.f, 1.f, 1.f));
	m_pArrow->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	m_pArrow->AddComponent(new CMeshRender);
	m_pArrow->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	m_pArrow->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//	m_pArrow[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlackTex.GetPointer());

	m_pArrow->AddComponent(new CCollider3D);
	m_pArrow->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);

	m_pArrow->AddComponent(new CWaterSkill0Script);
	m_pArrow->GetScript<CWaterSkill0Script>()->SetMove(false);



	//m_pArrow->GetScript<CWaterSkill0Script>()->SetPlayer(m_pPlayer);
	m_pArrow->GetScript<CWaterSkill0Script>()->SetSkill(CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::WATER_0));
	FindLayer(L"Arrow")->AddGameObject(m_pArrow);


	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Blue");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Red");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Red");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Cover");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Cover");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Red");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Arrow");

	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Arrow");

	
	CSensorMgr::GetInst()->CheckSensorLayer(L"Blue", L"Red");
	CSensorMgr::GetInst()->CheckSensorLayer(L"Blue", L"Blue");
	CSensorMgr::GetInst()->CheckSensorLayer(L"Red", L"Red");
	Awake();
	Start();
}
