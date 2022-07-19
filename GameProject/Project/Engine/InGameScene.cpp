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

#include "SkillMgr.h" 
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
    // 타일
    //-----------------------------------------------------------------------------------------------

    pObject = new CGameObject;
    pObject->SetName(L"Tile");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CMeshRender);

    // Transform     
    pObject->Transform()->SetLocalPos(Vec3(0.f, -1.f, 2300.f));
    pObject->Transform()->SetLocalScale(Vec3(2000.f, 10000.f, 1.f));
    pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

    // MeshRender     
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

    Ptr<CMaterial> pMtrl = new CMaterial;
    pMtrl->DisableFileSave();
    pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3DShader"));
    CResMgr::GetInst()->AddRes(L"TileMtrl", pMtrl);
    pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMtrl"));


    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());


    pObject->MeshRender()->SetDynamicShadow(false);


    // AddGameObject
    FindLayer(L"Tile")->AddGameObject(pObject);

    //-----------------------------------------------------------------------------------------------
    // 플레이어 
    //-----------------------------------------------------------------------------------------------

   // Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\player_red.fbx");
    Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player_without_bow01.mdat", L"MeshData\\player_without_bow01.mdat");
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
    pObject->Transform()->SetLocalPos(Vec3(-0.f, 0.f, 0.f));
    pObject->Transform()->SetLocalScale(Vec3(0.4f, 0.4f, 0.5f));
    pObject->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
    pObject->MeshRender()->SetDynamicShadow(true);
    pObject->Sensor()->SetRadius(500.f);
    pObject->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::FIRE);

    CGameObject* pPlayer = pObject;
    pObject->GetScript<CPlayerScript>()->SetCurHp(50);
    // pMainCam->Transform()->SetLocalPos(Vec3(0.f, 100.f, 130.f));

    pMainCam->Camera()->SetPlayer(pObject);
    pMainCam->Camera()->SetbPlay(true);

    CAnimation* pNewAnimation = new CAnimation;
    pNewAnimation->InsertAnimClip(L"IDLE", 0, 37);
    pNewAnimation->InsertAnimClip(L"WALK", 44, 73);
    pNewAnimation->InsertAnimClip(L"JUMP", 81, 100);
    pNewAnimation->InsertAnimClip(L"ATTACK_READY", 145, 167);
    pNewAnimation->InsertAnimClip(L"ATTACK", 168, 175);
    pNewAnimation->InsertAnimClip(L"ATTACK_READY_HIGH_1", 202, 207);
    pNewAnimation->InsertAnimClip(L"ATTACK_READY_HIGH_2", 202, 208);
    pNewAnimation->InsertAnimClip(L"ATTACK_READY_HIGH_3", 202, 209);
    pNewAnimation->InsertAnimClip(L"ATTACK_READY_HIGH_4", 202, 210);
    pNewAnimation->InsertAnimClip(L"ATTACK_READY_HIGH_5", 202, 212);
    pNewAnimation->InsertAnimClip(L"DEMAGED", 235, 240);
    pNewAnimation->InsertAnimClip(L"DIE", 242, 261);      //           240, 261      
    pNewAnimation->InsertAnimClip(L"RUN", 305, 320);      // 305, 320
    //pNewAnimation->InsertAnimation(L"DIE", 269, 289, false, false);

    pObject->Animator3D()->SetAnimation(pNewAnimation);
    pObject->Animator3D()->SetAnimClip(pNewAnimation->GetAnimClip());
    pObject->GetScript<CPlayerScript>()->Init();

    FindLayer(L"Red")->AddGameObject(pObject, false);

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
    pBow->GetScript<CBowScript>()->SetPlayer(pObject);
    pObject->GetScript<CPlayerScript>()->SetBowObject(pBow);

    CGameObject* pEmptyPlayer = new CGameObject;
    pEmptyPlayer->AddComponent(new CTransform);
    pEmptyPlayer->AddComponent(new CEmptyPlayerScript);
    pEmptyPlayer->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(0.f), 0.f));

    pMainCam->Transform()->SetLocalPos(Vec3(-200.f, 100.f, -50.f));      // -300, 130, -50
    pMainCam->Transform()->SetLocalRot(Vec3(0, XMConvertToRadians(90.f), XMConvertToRadians(0.f)));      // -15.f
    pMainCam->GetScript<CCameraScript>()->Init();

    pEmptyPlayer->AddChild(pMainCam);

    FindLayer(L"Default")->AddGameObject(pEmptyPlayer);

    //-----------------------------------------------------------------------------------------------
    // 맵 (baseMap + 타워 + 넥서스)
    //-----------------------------------------------------------------------------------------------
          // 기본 성 fbx
          //pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\testmap01.fbx");
    pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\baseMap.mdat", L"MeshData\\baseMap.mdat");
    //pMeshData->Save(pMeshData->GetPath());

    pObject = pMeshData->Instantiate();

    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));      //0.f, 370.f, 0.f
    pObject->Transform()->SetLocalRot(Vec3(-PI / 2, PI / 2, 0.f));
    pObject->Transform()->SetLocalScale(Vec3(0.8f, 0.8f, 0.8f));
    pObject->MeshRender()->SetDynamicShadow(false);

    FindLayer(L"Default")->AddGameObject(pObject);

    //pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Roof.fbx");
    pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Roof.mdat", L"MeshData\\Roof.mdat");
    //pMeshData->Save(pMeshData->GetPath());

    pObject = pMeshData->Instantiate();

    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(0.f, 1500.f, 0.f));      //0.f, 370.f, 0.f
    pObject->Transform()->SetLocalRot(Vec3(-PI / 2, PI / 2, 0.f));
    pObject->Transform()->SetLocalScale(Vec3(0.8f, 0.8f, 0.8f));
    pObject->MeshRender()->SetDynamicShadow(false);

    FindLayer(L"Default")->AddGameObject(pObject);

    // 레드 1차타워
    pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\SecondTower.mdat", L"MeshData\\SecondTower.mdat");
    CGameObject* pRedFirstTower;
    pRedFirstTower = pMeshData->Instantiate();
    pRedFirstTower->SetName(L"FirstTower");
    pRedFirstTower->AddComponent(new CCollider3D);
    pRedFirstTower->AddComponent(new CSensor);
    pRedFirstTower->AddComponent(new CTowerScript);
    pRedFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
    pRedFirstTower->Sensor()->SetRadius(1000.f);
    pRedFirstTower->Transform()->SetLocalPos(Vec3(-1625, 0.f, 3000.f));
    ///pRedFirstTower->Transform()->SetLocalRot(Vec3(0.f, 3.14f/4, 0.f));
    pRedFirstTower->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
    pRedFirstTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pRedFirstTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
    pRedFirstTower->Collider3D()->SetOffsetPos(Vec3(0.f, 330.f, 25.f));
    pRedFirstTower->FrustumCheck(false);
    pRedFirstTower->GetScript<CTowerScript>()->Init();
    pRedFirstTower->MeshRender()->SetDynamicShadow(false);
    pRedFirstTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::RED);
    FindLayer(L"Red")->AddGameObject(pRedFirstTower);

    // 레드 2차타워
    //   pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\SecondTower.fbx");
    pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\SecondTower.mdat", L"MeshData\\SecondTower.mdat");
    CGameObject* pRedSecondTower;
    pRedSecondTower = pMeshData->Instantiate();
    pRedSecondTower->SetName(L"SecondTower");
    pRedSecondTower->AddComponent(new CCollider3D);
    pRedSecondTower->AddComponent(new CSensor);
    pRedSecondTower->AddComponent(new CTowerScript);
    pRedSecondTower->Sensor()->SetRadius(1500.f);
    pRedSecondTower->Transform()->SetLocalPos(Vec3(875.f, 0.f, 5500.f));
    pRedSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
    pRedSecondTower->GetScript<CTowerScript>()->SetFirstTower(pRedFirstTower);
    pRedSecondTower->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
    pRedSecondTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pRedSecondTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
    pRedSecondTower->Collider3D()->SetOffsetPos(Vec3(0.f, 330.f, 25.f));
    pRedSecondTower->FrustumCheck(false);
    pRedSecondTower->GetScript<CTowerScript>()->Init();
    pRedSecondTower->MeshRender()->SetDynamicShadow(false);

    pRedSecondTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::RED);
    pRedFirstTower->GetScript<CTowerScript>()->SetSecondTower(pRedSecondTower);
    FindLayer(L"Red")->AddGameObject(pRedSecondTower);

    // 블루 1차타워
    //pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\FirstTower01.fbx");
    pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\SecondTower01.mdat", L"MeshData\\SecondTower01.mdat");
    //   pMeshData->Save(pMeshData->GetPath());
    CGameObject* pBlueFirstTower;
    pBlueFirstTower = pMeshData->Instantiate();
    pBlueFirstTower->AddComponent(new CCollider3D);
    pBlueFirstTower->AddComponent(new CSensor);
    pBlueFirstTower->AddComponent(new CTowerScript);
    pBlueFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
    pBlueFirstTower->Sensor()->SetRadius(1000.f);
    pBlueFirstTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pBlueFirstTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
    pBlueFirstTower->Collider3D()->SetOffsetPos(Vec3(0.f, 330.f, 25.f));
    pBlueFirstTower->FrustumCheck(false);
    pBlueFirstTower->Transform()->SetLocalPos(Vec3(375.f, 0.f, 300.f));
    pBlueFirstTower->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
    pBlueFirstTower->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
    pBlueFirstTower->MeshRender()->SetDynamicShadow(false);
    pBlueFirstTower->GetScript<CTowerScript>()->Init();
    pBlueFirstTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::BLUE);
    FindLayer(L"Blue")->AddGameObject(pBlueFirstTower);

    // 블루 2차타워
    pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\SecondTower01.mdat", L"MeshData\\SecondTower01.mdat");
    CGameObject* pBlueSecondTower;
    pBlueSecondTower = pMeshData->Instantiate();
    pBlueSecondTower->AddComponent(new CCollider3D);
    pBlueSecondTower->AddComponent(new CSensor);
    pBlueSecondTower->AddComponent(new CTowerScript);
    pBlueSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
    pBlueSecondTower->Sensor()->SetRadius(1500.f);
    pBlueSecondTower->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pBlueSecondTower->GetScript<CTowerScript>()->SetFirstTower(pBlueFirstTower);
    pBlueSecondTower->Collider3D()->SetOffsetScale(Vec3(100.f, 220.f, 150.f));
    pBlueSecondTower->Collider3D()->SetOffsetPos(Vec3(0.f, 330.f, 25.f));
    pBlueSecondTower->FrustumCheck(false);
    pBlueSecondTower->Transform()->SetLocalPos(Vec3(-2125.f, 0.f, -2200.f));
    pBlueSecondTower->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
    pBlueSecondTower->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
    pBlueSecondTower->MeshRender()->SetDynamicShadow(false);
    pBlueSecondTower->GetScript<CTowerScript>()->Init();
    pBlueSecondTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::BLUE);
    pBlueSecondTower->GetScript<CTowerScript>()->SetFirstTower(pBlueFirstTower);
    pBlueFirstTower->GetScript<CTowerScript>()->SetSecondTower(pBlueSecondTower);
    FindLayer(L"Blue")->AddGameObject(pBlueSecondTower);


    // 블루 넥서스
    CGameObject* pBlueNexus;
    //   pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Nexus01.fbx");
    pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Nexus01.mdat", L"MeshData\\Nexus01.mdat");
    //pMeshData->Save(pMeshData->GetPath());
    pBlueNexus = pMeshData->Instantiate();
    pBlueNexus->AddComponent(new CTowerScript);
    pBlueNexus->AddComponent(new CCollider3D);
    pBlueNexus->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pBlueNexus->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
    pBlueNexus->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));

    pBlueNexus->FrustumCheck(false);
    pBlueNexus->Transform()->SetLocalPos(Vec3(-625.f, 35.f, -4700.f));
    pBlueNexus->Transform()->SetLocalRot(Vec3(-3.14f / 6, 0.F, 0.f));

    pBlueNexus->Transform()->SetLocalScale(Vec3(130.f, 130.f, 130.f));
    pBlueNexus->MeshRender()->SetDynamicShadow(false);
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
    pBlueSpawnPlace->FrustumCheck(false);
    pBlueSpawnPlace->GetScript<CSpawnScript>()->SetSpawnState(CAMP_STATE::BLUE);

    FindLayer(L"Blue")->AddGameObject(pBlueSpawnPlace);

    // 레드 넥서스
    CGameObject* pRedNexus;
    //   pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Nexus.fbx");
    pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Nexus.mdat", L"MeshData\\Nexus.mdat");
    //pMeshData->Save(pMeshData->GetPath());
    pRedNexus = pMeshData->Instantiate();
    pRedNexus->AddComponent(new CCollider3D);   
    pRedNexus->AddComponent(new CTowerScript);
    pRedNexus->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pRedNexus->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
    pRedNexus->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));
    pRedNexus->FrustumCheck(false);
    pRedNexus->Transform()->SetLocalPos(Vec3(-625.f, 35.f, 8000.f));
    pRedNexus->Transform()->SetLocalRot(Vec3(-3.14f / 6, 3.14f, 0.f));

    pRedNexus->Transform()->SetLocalScale(Vec3(130.f, 130.f, 130.f));
    pRedNexus->MeshRender()->SetDynamicShadow(false);
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
    pRedSpawnPlace->FrustumCheck(false);
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

    //-----------------------------------------------------------------------------------------------
    // 장애물
    //-----------------------------------------------------------------------------------------------

    CGameObject* pObstacle;
    pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\obstacle.mdat", L"MeshData\\obstacle.mdat");
    Quaternion qRot;

    for (int i = 0; i < 8; ++i) {
        pObstacle = pMeshData->Instantiate();
        pObstacle->AddComponent(new CCollider3D);
        pObstacle->SetName(L"obstacle");
        pObstacle->FrustumCheck(false);
        pObstacle->MeshRender()->SetDynamicShadow(false);
        pObstacle->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
        pObstacle->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 500.f));
        pObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 250.f));
        pObstacle->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
        pObstacle->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));

        switch (i) {
        case 0:
            // R2 앞
            pObstacle->Transform()->SetLocalPos(Vec3(1000.f, 80.f, 4400.f));
            qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-39.f));
            break;
        case 1:
            // R1 앞
            pObstacle->Transform()->SetLocalPos(Vec3(-1975.f, 80.f, 2100.f));
            qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-43.f));
            break;
        case 2:
            // B1 앞
            pObstacle->Transform()->SetLocalPos(Vec3(875.f, 80.f, 1000.f));
            qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-35.f));
            break;
        case 3:
            // B2 앞
            pObstacle->Transform()->SetLocalPos(Vec3(-2725.f, 80.f, -900.f));
            qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(31.f));
            break;
        case 4:
            // R2 뒤
            pObstacle->Transform()->SetLocalPos(Vec3(1400.f, 80.f, 6600.f));
            qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(75.f));
            break;
        case 5:
            // R1 뒤
            pObstacle->Transform()->SetLocalPos(Vec3(-2170.f, 80.f, 4200.f));
            qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(72.f));
            break;
        case 6:
            // B1 뒤
            pObstacle->Transform()->SetLocalPos(Vec3(1080.f, 80.f, -1000.f));
            qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(60.f));
            break;
        case 7:
            // B2 뒤
            pObstacle->Transform()->SetLocalPos(Vec3(-2800.f, 80.f, -3000.f));
            qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-27.f));
            break;
        }
        pObstacle->Transform()->SetQuaternion(qRot);
        FindLayer(L"Obstacle")->AddGameObject(pObstacle);
    }


    //-----------------------------------------------------------------------------------------------
    // 맵 콜라이더
    //-----------------------------------------------------------------------------------------------
    pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TransparencyMtrl");
    Ptr<CTexture> pTransparency = CResMgr::GetInst()->FindRes<CTexture>(L"Transparency");
    Ptr<CTexture> pSky01 = CResMgr::GetInst()->FindRes<CTexture>(L"Sky01");
    
    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(-3780.f, 0.f, 1500.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(20.f, 200.f, 14290.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);

    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);

    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(2550.f, 0.f, 1500.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(20.f, 200.f, 14290.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);


    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(-2605.f, 0.f, -7070.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(850.f, 200.f, 3400.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);


    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(-3390.f, 0.f, -5000.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(820.f, 200.f, 820.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);


    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(-825.f, 0.f, -8750.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(4000.f, 200.f, 50.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);

    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(-825.f, 0.f, 12300.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(4000.f, 200.f, 50.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);

    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(2195.f, 0.f, -5000.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(820.f, 200.f, 820.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);

    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(1410.f, 0.f, -7070.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(850.f, 200.f, 3400.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);


    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(2195.f, 0.f, 8570.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(820.f, 200.f, 820.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);

    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(1410.f, 0.f, 10640.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(850.f, 200.f, 3400.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);

    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(-3390.f, 0.f, 8570.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(820.f, 200.f, 820.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);

    pObject = new CGameObject;
    pObject->SetName(L"Cover");
    pObject->AddComponent(new CTransform);
    pObject->AddComponent(new CCollider3D);
    pObject->FrustumCheck(false);
    pObject->Transform()->SetLocalPos(Vec3(-2605.f, 0.f, 10640.f));
    pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
    pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
    pObject->Collider3D()->SetOffsetScale(Vec3(850.f, 200.f, 3400.f));
    pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
    pObject->AddComponent(new CMeshRender);
    pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    pObject->MeshRender()->SetMaterial(pMtrl);
    pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
    FindLayer(L"Cover")->AddGameObject(pObject);

    /*
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
    //   pObject->AddComponent(new CMeshRender);
    //   pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    //   pObject->MeshRender()->SetMaterial(pMtrl);
    //   pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());

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
    */


    //-----------------------------------------------------------------------------------------------
    // 스카이박스
    //-----------------------------------------------------------------------------------------------
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
    Vec3 vScale = Vec3(res.fWidth / 13, res.fWidth / 13, 1.f);

    pUICrossHair->Transform()->SetLocalPos(Vec3(0.f, res.fHeight / 4, 1.f));
    pUICrossHair->Transform()->SetLocalScale(vScale);

    pUICrossHair->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    Ptr<CMaterial> pCrossHairMtrl = new CMaterial;
    pCrossHairMtrl->DisableFileSave();
    pCrossHairMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"DarkTexShader"));
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
    pDarkUIMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"DarkTexShader"));
    pDarkUI->MeshRender()->SetMaterial(pDarkUIMtrl);
    Ptr<CTexture> pDarkUITex = CResMgr::GetInst()->FindRes<CTexture>(L"DarkUITex");
    pDarkUI->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pDarkUITex.GetPointer());

    FindLayer(L"UI")->AddGameObject(pDarkUI);


    CGameObject* m_pArrow = new CGameObject;
    m_pArrow->SetName(L"Arrow");


    m_pArrow->AddComponent(new CTransform);
    m_pArrow->Transform()->SetLocalPos(Vec3(-50.f, 50.f, -50.f));
    m_pArrow->Transform()->SetLocalScale(Vec3(100.f, 1.f, 1.f));
    m_pArrow->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
    m_pArrow->AddComponent(new CMeshRender);
    m_pArrow->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
    m_pArrow->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
    //   m_pArrow[i]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlackTex.GetPointer());

    m_pArrow->AddComponent(new CCollider3D);
    m_pArrow->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);

    m_pArrow->AddComponent(new CArrowScript(ELEMENT_TYPE::WATER));
    m_pArrow->GetScript<CArrowScript>()->SetMove(false);
    m_pArrow->GetScript<CArrowScript>()->SetSkill(CSkillMgr::GetInst()->FindSkill((UINT)SKILL_CODE::WATER_0));
    m_pArrow->GetScript<CArrowScript>()->SetLayerIdx(4);
    //m_pArrow->GetScript<CArrowScript>()->SetPlayer(pPlayer);
    //pPlayer
    FindLayer(L"Arrow")->AddGameObject(m_pArrow);



    //-----------------------------------------------------------------------------------------------
    // 조명 추가
    //-----------------------------------------------------------------------------------------------

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


CInGameScene::~CInGameScene() {

}