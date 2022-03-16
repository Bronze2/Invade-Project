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

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Camera.h"

#include "CollisionMgr.h"
#include "EventMgr.h"
#include "RenderMgr.h"
#include "Device.h"

#include "GridScript.h"
#include "ToolCamScript.h"
#include "PlayerScript.h"
#include "MonsterScript.h"
#include "ParticleSystem.h"
#include "ArrowScript.h"
#include "GameFramework.h"

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
	// 필요한 리소스 로딩
	// Texture 로드

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

	m_pCurScene = new CScene;
	m_pCurScene->SetName(L"Test Scene");

	m_pCurScene->GetLayer(0)->SetName(L"Default");
	m_pCurScene->GetLayer(1)->SetName(L"Player");
	m_pCurScene->GetLayer(2)->SetName(L"Monster");
	m_pCurScene->GetLayer(3)->SetName(L"Arrow");
	m_pCurScene->GetLayer(31)->SetName(L"Tool");

	CGameObject* pMainCam = nullptr;

	// Camera Object
	pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CToolCamScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	//pMainCam->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pMainCam->Camera()->SetFar(100000.f);
	pMainCam->Camera()->SetLayerAllCheck();

	m_pCurScene->FindLayer(L"Default")->AddGameObject(pMainCam);

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



	
//	pMeshData->Save(pMeshData->GetPath());
	// MeshData 로드




	// Temp Object
	pObject = new CGameObject;
	pObject->SetName(L"Player Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	// Transform 설정
	pObject->Transform()->SetLocalPos(Vec3(0.f, 100.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 100.f));
	pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());
	pObject->MeshRender()->SetDynamicShadow(true);

	// Script 설정
//	pObject->AddComponent(new CPlayerScript);

	// AddGameObject
	m_pCurScene->FindLayer(L"Player")->AddGameObject(pObject);

//	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX( L"FBX\\monster.fbx");
//	pMeshData->Save(pMeshData->GetPath())
//	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\test_sibal.fbx");
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\monster.mdat", L"MeshData\\monster.mdat");




	pObject = pMeshData->Instantiate();
	pObject->SetName(L"Monster");

	pObject->AddComponent(new CCollider3D);

	pObject->AddComponent(new CPlayerScript);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(10.f,40.f,10.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(50.f, 100.f, 100.f));
	pObject->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));
	pObject->MeshRender()->SetDynamicShadow(true);
	pObject->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::FROZEN);
	
	pMainCam->Transform()->SetLocalPos(Vec3(-60,40,-10));
	pMainCam->Transform()->SetLocalRot(Vec3(0, PI/2, -PI/18));
	
	pObject->AddChild(pMainCam);


	m_pCurScene->FindLayer(L"Monster")->AddGameObject(pObject, false);
	

	pObject = new CGameObject;
	pObject->SetName(L"Particle");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CParticleSystem);
	pObject->ParticleSystem()->Init((CResMgr::GetInst()->FindRes<CTexture>(L"Snow")));
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(500.f, 0.f, 0.f));

	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	
	pObject = new CGameObject;
	pObject->SetName(L"SkyBox");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
//	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky01.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky02.GetPointer());

	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CGameObject * m_pArrow = new CGameObject;
	m_pArrow->SetName(L"Arrow");


		m_pArrow->AddComponent(new CTransform());
//		Vec3 vPos = m_pArrow->Transform()->GetLocalPos();
		m_pArrow->Transform()->SetLocalPos(Vec3(50,150,100));
		m_pArrow->Transform()->SetLocalScale(Vec3(100.f, 1.f, 1.f));
//
		m_pArrow->AddComponent(new CMeshRender);
		m_pArrow->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
		CMaterial* pMaterial = new CMaterial;
		NewMaterial(pMaterial,L"Std3DShader",L"Black",L"Material\\Black.mtrl");
		
		m_pArrow->MeshRender()->SetMaterial(pMaterial);
		m_pArrow->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlackTex.GetPointer());
//
		m_pArrow->AddComponent(new CCollider3D);
		m_pArrow->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
//
		m_pArrow->AddComponent(new CArrowScript(ELEMENT_TYPE::DARK));
		pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow);
		m_pArrow->GetScript<CArrowScript>()->SetMove(false);






	int i = 0;
	Ptr<CMaterial> pCSMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"CSTestMtrl");
	pCSMtrl->SetData(SHADER_PARAM::INT_0, &i);
	CDevice::GetInst()->SetUAVToRegister_CS(pTestUAVTexture.GetPointer(), UAV_REGISTER::u0);

	pCSMtrl->Dispatch(1, 1024, 1);



	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Monster");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Arrow", L"Monster");
	m_pCurScene->Awake();
	m_pCurScene->Start();
}

void CSceneMgr::Update()
{
	m_pCurScene->Update();
	m_pCurScene->LateUpdate();

	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();

	m_pCurScene->FinalUpdate();

	// 충돌 처리
	CCollisionMgr::GetInst()->Update();
}

void CSceneMgr::Update_Tool()
{
	// rendermgr 카메라 초기화
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

