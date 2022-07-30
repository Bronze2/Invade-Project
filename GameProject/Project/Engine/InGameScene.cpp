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
#include "MonsterScript.h"
#include "ParticleSystem.h"
#include "ArrowScript.h"
#include "MinionScript.h"
#include "GameFramework.h"
#include "Animation3D.h"
#include "SpawnScript.h"
#include "Sensor.h"
#include "TowerScript.h"
#include "EmptyPlayerScript.h"
#include "BowScript.h"
#include "EmptyCameraScript.h"
#include "CrossHairScript.h"
#include "Network.h"
#include "FontMgr.h"
#include "SpawnMgr.h"
#include "PlayerColScript.h"
#include "CTowerColScript.h"
#include "SafeZone.h"

void CInGameScene::Init()
{
	ShowCursor(false);

	GetLayer(0)->SetName(L"Default");
	GetLayer(1)->SetName(L"Player");
	GetLayer(2)->SetName(L"Monster");
	GetLayer(3)->SetName(L"Blue");
	GetLayer(4)->SetName(L"Red");
	GetLayer(5)->SetName(L"Cover");
	GetLayer(6)->SetName(L"Arrow");
	GetLayer(7)->SetName(L"HpBar");


	GetLayer(8)->SetName(L"BlueSpawnPlace");
	GetLayer(9)->SetName(L"RedSpawnPlace");
	GetLayer(10)->SetName(L"Obstacle");
	GetLayer(11)->SetName(L"Box");

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
	pMainCam->Camera()->SetLayerAllCheck();
	CCollisionMgr::GetInst()->SetCameraObject(pMainCam->Camera());
	//pMainCam->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pMainCam->Camera()->SetFar(30000.f);
	pMainCam->Camera()->SetLayerAllCheck();

	//CGameObject* pEmptyCam = nullptr;
	//pEmptyCam = new CGameObject;
	//pEmptyCam->SetName(L"EmptyCam");
	//pEmptyCam->AddComponent(new CTransform);
	//pEmptyCam->AddComponent(new CCamera);
	//pEmptyCam->AddComponent(new CEmptyCameraScript);
	//pEmptyCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	//CRenderMgr::GetInst()->SetCamera(pMainCam->Camera());


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
	pObject->Transform()->SetLocalPos(Vec3(-1200.f, 1000.f, -1500.f));		//수민
	FindLayer(L"Default")->AddGameObject(pObject);







	//Ptr<CMeshData> pMeshDataBlue = CResMgr::GetInst()->LoadFBX(L"FBX\\player_without_bow03.fbx");
	//pMeshDataBlue->Save(pMeshDataBlue->GetPath());


	//Ptr<CMeshData> pMeshDataRed = CResMgr::GetInst()->LoadFBX(L"FBX\\player_red.fbx");
	//pMeshDataRed->Save(pMeshDataRed->GetPath());

	//Ptr<CMeshData> pMeshDataBlue = CResMgr::GetInst()->LoadFBX(L"FBX\\player_blue.fbx");
	//pMeshDataBlue->Save(pMeshDataBlue->GetPath());

	Ptr<CMeshData> pMeshDataRed = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player_red.mdat", L"MeshData\\player_red.mdat");
	Ptr<CMeshData> pMeshDataBlue = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player_blue.mdat", L"MeshData\\player_blue.mdat");
	Ptr<CMeshData> pMeshbowData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\bow_big.mdat", L"MeshData\\bow_big.mdat");

	Ptr<CTexture>pPlayerRed = CResMgr::GetInst()->FindRes<CTexture>(L"Player_Red");
	Ptr<CTexture>pPlayerBlue = CResMgr::GetInst()->FindRes<CTexture>(L"Player_Blue");

	CGameObject* pEmptyPlayer = new CGameObject;
	pEmptyPlayer->AddComponent(new CTransform);
	pEmptyPlayer->AddComponent(new CEmptyPlayerScript);
	pEmptyPlayer->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(0.f), 0.f));
	
	pMainCam->Transform()->SetLocalPos(Vec3(-200, 100, -30));

	//pMainCam->Transform()->SetLocalPos(Vec3(-200, 100, -50));
	pMainCam->Transform()->SetLocalRot(Vec3(0, XMConvertToRadians(90.f), XMConvertToRadians(0.f)));
	pMainCam->GetScript<CCameraScript>()->Init();
	pEmptyPlayer->AddChild(pMainCam);



	pObject = new CGameObject;


	if (Network::GetInst()->getMainClient().camp == CAMP_STATE::BLUE) {
		//pObject = pMeshDataBlue->Instantiate();
		pObject = pMeshDataBlue->Instantiate();
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pPlayerBlue.GetPointer());
		//			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pPlayerBlue.GetPointer());
	}
	else if (Network::GetInst()->getMainClient().camp == CAMP_STATE::RED) {
		pObject = pMeshDataRed->Instantiate();
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pPlayerRed.GetPointer());
	}


	pObject->SetName(L"Monster");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CSensor);
	pObject->AddComponent(new CPlayerScript);
	pObject->FrustumCheck(false);

	Vec3 Pos = Vec3(Network::GetInst()->getMainClient().pos.x, Network::GetInst()->getMainClient().pos.y, Network::GetInst()->getMainClient().pos.z);
	if (Network::GetInst()->getMainClient().camp == CAMP_STATE::BLUE) {

		pObject->Transform()->SetLocalPos(Pos);
		pObject->GetScript<CPlayerScript>()->SetLerpPos(Pos);

		//pEmptyPlayer->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0));
		cout << "메인클라 블루  [" << Network::GetInst()->getMainClient().id<<"]" << endl;
		//pObject->GetScript<CPlayerScript>()->SetType(Network::GetInst()->getMainClient().skill);
		pObject->GetScript<CPlayerScript>()->SetCamp(CAMP_STATE::BLUE);

	}

	else if (Network::GetInst()->getMainClient().camp == CAMP_STATE::RED) {
		pObject->Transform()->SetLocalPos(Pos);
		pObject->GetScript<CPlayerScript>()->SetLerpPos(Pos);
		//pEmptyPlayer->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0));
		cout << "메인클라 레드  [" << Network::GetInst()->getMainClient().id << "]" << endl;
		pObject->GetScript<CPlayerScript>()->SetCamp(CAMP_STATE::RED);

	}
	pObject->GetScript<CPlayerScript>()->SetType(Network::GetInst()->getMainClient().skill);
	//pObject->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::THUNDER);


	pObject->Transform()->SetLocalScale(Vec3(0.4f, 0.4f, 0.5f));
	pObject->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
	pObject->MeshRender()->SetDynamicShadow(true);
	pObject->GetScript<CPlayerScript>()->m_SetId(Network::GetInst()->getMainClient().id);
	
	
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
	pNewAnimation->InsertAnimClip(L"RUN", 298, 319);      // 305, 320
	//pNewAnimation->InsertAnimation(L"DIE", 269, 289, false, false);

	pObject->Animator3D()->SetAnimation(pNewAnimation);
	pObject->Animator3D()->SetAnimClip(pNewAnimation->GetAnimClip());

	pObject->GetScript<CPlayerScript>()->Init();
	pObject->GetScript<CPlayerScript>()->SetMain();

	FindLayer(L"Blue")->AddGameObject(pObject, false);
	pPlayer = pObject;
	//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\bow_big.mdat", L"MeshData\\bow_big.mdat");
	CGameObject* pBow;
	pBow = pMeshbowData->Instantiate();
	pBow->SetName(L"Bow");

	pBow->AddComponent(new CTransform);
	pBow->AddComponent(new CSensor);
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
	pBow->GetScript<CBowScript>()->SetCamp(Network::GetInst()->getMainClient().camp);
	pObject->AddChild(pBow);
	pBow->GetScript<CBowScript>()->SetPlayer(pObject);
	pObject->GetScript<CPlayerScript>()->SetBowObject(pBow);
	pMainCam->Camera()->SetPlayer(pObject);
	pMainCam->Camera()->SetbPlay(true);
	pBow->GetScript<CBowScript>()->SetMain();
	FindLayer(L"Blue")->AddGameObject(pBow);

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TransparencyMtrl");
	Ptr<CTexture> pTransparency = CResMgr::GetInst()->FindRes<CTexture>(L"Transparency");
	CGameObject* pColObj = new CGameObject;
	pColObj->SetName(L"PlayerCol");
	pColObj->AddComponent(new CCollider3D);
	pColObj->AddComponent(new CPlayerColScript);
	pColObj->AddComponent(new CTransform);
	pColObj->AddComponent(new CMeshRender);
	pColObj->Transform()->SetLocalPos(pObject->Transform()->GetLocalPos());
	pColObj->Transform()->SetLocalScale(pObject->Transform()->GetLocalScale());
	pColObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pColObj->MeshRender()->SetMaterial(pMtrl);
	pColObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	pColObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pColObj->Collider3D()->SetOffsetScale(Vec3(80.f, 200.f, 80.f));      // 80.f, 200.f, 80.f ?????
	pColObj->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pColObj->GetScript<CPlayerColScript>()->SetPlayer(pObject);
	pObject->GetScript<CPlayerScript>()->SetColPlayer(pColObj);
	FindLayer(L"Blue")->AddGameObject(pColObj, false);



	//pEmptyCam->Transform()->SetLocalPos(Vec3(-300, 130, -50));
	//pEmptyCam->Transform()->SetLocalRot(Vec3(0, XMConvertToRadians(90.f), XMConvertToRadians(0.f)));
//	pEmptyPlayer->AddChild(pEmptyCam);
	FindLayer(L"Default")->AddGameObject(pEmptyPlayer, false);


	for (auto &cl : Network::GetInst()->getOtherClients())
	{

		pObject = new CGameObject;


		if (cl.second.camp == CAMP_STATE::BLUE) {
			pObject = pMeshDataBlue->Instantiate();
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pPlayerBlue.GetPointer());
		}
		else if(cl.second.camp == CAMP_STATE::RED) {
			pObject = pMeshDataRed->Instantiate();
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pPlayerRed.GetPointer());
		}


		pObject->SetName(L"OtherClients");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CSensor);
		pObject->AddComponent(new CPlayerScript);
		pObject->FrustumCheck(false);

		Vec3 Pos = Vec3(cl.second.pos.x, cl.second.pos.y, cl.second.pos.z);

		// ColPlayer
		CGameObject* pColObj = new CGameObject;
		pColObj->SetName(L"PlayerCol");
		pColObj->AddComponent(new CCollider3D);
		pColObj->AddComponent(new CPlayerColScript);
		pColObj->AddComponent(new CTransform);
		pColObj->AddComponent(new CMeshRender);
		pColObj->Transform()->SetLocalPos(pObject->Transform()->GetLocalPos());
		pColObj->Transform()->SetLocalScale(pObject->Transform()->GetLocalScale());
		pColObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pColObj->MeshRender()->SetMaterial(pMtrl);
		pColObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
		pColObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		pColObj->Collider3D()->SetOffsetScale(Vec3(80.f, 200.f, 80.f));      // 80.f, 200.f, 80.f ?????
		pColObj->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
		pColObj->GetScript<CPlayerColScript>()->SetPlayer(pObject);
		pObject->GetScript<CPlayerScript>()->SetColPlayer(pColObj);
		FindLayer(L"Blue")->AddGameObject(pColObj, false);


		if (cl.second.camp == CAMP_STATE::BLUE) {
			pObject->Transform()->SetLocalPos(Pos);
			pObject->GetScript<CPlayerScript>()->SetLerpPos(Pos);

			cout << "[ID" << cl.second.id << "] 블루"<< endl;
			pObject->GetScript<CPlayerScript>()->SetCamp(CAMP_STATE::BLUE);

		}
		else if (cl.second.camp == CAMP_STATE::RED) {
			pObject->Transform()->SetLocalPos(Pos);
			pObject->GetScript<CPlayerScript>()->SetLerpPos(Pos);
			cout << "[ID" << cl.second.id << "] 레드" << endl;
			pObject->GetScript<CPlayerScript>()->SetCamp(CAMP_STATE::RED);
		}

		pObject->GetScript<CPlayerScript>()->SetType(cl.second.skill);
		//pObject->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::THUNDER);

		pObject->Transform()->SetLocalScale(Vec3(0.4f, 0.4f, 0.5f));
		pObject->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));
		pObject->MeshRender()->SetDynamicShadow(true);
		pObject->GetScript<CPlayerScript>()->m_SetId(cl.second.id);

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
		pNewAnimation->InsertAnimClip(L"RUN", 298, 319);      // 305, 320

		pObject->Animator3D()->SetAnimation(pNewAnimation);
		pObject->Animator3D()->SetAnimClip(pNewAnimation->GetAnimClip());

		pObject->GetScript<CPlayerScript>()->Init();

		if (pObject->GetScript<CPlayerScript>()->GetCamp() == Network::GetInst()->getMainClient().camp) {
			TeamPlayer.push_back(pObject);
		}



		FindLayer(L"Blue")->AddGameObject(pObject, false);

		//pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\bow_big.mdat", L"MeshData\\bow_big.mdat");
		CGameObject* pBow;
		pBow = pMeshbowData->Instantiate();
		pBow->SetName(L"Bow");

		pBow->AddComponent(new CTransform);
		pBow->AddComponent(new CSensor);
		pBow->AddComponent(new CBowScript);
		pBow->FrustumCheck(false);
		pBow->Transform()->SetLocalPos(Vec3(0.0f, 0.0f, 0.0f));
		pBow->Transform()->SetLocalScale(Vec3(2.4f, 1.2f, 2.4f));
		pBow->Transform()->SetLocalRot(Vec3(0.0f, 0.0f, 0.0f));
		pBow->MeshRender()->SetDynamicShadow(true);
		pBow->GetScript<CBowScript>()->SetTarget(pObject);
		pBow->GetScript<CBowScript>()->SetBoneIdx(14);
		pBow->GetScript<CBowScript>()->SetCamp(cl.second.camp);

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
		FindLayer(L"Blue")->AddGameObject(pBow);
	}

	///맵

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\ingame.fbx");
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\ingame.mdat", L"MeshData\\ingame.mdat");
	//pMeshData->Save(pMeshData->GetPath());

	pObject = pMeshData->Instantiate();

	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(0.f, -80.f, 0.f));      //0.f, 370.f, 0.f
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
	pRedFirstTower->AddComponent(new CSensor);
	pRedFirstTower->AddComponent(new CTowerScript);
	pRedFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
	pRedFirstTower->Sensor()->SetRadius(300.f);
	pRedFirstTower->Transform()->SetLocalPos(Vec3(0.f, 0.f, 11500.f));
	pRedFirstTower->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
	pRedFirstTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedFirstTower->FrustumCheck(false);
	pRedFirstTower->GetScript<CTowerScript>()->m_SetId(0);
	pRedFirstTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::RED);      // 에이치피바
	pRedFirstTower->GetScript<CTowerScript>()->Init();
	pRedFirstTower->MeshRender()->SetDynamicShadow(true);

	FindLayer(L"Red")->AddGameObject(pRedFirstTower);

	CGameObject* pTowerCol = new CGameObject;
	pTowerCol->SetName(L"TowerRed1");
	pTowerCol->AddComponent(new CCollider3D);
	pTowerCol->AddComponent(new CTowerColScript);
	pTowerCol->AddComponent(new CMeshRender);
	pTowerCol->AddComponent(new CTransform);
	pTowerCol->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pTowerCol->Collider3D()->SetOffsetScale(Vec3(300.f, 800.f, 300.f));
	pTowerCol->Collider3D()->SetOffsetPos(Vec3(0.f, 400.f, 0.f));
	pTowerCol->Transform()->SetLocalPos(pRedFirstTower->Transform()->GetLocalPos());
	pTowerCol->FrustumCheck(false);
	pTowerCol->GetScript<CTowerColScript>()->SetTower(pRedFirstTower);
	pTowerCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pTowerCol->MeshRender()->SetMaterial(pMtrl);
	pTowerCol->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	pRedFirstTower->GetScript<CTowerScript>()->SetColTower(pTowerCol);
	FindLayer(L"Red")->AddGameObject(pTowerCol);

	// 레드 2차타워
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\LowPoly_Tower.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\LowPoly_Tower.mdat", L"MeshData\\LowPoly_Tower.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	CGameObject* pRedSecondTower;
	pRedSecondTower = pMeshData->Instantiate();
	pRedSecondTower->SetName(L"SecondTower");
	pRedSecondTower->AddComponent(new CSensor);
	pRedSecondTower->AddComponent(new CTowerScript);
	pRedSecondTower->Sensor()->SetRadius(300.f);
	pRedSecondTower->Transform()->SetLocalPos(Vec3(2500.f, 0.f, 14500.f));
	pRedSecondTower->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
	pRedSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
	pRedSecondTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedSecondTower->FrustumCheck(false);
	pRedSecondTower->GetScript<CTowerScript>()->m_SetId(1);
	pRedSecondTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::RED);
	pRedSecondTower->GetScript<CTowerScript>()->Init();
	pRedSecondTower->MeshRender()->SetDynamicShadow(true);

	FindLayer(L"Red")->AddGameObject(pRedSecondTower);

	pTowerCol = new CGameObject;
	pTowerCol->SetName(L"TowerRed2");
	pTowerCol->AddComponent(new CCollider3D);
	pTowerCol->AddComponent(new CTowerColScript);
	pTowerCol->AddComponent(new CMeshRender);
	pTowerCol->AddComponent(new CTransform);
	pTowerCol->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pTowerCol->Collider3D()->SetOffsetScale(Vec3(300.f, 800.f, 300.f));
	pTowerCol->Collider3D()->SetOffsetPos(Vec3(0.f, 400.f, 0.f));
	pTowerCol->Transform()->SetLocalPos(pRedSecondTower->Transform()->GetLocalPos());
	pTowerCol->FrustumCheck(false);
	pTowerCol->GetScript<CTowerColScript>()->SetTower(pRedSecondTower);
	pRedSecondTower->GetScript<CTowerScript>()->SetColTower(pTowerCol);
	pTowerCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pTowerCol->MeshRender()->SetMaterial(pMtrl);
	pTowerCol->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	FindLayer(L"Red")->AddGameObject(pTowerCol);

	// 블루 1차타워
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\LowPoly_Tower_Blue.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\LowPoly_Tower_Blue.mdat", L"MeshData\\LowPoly_Tower_Blue.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	CGameObject* pBlueFirstTower;
	pBlueFirstTower = pMeshData->Instantiate();
	pBlueFirstTower->AddComponent(new CSensor);
	pBlueFirstTower->AddComponent(new CTowerScript);
	pBlueFirstTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::FIRST);
	pBlueFirstTower->Sensor()->SetRadius(300.f);
	pBlueFirstTower->FrustumCheck(false);
	pBlueFirstTower->Transform()->SetLocalPos(Vec3(2000.f, 10.f, 9000.f));
	pBlueFirstTower->Transform()->SetLocalRot(Vec3(-PI / 2, 3.14f, 0.f));
	pBlueFirstTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueFirstTower->MeshRender()->SetDynamicShadow(true);
	pBlueFirstTower->GetScript<CTowerScript>()->m_SetId(2);
	pBlueFirstTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::BLUE);
	pBlueFirstTower->GetScript<CTowerScript>()->Init();
	FindLayer(L"Blue")->AddGameObject(pBlueFirstTower);

	pTowerCol = new CGameObject;
	pTowerCol->SetName(L"TowerBlue1");
	pTowerCol->AddComponent(new CCollider3D);
	pTowerCol->AddComponent(new CTowerColScript);
	pTowerCol->AddComponent(new CMeshRender);
	pTowerCol->AddComponent(new CTransform);
	pTowerCol->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pTowerCol->Collider3D()->SetOffsetScale(Vec3(300.f, 800.f, 300.f));
	pTowerCol->Collider3D()->SetOffsetPos(Vec3(0.f, 410.f, 0.f));
	pTowerCol->Transform()->SetLocalPos(pBlueFirstTower->Transform()->GetLocalPos());
	pTowerCol->FrustumCheck(false);
	pTowerCol->GetScript<CTowerColScript>()->SetTower(pBlueFirstTower);
	pTowerCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pTowerCol->MeshRender()->SetMaterial(pMtrl);
	pTowerCol->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	pBlueFirstTower->GetScript<CTowerScript>()->SetColTower(pTowerCol);
	FindLayer(L"Blue")->AddGameObject(pTowerCol);

	// 블루 2차타워   
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\BlueArrow.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\LowPoly_Tower_Blue.mdat", L"MeshData\\LowPoly_Tower_Blue.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	CGameObject* pBlueSecondTower;
	pBlueSecondTower = pMeshData->Instantiate();
	pBlueSecondTower->AddComponent(new CSensor);
	pBlueSecondTower->AddComponent(new CTowerScript);
	pBlueSecondTower->GetScript<CTowerScript>()->SetType(TOWER_TYPE::SECOND);
	pBlueSecondTower->Sensor()->SetRadius(300.f);
	pBlueSecondTower->FrustumCheck(false);
	pBlueSecondTower->Transform()->SetLocalPos(Vec3(-500.f, 10.f, 5500.f));
	pBlueSecondTower->Transform()->SetLocalRot(Vec3(-PI / 2, 3.14f, 0.f));
	pBlueSecondTower->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueSecondTower->MeshRender()->SetDynamicShadow(true);
	pBlueSecondTower->GetScript<CTowerScript>()->m_SetId(3);
	pBlueSecondTower->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::BLUE);
	pBlueSecondTower->GetScript<CTowerScript>()->Init();
	FindLayer(L"Blue")->AddGameObject(pBlueSecondTower);

	pTowerCol = new CGameObject;
	pTowerCol->SetName(L"TowerBlue2");
	pTowerCol->AddComponent(new CCollider3D);
	pTowerCol->AddComponent(new CTowerColScript);
	pTowerCol->AddComponent(new CMeshRender);
	pTowerCol->AddComponent(new CTransform);
	pTowerCol->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pTowerCol->Collider3D()->SetOffsetScale(Vec3(300.f, 800.f, 300.f));
	pTowerCol->Collider3D()->SetOffsetPos(Vec3(0.f, 400.f, 0.f));
	pTowerCol->Transform()->SetLocalPos(pBlueSecondTower->Transform()->GetLocalPos());
	pTowerCol->FrustumCheck(false);
	pTowerCol->GetScript<CTowerColScript>()->SetTower(pBlueSecondTower);
	pTowerCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pTowerCol->MeshRender()->SetMaterial(pMtrl);
	pTowerCol->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	pBlueSecondTower->GetScript<CTowerScript>()->SetColTower(pTowerCol);
	FindLayer(L"Blue")->AddGameObject(pTowerCol);


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
	pBlueNexus->AddComponent(new CTowerScript);
	pBlueNexus->GetScript<CTowerScript>()->SetType(TOWER_TYPE::NEXUS);

	pBlueNexus->FrustumCheck(false);
	pBlueNexus->Transform()->SetLocalPos(Vec3(1000.f, 0.f, 2000.f));
	pBlueNexus->Transform()->SetLocalRot(Vec3(-PI / 2, PI, 0.f));

	pBlueNexus->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueNexus->MeshRender()->SetDynamicShadow(true);
	pBlueNexus->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::BLUE);
	pBlueNexus->GetScript<CTowerScript>()->Init();
	pBlueNexus->GetScript<CTowerScript>()->SetFirstTower(pBlueFirstTower);
	pBlueNexus->GetScript<CTowerScript>()->SetSecondTower(pBlueSecondTower);
	pBlueNexus->GetScript<CTowerScript>()->m_SetId(4);

	pTowerCol = new CGameObject;
	pTowerCol->SetName(L"TowerBlue3");
	pTowerCol->AddComponent(new CCollider3D);
	pTowerCol->AddComponent(new CTowerColScript);
	pTowerCol->AddComponent(new CMeshRender);
	pTowerCol->AddComponent(new CTransform);
	pTowerCol->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pTowerCol->Collider3D()->SetOffsetScale(Vec3(500.f, 500.f, 500.f));
	pTowerCol->Collider3D()->SetOffsetPos(Vec3(0.f, 260.f, 0.f));
	pTowerCol->Transform()->SetLocalPos(pBlueNexus->Transform()->GetLocalPos());
	pTowerCol->FrustumCheck(false);
	pTowerCol->GetScript<CTowerColScript>()->SetTower(pBlueNexus);
	pTowerCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pTowerCol->MeshRender()->SetMaterial(pMtrl);
	pTowerCol->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	pBlueNexus->GetScript<CTowerScript>()->SetColTower(pTowerCol);
	FindLayer(L"Blue")->AddGameObject(pTowerCol);

	FindLayer(L"Blue")->AddGameObject(pBlueNexus);

	CGameObject* pBlueSpawnPlace = new CGameObject;
	pBlueSpawnPlace->SetName(L"Spawn_Place");
	pBlueSpawnPlace->AddComponent(new CTransform);
	pBlueSpawnPlace->AddComponent(new CSpawnScript);
	pBlueSpawnPlace->Transform()->SetLocalPos(Vec3(1000.f, 0.f, 2000.f));
	pBlueSpawnPlace->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBlueSpawnPlace->FrustumCheck(false);
	pBlueSpawnPlace->GetScript<CSpawnScript>()->SetSpawnState(CAMP_STATE::BLUE);

	FindLayer(L"BlueSpawnPlace")->AddGameObject(pBlueSpawnPlace);

	// 레드 넥서스
	CGameObject* pRedNexus;
	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\LowPoly_Nexus.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\LowPoly_Nexus.mdat", L"MeshData\\LowPoly_Nexus.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	pRedNexus = pMeshData->Instantiate();
	pRedNexus->AddComponent(new CTowerScript);
	pRedNexus->GetScript<CTowerScript>()->SetType(TOWER_TYPE::NEXUS);
	pRedNexus->FrustumCheck(false);
	pRedNexus->Transform()->SetLocalPos(Vec3(1000.f, -10.f, 17600.f));
	pRedNexus->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));

	pRedNexus->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pRedNexus->MeshRender()->SetDynamicShadow(true);
	pRedNexus->GetScript<CTowerScript>()->SetCampState(CAMP_STATE::RED);
	pRedNexus->GetScript<CTowerScript>()->m_SetId(5);

	pRedNexus->GetScript<CTowerScript>()->Init();
	pRedNexus->GetScript<CTowerScript>()->SetFirstTower(pRedFirstTower);
	pRedNexus->GetScript<CTowerScript>()->SetSecondTower(pRedSecondTower);
	FindLayer(L"Red")->AddGameObject(pRedNexus);

	pTowerCol = new CGameObject;
	pTowerCol->SetName(L"TowerBlue3");
	pTowerCol->AddComponent(new CCollider3D);
	pTowerCol->AddComponent(new CTowerColScript);
	pTowerCol->AddComponent(new CMeshRender);
	pTowerCol->AddComponent(new CTransform);
	pTowerCol->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pTowerCol->Collider3D()->SetOffsetScale(Vec3(500.f, 500.f, 500.f));
	pTowerCol->Collider3D()->SetOffsetPos(Vec3(0.f, 250.f, 0.f));
	pTowerCol->Transform()->SetLocalPos(pRedNexus->Transform()->GetLocalPos());
	pTowerCol->FrustumCheck(false);
	pTowerCol->GetScript<CTowerColScript>()->SetTower(pRedNexus);
	pTowerCol->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pTowerCol->MeshRender()->SetMaterial(pMtrl);
	pTowerCol->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	pBlueFirstTower->GetScript<CTowerScript>()->SetColTower(pTowerCol);
	FindLayer(L"Red")->AddGameObject(pTowerCol);

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

	FindLayer(L"RedSpawnPlace")->AddGameObject(pRedSpawnPlace);

	//-----------------------------------------------------------------------------------------------
	// 장애물
	//-----------------------------------------------------------------------------------------------

	CGameObject* pObstacle = new CGameObject;
	CGameObject* pColObstacle;
	Quaternion qRot;

	for (int i = 0; i < 8; ++i) {
		switch (i) {
		case 0:
			// R2 앞
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Obstacle01.mdat", L"MeshData\\Obstacle01.mdat");
			pObstacle = pMeshData->Instantiate();
			pColObstacle = new CGameObject;
			pColObstacle->SetName(L"obstacle");
			pColObstacle->AddComponent(new CCollider3D);
			pColObstacle->AddComponent(new CMeshRender);
			pColObstacle->AddComponent(new CTransform);
			pColObstacle->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pColObstacle->MeshRender()->SetMaterial(pMtrl);
			pColObstacle->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());


			pObstacle->Transform()->SetLocalPos(Vec3(2725, 80.f, 13400.f));      // R2기준 (125, -1100)
			pObstacle->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pColObstacle->Transform()->SetLocalPos(Vec3(2725, 80.f, 13400.f));
			pColObstacle->Transform()->SetLocalRot(pObstacle->Transform()->GetLocalRot());
			pColObstacle->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pColObstacle->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 230.f));
			pColObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 80.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-39.f));
			pColObstacle->Transform()->SetQuaternion(qRot);
			FindLayer(L"Obstacle")->AddGameObject(pColObstacle);
			break;
		case 1:
			// R1 앞
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Obstacle01.mdat", L"MeshData\\Obstacle01.mdat");
			pObstacle = pMeshData->Instantiate();
			pColObstacle = new CGameObject;
			pColObstacle->SetName(L"obstacle");
			pColObstacle->AddComponent(new CCollider3D);
			pColObstacle->AddComponent(new CMeshRender);
			pColObstacle->AddComponent(new CTransform);
			pColObstacle->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pColObstacle->MeshRender()->SetMaterial(pMtrl);
			pColObstacle->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());

			pObstacle->Transform()->SetLocalPos(Vec3(-350.f, 80.f, 10600.f));      // R1 기준 (-300, -900)
			pObstacle->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			pColObstacle->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pColObstacle->Transform()->SetLocalPos(Vec3(-350.f, 80.f, 10600.f));

			pColObstacle->Transform()->SetLocalRot(pObstacle->Transform()->GetLocalRot());
			pObstacle->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pColObstacle->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 230.f));
			pColObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 80.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-43.f));
			pColObstacle->Transform()->SetQuaternion(qRot);
			FindLayer(L"Obstacle")->AddGameObject(pColObstacle);
			break;
		case 2:
			// B1 앞
			pColObstacle = new CGameObject;
			pColObstacle->SetName(L"obstacle");
			pColObstacle->AddComponent(new CCollider3D);
			pColObstacle->AddComponent(new CMeshRender);
			pColObstacle->AddComponent(new CTransform);
			pColObstacle->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pColObstacle->MeshRender()->SetMaterial(pMtrl);
			pColObstacle->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());

			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Obstacle01.mdat", L"MeshData\\Obstacle01.mdat");
			pObstacle = pMeshData->Instantiate();
			pColObstacle->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObstacle->Transform()->SetLocalPos(Vec3(2500.f, 80.f, 9700.f));         // B1 기준 (500, 700)
			pObstacle->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			pColObstacle->Transform()->SetLocalPos(pObstacle->Transform()->GetLocalPos());
			pColObstacle->Transform()->SetLocalRot(pObstacle->Transform()->GetLocalRot());
			pObstacle->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pColObstacle->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 230.f));
			pColObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 80.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-35.f));
			pColObstacle->Transform()->SetQuaternion(qRot);
			FindLayer(L"Obstacle")->AddGameObject(pColObstacle);
			break;
		case 3:
			// B2 앞
			pColObstacle = new CGameObject;
			pColObstacle->SetName(L"obstacle");
			pColObstacle->AddComponent(new CCollider3D);
			pColObstacle->AddComponent(new CMeshRender);
			pColObstacle->AddComponent(new CTransform);
			pColObstacle->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pColObstacle->MeshRender()->SetMaterial(pMtrl);
			pColObstacle->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Obstacle01.mdat", L"MeshData\\Obstacle01.mdat");
			pObstacle = pMeshData->Instantiate();
			pColObstacle->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);

			pObstacle->Transform()->SetLocalPos(Vec3(-1100.f, 80.f, 6800.f));      // B2 기준 (-600, 1300)
			pObstacle->Transform()->SetLocalRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pColObstacle->Transform()->SetLocalPos(pObstacle->Transform()->GetLocalPos());
			pColObstacle->Collider3D()->SetOffsetScale(Vec3(150.f, 150.f, 230.f));
			pColObstacle->Transform()->SetLocalRot(pObstacle->Transform()->GetLocalRot());
			pColObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 80.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(31.f));
			pColObstacle->Transform()->SetQuaternion(qRot);
			FindLayer(L"Obstacle")->AddGameObject(pColObstacle);
			break;
		case 4:
			// R2 뒤
			pColObstacle = new CGameObject;
			pColObstacle->SetName(L"obstacle");
			pColObstacle->AddComponent(new CCollider3D);
			pColObstacle->AddComponent(new CMeshRender);
			pColObstacle->AddComponent(new CTransform);
			pColObstacle->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pColObstacle->MeshRender()->SetMaterial(pMtrl);
			pColObstacle->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());

			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\obstacle_02.mdat", L"MeshData\\obstacle_02.mdat");
			pObstacle = pMeshData->Instantiate();
			pColObstacle->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObstacle->Transform()->SetLocalPos(Vec3(3000.f, 127.f, 15600.f));      // R2 기준 (525, 1100)
			pObstacle->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(3.f, 2.f, 2.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(75.f));
			pColObstacle->Transform()->SetLocalPos(pObstacle->Transform()->GetLocalPos());
			pColObstacle->Collider3D()->SetOffsetScale(Vec3(520.f, 280.f, 240.f));
			pColObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, -55.f, 0.f));
			pColObstacle->Transform()->SetQuaternion(qRot);
			FindLayer(L"Obstacle")->AddGameObject(pColObstacle);
			break;
		case 5:
			// R1 뒤
			pColObstacle = new CGameObject;
			pColObstacle->SetName(L"obstacle");
			pColObstacle->AddComponent(new CCollider3D);
			pColObstacle->AddComponent(new CMeshRender);
			pColObstacle->AddComponent(new CTransform);
			pColObstacle->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pColObstacle->MeshRender()->SetMaterial(pMtrl);
			pColObstacle->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\obstacle_02.mdat", L"MeshData\\obstacle_02.mdat");
			pObstacle = pMeshData->Instantiate();
			pColObstacle->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObstacle->Transform()->SetLocalPos(Vec3(-500.f, 127.f, 12700.f));      // R1 기준 (-500, 1200)
			pObstacle->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(3.f, 2.f, 2.f));
			pColObstacle->Transform()->SetLocalPos(pObstacle->Transform()->GetLocalPos());
			pColObstacle->Collider3D()->SetOffsetScale(Vec3(520.f, 280.f, 240.f));
			pColObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, -55.f, 0.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(72.f));
			pColObstacle->Transform()->SetQuaternion(qRot);
			FindLayer(L"Obstacle")->AddGameObject(pColObstacle);
			break;
		case 6:
			// B1 뒤
			pColObstacle = new CGameObject;
			pColObstacle->SetName(L"obstacle");
			pColObstacle->AddComponent(new CCollider3D);
			pColObstacle->AddComponent(new CMeshRender);
			pColObstacle->AddComponent(new CTransform);
			pColObstacle->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pColObstacle->MeshRender()->SetMaterial(pMtrl);
			pColObstacle->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\obstacle_02.mdat", L"MeshData\\obstacle_02.mdat");
			pObstacle = pMeshData->Instantiate();
			pColObstacle->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObstacle->Transform()->SetLocalPos(Vec3(2700.f, 127.f, 7700.f));      // B1 기준 (700, -1300)
			pObstacle->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(3.f, 2.f, 2.f));
			pColObstacle->Transform()->SetLocalPos(pObstacle->Transform()->GetLocalPos());
			pColObstacle->Collider3D()->SetOffsetScale(Vec3(520.f, 280.f, 240.f));
			pColObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, -55.f, 0.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(60.f));
			pColObstacle->Transform()->SetQuaternion(qRot);
			FindLayer(L"Obstacle")->AddGameObject(pColObstacle);
			break;
		case 7:
			// B2 뒤
			pColObstacle = new CGameObject;
			pColObstacle->SetName(L"obstacle");
			pColObstacle->AddComponent(new CCollider3D);
			pColObstacle->AddComponent(new CMeshRender);
			pColObstacle->AddComponent(new CTransform);
			pColObstacle->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pColObstacle->MeshRender()->SetMaterial(pMtrl);
			pColObstacle->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
			pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\obstacle_02.mdat", L"MeshData\\obstacle_02.mdat");
			pObstacle = pMeshData->Instantiate();
			pColObstacle->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
			pObstacle->Transform()->SetLocalPos(Vec3(-1200.f, 127.f, 4700.f));      // B2 기준 (-675, -800)
			pObstacle->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
			pObstacle->Transform()->SetLocalScale(Vec3(3.f, 2.f, 2.f));
			pColObstacle->Transform()->SetLocalPos(pObstacle->Transform()->GetLocalPos());
			pColObstacle->Collider3D()->SetOffsetScale(Vec3(520.f, 280.f, 240.f));
			pColObstacle->Collider3D()->SetOffsetPos(Vec3(0.f, -55.f, 0.f));
			qRot = Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-27.f));
			pColObstacle->Transform()->SetQuaternion(qRot);
			FindLayer(L"Obstacle")->AddGameObject(pColObstacle);
			break;
		}
		pObstacle->SetName(L"obstacle");
		pObstacle->FrustumCheck(false);
		pObstacle->MeshRender()->SetDynamicShadow(true);

		pObstacle->Transform()->SetQuaternion(qRot);
		FindLayer(L"Obstacle")->AddGameObject(pObstacle);

	}


	Ptr<CTexture> pSky01 = CResMgr::GetInst()->FindRes<CTexture>(L"Sky01");
	
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

	tResolution res = CRenderMgr::GetInst()->GetResolution();



	// 크로스헤어
	CGameObject* pUICrossHair = new CGameObject;
	pUICrossHair->SetName(L"UICrossHair");
	pUICrossHair->FrustumCheck(false);
	pUICrossHair->AddComponent(new CTransform);
	pUICrossHair->AddComponent(new CMeshRender);
	//pUICrossHair->AddComponent(new CStaticUI);
	pUICrossHair->AddComponent(new CCrossHairScript);

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


	CGameObject* pUISkill = new CGameObject;
	pUISkill->SetName(L"UISkill1");
	pUISkill->FrustumCheck(false);
	pUISkill->AddComponent(new CTransform);
	pUISkill->AddComponent(new CMeshRender);

	Vec3 vESkillScale = Vec3(res.fWidth / 18, res.fWidth / 18, 1.f);
	Vec3 vZSkillScale = Vec3(res.fWidth / 13, res.fWidth / 13, 1.f);

	pUISkill->Transform()->SetLocalPos(Vec3(res.fWidth / 2 - vESkillScale.x * 0.5f - vZSkillScale.x - res.fWidth / 30, -res.fHeight / 2 + vESkillScale.y / 2, 1.f));
	pUISkill->Transform()->SetLocalScale(vESkillScale);


	pUISkill->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pSkillMtrl = new CMaterial;
	pSkillMtrl->DisableFileSave();
	pSkillMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexUIShader"));
	CResMgr::GetInst()->AddRes(L"SkillTexMtrl", pSkillMtrl);
	pSkillMtrl->SetData(SHADER_PARAM::TEX_0,
		pPlayer->GetScript<CPlayerScript>()->GetESkill()->pTex);
	pUISkill->MeshRender()->SetMaterial(pSkillMtrl);
	pPlayer->GetScript<CPlayerScript>()->SetESkillObj(pUISkill);
	FindLayer(L"UI")->AddGameObject(pUISkill);

	//add 추가
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	CResMgr::GetInst()->AddRes(L"Texture00", pMtrl);
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	CResMgr::GetInst()->AddRes(L"Texture01", pMtrl);

	pUISkill = new CGameObject;
	pUISkill->SetName(L"UISkill2");
	pUISkill->FrustumCheck(false);
	pUISkill->AddComponent(new CTransform);
	pUISkill->AddComponent(new CMeshRender);

	pUISkill->Transform()->SetLocalPos(Vec3(res.fWidth / 2 - vZSkillScale.x * 0.5, -res.fHeight / 2 + vZSkillScale.y / 2, 1.f));
	pUISkill->Transform()->SetLocalScale(vZSkillScale);

	pUISkill->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pSkillMtrl = new CMaterial;
	pSkillMtrl->DisableFileSave();
	pSkillMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexUIShader"));
	CResMgr::GetInst()->AddRes(L"SkillTex2Mtrl", pSkillMtrl);
	pSkillMtrl->SetData(SHADER_PARAM::TEX_0,
		pPlayer->GetScript<CPlayerScript>()->GetZSkill()->pTex);
	pUISkill->MeshRender()->SetMaterial(pSkillMtrl);
	pPlayer->GetScript<CPlayerScript>()->SetZSkillObj(pUISkill);
	FindLayer(L"UI")->AddGameObject(pUISkill);

	// 스킬쿨타임 글씨 수민
	CFontMgr::GetInst()->AddText(L"Eskill", L"E", Vec2(0.8f, 0.95f), Vec2(3.f, 3.f), Vec2(0.5f, 0.f), Vec4(1.f, 1.f, 1.f, 1.f));
	CFontMgr::GetInst()->AddText(L"Zskill", L"Z", Vec2(0.9f, 0.95f), Vec2(3.f, 3.f), Vec2(0.5f, 0.f), Vec4(1.f, 1.f, 1.f, 1.f));




	Vec3 vPlayerTexScale = Vec3(res.fWidth / 25.f, res.fHeight / 14.f, 1.f);
	Vec3 vHPBarScale = Vec3(res.fWidth / 15.f, res.fHeight / 44.f, 1.f);
	CGameObject* pPlayerInfoTex;
	CGameObject* pHpBar;

	int i = 0;
	for (auto tPlayer : TeamPlayer) {
		// 사진
		pPlayerInfoTex = new CGameObject;
		pPlayerInfoTex->SetName(L"PlayerInfoTex");
		pPlayerInfoTex->FrustumCheck(false);
		pPlayerInfoTex->AddComponent(new CTransform);
		pPlayerInfoTex->AddComponent(new CMeshRender);

		pPlayerInfoTex->Transform()->SetLocalPos(Vec3(-res.fWidth / 2.f + vPlayerTexScale.x / 2 + 10.f, 0.f + vPlayerTexScale.y / 2 - res.fHeight / 11 * i, 1.f));
		pPlayerInfoTex->Transform()->SetLocalScale(vPlayerTexScale);

		pPlayerInfoTex->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
		Ptr<CMaterial> pPlayerTexMtrl = new CMaterial;
		pPlayerTexMtrl->DisableFileSave();
		pPlayerTexMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
		pPlayerInfoTex->MeshRender()->SetMaterial(pPlayerTexMtrl);

		//아군플레어
		int iType = tPlayer->GetScript<CPlayerScript>()->GetType();
		Ptr<CTexture> pPlayerTexTex;

		if (tPlayer->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::BLUE) {
			switch (iType) {
			case 0:
				pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_01_Blue");
				break;
			case 1:
				pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_02_Blue");
				break;
			case 2:
				pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_03_Blue");
				break;
			case 3:
				pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_04_Blue");
				break;
			case 4:
				pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_05_Blue");
				break;

			}
		}
		else if(tPlayer->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::RED) {
			switch (iType) {
			case 0:
				pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_01_Red");
				break;
			case 1:
				pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_02_Red");
				break;
			case 2:
				pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_03_Red");
				break;
			case 3:
				pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_04_Red");
				break;
			case 4:
				pPlayerTexTex = CResMgr::GetInst()->FindRes<CTexture>(L"helmet_05_Red");
				break;
			}
		}
		pPlayerInfoTex->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pPlayerTexTex.GetPointer());
		FindLayer(L"UI")->AddGameObject(pPlayerInfoTex);
		int index = tPlayer->GetScript<CPlayerScript>()->m_GetId();
		// 플레이어 id
		CFontMgr::GetInst()->AddText(wstring(L"PlayerID") + to_wstring(index), 
			wstring(L"PlayerID"), Vec2(0.05f, 0.44f + 0.09f * i), Vec2(1.5f, 1.5f), Vec2(0.5f, 0.f), Vec4(1.f, 0.f, 1.f, 1.f));
			//위에있는거에 이름 넣어주기

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
		tPlayer->GetScript<CPlayerScript>()->AddUIHpBarObj(pHpBar);
		FindLayer(L"UI")->AddGameObject(pHpBar);
		i++;
	}

//-----------------------------------------------------------------------------------------------
// 조명 추가
//------------------------------------------------------------------------------------------------

	//for (int i = 0; i < 2; i++) {
	//	pObject = new CGameObject;
	//	pObject->SetName(wstring(L"Light") + to_wstring(i + 1));
	//	pObject->AddComponent(new CTransform);
	//	pObject->AddComponent(new CLight3D);
	//	pObject->Light3D()->SetLightPos(Vec3(1000.f + 2000.f * i, 1000.f, -1500.f));
	//	pObject->Light3D()->SetLightType(LIGHT_TYPE::DIR);
	//	pObject->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
	//	pObject->Light3D()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
	//	pObject->Light3D()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
	//	pObject->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
	//	pObject->Light3D()->SetLightRange(1000.f);
	//	FindLayer(L"Default")->AddGameObject(pObject);
	//}

	// 세이프존

	CGameObject* pRangeObject = new CGameObject;
	pRangeObject->AddComponent(new CTransform);
	pRangeObject->AddComponent(new CMeshRender);
	pRangeObject->AddComponent(new CCollider3D);
	pRangeObject->AddComponent(new CSafeZone);
	pRangeObject->SetName(L"SafeZone");
	pRangeObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRangeObject->Transform()->SetLocalPos(Vec3(850.f, 15.f, 800.f));
	pRangeObject->Transform()->SetLocalScale(Vec3(3200.f, 1.f, 1300.f));
	pRangeObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pRangeObject->Collider3D()->SetOffsetScale(Vec3(1.f, 100.f, 1.f));
	pRangeObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"Rect2Mesh"));

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	pRangeObject->MeshRender()->SetMaterial(pMtrl);
	pRangeObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Stop").GetPointer());
	pRangeObject->GetScript<CSafeZone>()->SetCampState(CAMP_STATE::BLUE);
	FindLayer(L"Default")->AddGameObject(pRangeObject);

	pRangeObject = new CGameObject;
	pRangeObject->AddComponent(new CTransform);
	pRangeObject->AddComponent(new CMeshRender);
	pRangeObject->AddComponent(new CCollider3D);
	pRangeObject->AddComponent(new CSafeZone);
	pRangeObject->SetName(L"SafeZone");
	pRangeObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pRangeObject->Transform()->SetLocalPos(Vec3(900.f, 15.f, 19100.f));
	pRangeObject->Transform()->SetLocalScale(Vec3(3200.f, 1.f, 1300.f));
	pRangeObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pRangeObject->Collider3D()->SetOffsetScale(Vec3(1.f, 100.f, 1.f));
	pRangeObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"Rect2Mesh"));

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	pRangeObject->MeshRender()->SetMaterial(pMtrl);
	pRangeObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Stop").GetPointer());

	pRangeObject->GetScript<CSafeZone>()->SetCampState(CAMP_STATE::RED);
	FindLayer(L"Default")->AddGameObject(pRangeObject);


	Ptr<CMaterial> m_pMtrl = new CMaterial;
	m_pMtrl->SetName(L"Texture00");
	m_pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	CResMgr::GetInst()->AddRes<CMaterial>(m_pMtrl->GetName(), m_pMtrl);
	m_pMtrl = new CMaterial;
	m_pMtrl->SetName(L"Texture01");
	m_pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	CResMgr::GetInst()->AddRes<CMaterial>(m_pMtrl->GetName(), m_pMtrl);


	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Blue");
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Red");
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Red");
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Cover");
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Red");

	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Arrow");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Arrow");

	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Blue", L"Box");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Red", L"Box");

	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Box", L"Arrow");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Obstacle", L"Arrow");


	CSensorMgr::GetInst()->CheckSensorLayer(L"Monster", L"Blue");
	CSensorMgr::GetInst()->CheckSensorLayer(L"Blue", L"Red");

	CSpawnMgr::GetInst()->Init();
	
	//cout << "Init ----!!" << endl;
	//Awake();
	//Start();
	//cout << "DONE" << endl;

}
