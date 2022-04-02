#include "pch.h"

#include "SceneMgr.h"

#include "Scene.h"
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

#include "GridScript.h"
#include "CameraScript.h"
#include "PlayerScript.h"
#include "MonsterScript.h"
#include "ParticleSystem.h"
#include "ArrowScript.h"
#include "CMinionScript.h"
#include "GameFramework.h"
#include "Animation3D.h"

CScene* CSceneMgr::GetCurScene()
{
	return m_pCurScene;
}

void CSceneMgr::ChangeScene(CScene* _pNextScene)
{
	SAFE_DELETE(m_pCurScene);
	m_pCurScene = _pNextScene;
}

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}

void CSceneMgr::Init()
{
	// �ʿ��� ���ҽ� �ε�
	// Texture �ε�

	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"TestTex", L"Texture\\Health.png");
	Ptr<CTexture> pExplosionTex = CResMgr::GetInst()->Load<CTexture>(L"Explosion", L"Texture\\Explosion\\Explosion80.png");
	Ptr<CTexture> pBlackTex = CResMgr::GetInst()->Load<CTexture>(L"Black", L"Texture\\asd.png");
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky01.png");
	Ptr<CTexture> pSky02 = CResMgr::GetInst()->Load<CTexture>(L"Sky02", L"Texture\\Skybox\\Sky02.jpg");
	CResMgr::GetInst()->Load<CTexture>(L"Snow", L"Texture\\Particle\\Snow50px.png");
	CResMgr::GetInst()->Load<CTexture>(L"smokeparticle", L"Texture\\Particle\\smokeparticle.png");
	CResMgr::GetInst()->Load<CTexture>(L"HardCircle", L"Texture\\Particle\\HardCircle.png");
	CResMgr::GetInst()->Load<CTexture>(L"particle_00", L"Texture\\Particle\\particle_00.png");
	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Tile\\TILE_03.tga");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>(L"Tile_n", L"Texture\\Tile\\TILE_03_N.tga");

	Ptr<CTexture> pDiffuseTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex");
	Ptr<CTexture> pNormalTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex");
	Ptr<CTexture> pPositionTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex");

	Ptr<CTexture> pTestUAVTexture = CResMgr::GetInst()->CreateTexture(L"UAVTexture", 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	Ptr<CMaterial> pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_3, pSky01.GetPointer());
//
	pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_2, pSky01.GetPointer());
//
	Ptr<CTexture> pTreeTex1= CResMgr::GetInst()->Load<CTexture>(L"TreeTex1", L"Texture\\Tree01.png");
	Ptr<CTexture> pTreeTex2 =CResMgr::GetInst()->Load<CTexture>(L"TreeTex2", L"Texture\\Tree02.png");

	m_pCurScene = new CScene;
	m_pCurScene->SetName(L"Play Scene");

	m_pCurScene->GetLayer(0)->SetName(L"Default");
	m_pCurScene->GetLayer(1)->SetName(L"Player");
	m_pCurScene->GetLayer(2)->SetName(L"Monster");
	m_pCurScene->GetLayer(3)->SetName(L"Arrow");
	m_pCurScene->GetLayer(4)->SetName(L"Minion");
	m_pCurScene->GetLayer(5)->SetName(L"Tower");

	m_pCurScene->GetLayer(6)->SetName(L"Terrain");

	m_pCurScene->GetLayer(31)->SetName(L"Tool");

	CGameObject* pMainCam = nullptr;

	// Camera Object
	pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	//pMainCam->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pMainCam->Camera()->SetFar(100000.f);
	pMainCam->Camera()->SetLayerAllCheck();

	m_pCurScene->FindLayer(L"Default")->AddGameObject(pMainCam);


	CRenderMgr::GetInst()->SetCamera(pMainCam->Camera());


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
	pObject->Transform()->SetLocalPos(Vec3(-1000.f, 1000.f, -1000.f));
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	pObject = new CGameObject;
	pObject->SetName(L"Terrain");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CTerrain);
	
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(0.f, 100.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(50.f, 400.f, 50.f));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TerrainMtrl"));
	pObject->Terrain()->Init();
	
	
	m_pCurScene->FindLayer(L"Terrain")->AddGameObject(pObject);

	
//	pMeshData->Save(pMeshData->GetPath());
	// MeshData �ε�




	// Temp Object
	pObject = new CGameObject;
	pObject->SetName(L"Player Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	// Transform ����
	pObject->Transform()->SetLocalPos(Vec3(0.f, 100.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 100.f));
	pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	// MeshRender ����
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());
	pObject->MeshRender()->SetDynamicShadow(true);

	m_pCurScene->FindLayer(L"Player")->AddGameObject(pObject);

	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\monster.mdat", L"MeshData\\monster.mdat");


	pObject = new CGameObject;

	pObject = pMeshData->Instantiate();
	pObject->SetName(L"Monster");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);

	pObject->AddComponent(new CPlayerScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f,40.f,10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(50.f, 100.f, 100.f));
	pObject->Transform()->SetLocalScale(Vec3(2.f, 2.0f, 2.0f));
	pObject->MeshRender()->SetDynamicShadow(true);
	pObject->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::FROZEN);
	
	pMainCam->Transform()->SetLocalPos(Vec3(-60,40,-10));
//	pMainCam->Transform()->SetLocalScale(Vec3(15000.f, 15000.f, 15000.f));
	pMainCam->Transform()->SetLocalRot(Vec3(0, PI/2, -PI/18));
	
	pObject->AddChild(pMainCam);
//
//
	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject, false);


	 pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Canon_min.mdat", L"MeshData\\Canon_min.mdat");
	 pObject = pMeshData->Instantiate();

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CMinionScript);
;
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(150.f, 100.f, 150.f));
	pObject->Transform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
	pObject->MeshRender()->SetDynamicShadow(true);

	CAnimation* pNewAnimation =new CAnimation;
	pNewAnimation->InsertAnimation(L"IDLE", 0, 30, true, false);
	pNewAnimation->InsertAnimation(L"WALK", 31, 54, false, false);
	pNewAnimation->InsertAnimation(L"ATTACK", 80, 104, false, false);
	pNewAnimation->InsertAnimation(L"DIE", 105, 154, false, false);
	pObject->Animator3D()->SetAnimation(pNewAnimation);
	pObject->GetScript<CMinionScript>()->Init();

	m_pCurScene->FindLayer(L"Minion")->AddGameObject(pObject);


	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Canon_min1.mdat", L"MeshData\\Canon_min1.mdat");
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CMinionScript);
	;
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(200.f, 100.f, 150.f));
	pObject->Transform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
	pObject->MeshRender()->SetDynamicShadow(true);
	pNewAnimation = new CAnimation;
	pNewAnimation->InsertAnimation(L"IDLE", 0, 39, true, false);
	pNewAnimation->InsertAnimation(L"WALK", 41, 74, false, false);
	pNewAnimation->InsertAnimation(L"ATTACK", 75, 99, false, false);
	pNewAnimation->InsertAnimation(L"DIE", 125, 174, false, false);
	pObject->Animator3D()->SetAnimation(pNewAnimation);
	pObject->GetScript<CMinionScript>()->Init();



	m_pCurScene->FindLayer(L"Minion")->AddGameObject(pObject);

	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\sword_min.mdat", L"MeshData\\sword_min.mdat");
//	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Ghost.mdat", L"MeshData\\Ghost.mdat");
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CMinionScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(250.f, 100.f, 150.f));
	pObject->Transform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
	pObject->MeshRender()->SetDynamicShadow(true);
	pNewAnimation = new CAnimation;
	pNewAnimation->InsertAnimation(L"IDLE", 0, 34, true, false);
	pNewAnimation->InsertAnimation(L"WALK", 35, 49, false, false);
	pNewAnimation->InsertAnimation(L"ATTACK", 50, 74, false, false);
	pNewAnimation->InsertAnimation(L"DIE", 75, 124, false, false);
	pObject->Animator3D()->SetAnimation(pNewAnimation);
	pObject->GetScript<CMinionScript>()->Init();

	m_pCurScene->FindLayer(L"Minion")->AddGameObject(pObject);


	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\sword_min1.mdat", L"MeshData\\sword_min1.mdat");
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CMinionScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(300.f, 100.f, 150.f));
	pObject->Transform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
	pObject->MeshRender()->SetDynamicShadow(true);
	pNewAnimation = new CAnimation;

	pNewAnimation->InsertAnimation(L"IDLE", 0, 29, true, false);
	pNewAnimation->InsertAnimation(L"WALK", 30, 49, false, false);
	pNewAnimation->InsertAnimation(L"ATTACK", 50, 74, false, false);
	pNewAnimation->InsertAnimation(L"DIE", 75, 114, false, false);
	pObject->Animator3D()->SetAnimation(pNewAnimation);
	pObject->GetScript<CMinionScript>()->Init();
//
	m_pCurScene->FindLayer(L"Minion")->AddGameObject(pObject);

//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\wizard_min.fbx");
//	pMeshData->Save(pMeshData->GetPath());
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\wizard_min.mdat", L"MeshData\\wizard_min.mdat");
	pObject = pMeshData->Instantiate();

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CMinionScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(350.f, 100.f, 150.f));
	pObject->Transform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
	pObject->MeshRender()->SetDynamicShadow(true);
	pNewAnimation = new CAnimation;

	pNewAnimation->InsertAnimation(L"IDLE", 0, 34, true, false);
	pNewAnimation->InsertAnimation(L"WALK", 35, 59, false, false);
	pNewAnimation->InsertAnimation(L"ATTACK", 60, 84, false, false);
	pNewAnimation->InsertAnimation(L"DIE", 110, 159, false, false);
	pObject->Animator3D()->SetAnimation(pNewAnimation);
	pObject->GetScript<CMinionScript>()->Init();
	m_pCurScene->FindLayer(L"Minion")->AddGameObject(pObject);


	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\wizard_min1.mdat", L"MeshData\\wizard_min1.mdat");
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->AddComponent(new CMinionScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(400.f, 100.f, 150.f));
	pObject->Transform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
	pObject->MeshRender()->SetDynamicShadow(true);
	pNewAnimation = new CAnimation;

	pNewAnimation->InsertAnimation(L"IDLE", 0, 29, true, false);
	pNewAnimation->InsertAnimation(L"WALK", 30, 49, false, false);
	pNewAnimation->InsertAnimation(L"ATTACK", 50, 74, false, false);
	pNewAnimation->InsertAnimation(L"DIE", 100, 149, false, false);
	pObject->Animator3D()->SetAnimation(pNewAnimation);
	pObject->GetScript<CMinionScript>()->Init();

	m_pCurScene->FindLayer(L"Minion")->AddGameObject(pObject);


    pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\FirstTower.mdat", L"MeshData\\FirstTower.mdat");

	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(150.f, 100.f, 200.f)); 
	pObject->Transform()->SetLocalRot(Vec3(-3.14f/2, 0.f, 0.f));

	pObject->Transform()->SetLocalScale(Vec3(7.f, 7.f, 7.f));
	pObject->MeshRender()->SetDynamicShadow(true);



	m_pCurScene->FindLayer(L"Tower")->AddGameObject(pObject);

//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\SecondTower.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\SecondTower.mdat", L"MeshData\\SecondTower.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 40.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(150.f, 100.f, 400.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->MeshRender()->SetDynamicShadow(true);


	m_pCurScene->FindLayer(L"Tower")->AddGameObject(pObject);

//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\FirstTower01.fbx");
    pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\FirstTower01.mdat", L"MeshData\\FirstTower01.mdat");
//	pMeshData->Save(pMeshData->GetPath());
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 10.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(150.f, 100.f, -200.f));
	pObject->Transform()->SetLocalRot(Vec3(-3.14f / 2, 3.14f, 0.f));

	pObject->Transform()->SetLocalScale(Vec3(7.f, 7.f, 7.f));
	pObject->MeshRender()->SetDynamicShadow(true);
	//pObject->Animator3D()->SetClipIndex(1);


	m_pCurScene->FindLayer(L"Tower")->AddGameObject(pObject);

	//pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\SecondTower01.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\SecondTower01.mdat", L"MeshData\\SecondTower01.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f, 10.f, 10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(150.f, 100.f, -400.f));
	pObject->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->MeshRender()->SetDynamicShadow(true);


	m_pCurScene->FindLayer(L"Tower")->AddGameObject(pObject);


//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Nexus01.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Nexus01.mdat", L"MeshData\\Nexus01.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));
	
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(150.f, 135.f, -700.f));
	pObject->Transform()->SetLocalRot(Vec3(-3.14f/6, 0.F, 0.f));

	pObject->Transform()->SetLocalScale(Vec3(70.f, 70.f, 70.f));
	pObject->MeshRender()->SetDynamicShadow(true);

	m_pCurScene->FindLayer(L"Tower")->AddGameObject(pObject);


//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Nexus.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Nexus.mdat", L"MeshData\\Nexus.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(30.f, 10.f, 50.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(150.f, 135.f, 700.f));
	pObject->Transform()->SetLocalRot(Vec3(-3.14f / 6, 3.14f, 0.f));

	pObject->Transform()->SetLocalScale(Vec3(70.f, 70.f, 70.f));
	pObject->MeshRender()->SetDynamicShadow(true);
	m_pCurScene->FindLayer(L"Tower")->AddGameObject(pObject);

//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Cover.fbx");
	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cover.mdat", L"MeshData\\Cover.mdat");
	//pMeshData->Save(pMeshData->GetPath());
	pObject = pMeshData->Instantiate();
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(-100.f, 100.f, 150.f));
	pObject->Transform()->SetLocalScale(Vec3(70.f, 70.f, 70.f));
	pObject->MeshRender()->SetDynamicShadow(true);
	m_pCurScene->FindLayer(L"Tower")->AddGameObject(pObject);

	pObject = new CGameObject;
	pObject->SetName(L"Tree");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(-200.f, 150.f, 150.f));
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

	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);
	
	pObject = new CGameObject;
	pObject->SetName(L"Tree");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(-250.f, 150.f, 150.f));
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
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);


	
	
	pObject = new CGameObject;
	pObject->SetName(L"SkyBox");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky02.GetPointer());

	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	




	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Monster");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Tower");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Arrow", L"Monster");
	m_pCurScene->Awake();
	m_pCurScene->Start();
}

void CSceneMgr::Update()
{
	m_pCurScene->Update();
	m_pCurScene->LateUpdate();

	// rendermgr ī�޶� �ʱ�ȭ
	CRenderMgr::GetInst()->ClearCamera();
	//CRenderMgr::GetInst()->RegisterCamera(CRenderMgr::GetInst()->GetCamera());

	m_pCurScene->FinalUpdate();

	// �浹 ó��
	CCollisionMgr::GetInst()->Update();
}

void CSceneMgr::Update_Tool()
{
	// rendermgr ī�޶� �ʱ�ȭ
	CRenderMgr::GetInst()->ClearCamera();
	m_pCurScene->FinalUpdate();
}

void CSceneMgr::FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObject = m_pCurScene->GetLayer(i)->GetObjects();
		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			if (_strTag == vecObject[j]->GetName())
			{
				_vecFindObj.push_back(vecObject[j]);
			}
		}
	}
}

bool Compare(CGameObject* _pLeft, CGameObject* _pRight)
{
	return (_pLeft->Transform()->GetWorldPos().z < _pRight->Transform()->GetWorldPos().z);
}

