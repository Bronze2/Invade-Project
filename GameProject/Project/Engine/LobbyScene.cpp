#include "pch.h"
#include "LobbyScene.h"
#include "Layer.h"
#include "GameObject.h"

#include "Transform.h"
#include "Camera.h"
#include "ResMgr.h"
#include "RenderMgr.h"
#include "KeyMgr.h"
#include "MeshRender.h"
#include "CameraScript.h"
#include "CollisionMgr.h"
#include "ButtonScript.h"
#include "PlayerScript.h"
#include "Animation3D.h"
#include "Animator3D.h"
#include "Light3D.h"
#include "CrossHairScript.h"

void CLobbyScene::Init()
{
	ShowCursor(true);


	GetLayer(0)->SetName(L"Default");
	GetLayer(3)->SetName(L"Blue");
	GetLayer(4)->SetName(L"Red");
	GetLayer(8)->SetName(L"Tile");
	GetLayer(30)->SetName(L"UI");

	tResolution res = CRenderMgr::GetInst()->GetResolution();

//-----------------------------------------------------------------------------------------------
// 카메라
//-----------------------------------------------------------------------------------------------

	CGameObject* pMainCam = nullptr;

	// Camera Object
	pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	pMainCam->Camera()->SetFar(10000.f);
	pMainCam->Camera()->SetLayerAllCheck();

	pMainCam->Transform()->SetLocalPos(Vec3(0, 100, 0));		// -300, 130, -50
	pMainCam->Transform()->SetLocalRot(Vec3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));		// -15.f
	pMainCam->GetScript<CCameraScript>()->SetMoveLobbyCam(false);

	CRenderMgr::GetInst()->SetCamera(pMainCam->Camera());

	FindLayer(L"Default")->AddGameObject(pMainCam);


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

//-----------------------------------------------------------------------------------------------
// 조명
//-----------------------------------------------------------------------------------------------

	CGameObject* pLight = new CGameObject;
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight3D);
	pLight->Light3D()->SetLightPos(Vec3(-3000.f, 5000.f, 0.f));
	pLight->Light3D()->SetLightType(LIGHT_TYPE::DIR);
	pLight->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
	pLight->Light3D()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
	pLight->Light3D()->SetAmbient(Vec3(0.5f, 0.5f, 0.5f));
	pLight->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
	pLight->Light3D()->SetLightRange(10000.f);
	FindLayer(L"Default")->AddGameObject(pLight);

//-----------------------------------------------------------------------------------------------
// 맵
//-----------------------------------------------------------------------------------------------

	//Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\lobby3.fbx");
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\lobby3.mdat", L"MeshData\\lobby3.mdat");
	//pMeshData->Save(pMeshData->GetPath());

	CGameObject* pLobbyObj = pMeshData->Instantiate();

	pLobbyObj->FrustumCheck(false);
	pLobbyObj->Transform()->SetLocalPos(Vec3(-72.f, -100.f, 700.f));		//0.f, 370.f, 0.f
	pLobbyObj->Transform()->SetLocalRot(Vec3(-PI / 2, -PI/2, 0.f));
	pLobbyObj->Transform()->SetLocalScale(Vec3(0.2f, 0.2f, 0.2f));
	pLobbyObj->MeshRender()->SetDynamicShadow(false);

	FindLayer(L"Default")->AddGameObject(pLobbyObj);

//-----------------------------------------------------------------------------------------------
// 플레이어 fbx
//-----------------------------------------------------------------------------------------------

	// Player (helmet - PlayerScript에서)
	Ptr<CMeshData> pPlayerBlueMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player_blue.mdat", L"MeshData\\player_blue.mdat");
	Ptr<CMeshData> pPlayerRedMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player_red.mdat", L"MeshData\\player_red.mdat");
	Ptr<CTexture> pBlueTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerBlue");
	Ptr<CTexture> pRedTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerRed");

	for (int i = 0; i < 8; i++) {
		CGameObject* pPlayerObj = new CGameObject;

		if (i < 4) {
			pPlayerObj = pPlayerBlueMeshData->Instantiate();
			pPlayerObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlueTex.GetPointer());
			pPlayerObj->Transform()->SetLocalPos(Vec3(-400.f + i * 100.f, -70.f, 650.f));	// -525.f + i * 150.f, 0.f, 1000.f

		}
		else {
			pPlayerObj = pPlayerRedMeshData->Instantiate();
			pPlayerObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pRedTex.GetPointer());
			pPlayerObj->Transform()->SetLocalPos(Vec3(100.f + (i - 4) * 100.f, -70.f, 650.f));	// -525.f + i * 150.f, 0.f, 1000.f
		}

		pPlayerObj->SetName(L"Monster");
		pPlayerObj->AddComponent(new CPlayerScript);
		pPlayerObj->FrustumCheck(false);
		pPlayerObj->Transform()->SetLocalScale(Vec3(0.4f, 0.4f, 0.5f));
		pPlayerObj->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), XMConvertToRadians(0.f), 0.f));
		pPlayerObj->MeshRender()->SetDynamicShadow(true);

		CAnimation* pPlayerAnimation = new CAnimation;
		pPlayerAnimation->InsertAnimClip(L"IDLE", 0, 37);
		pPlayerAnimation->InsertAnimClip(L"WALK", 44, 73);
		pPlayerAnimation->InsertAnimClip(L"JUMP", 81, 100);

		pPlayerObj->Animator3D()->SetAnimation(pPlayerAnimation);
		pPlayerObj->Animator3D()->SetAnimClip(pPlayerAnimation->GetAnimClip());

		switch (i % 4) {
		case 0:
			pPlayerObj->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::WATER);
			break;
		case 1:
			pPlayerObj->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::DARK);
			break;
		case 2:
			pPlayerObj->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::WIND);
			break;
		case 3:
			pPlayerObj->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::THUNDER);
			break;
		case 4:
			pPlayerObj->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::FIRE);
			break;
		}

		pPlayerObj->GetScript<CPlayerScript>()->Init();

		if (i < 4) FindLayer(L"Blue")->AddGameObject(pPlayerObj);
		else FindLayer(L"Red")->AddGameObject(pPlayerObj);

		CGameObject* pReadyBarObj = new CGameObject;
		pReadyBarObj->SetName(L"UIReadyBar");
		pReadyBarObj->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pReadyBarObj->AddComponent(new CTransform);
		pReadyBarObj->AddComponent(new CMeshRender);
		pReadyBarObj->SetActive(false);
		
		Vec3 vUIReadyBarScale = Vec3(100.f, 40.f, 1.f);
		pReadyBarObj->Transform()->SetLocalPos(Vec3(0.f, 0.f, 300.f));
		pReadyBarObj->Transform()->SetLocalRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		pReadyBarObj->Transform()->SetLocalScale(vUIReadyBarScale);
		pReadyBarObj->Transform()->SetBillBoard(false);
		pReadyBarObj->Transform()->SetCamera(pMainCam);

		pReadyBarObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pUIReadyBarMtrl = new CMaterial;
		pUIReadyBarMtrl->DisableFileSave();
		pUIReadyBarMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
		pReadyBarObj->MeshRender()->SetMaterial(pUIReadyBarMtrl);
		Ptr<CTexture> pUIReadyBarTex = CResMgr::GetInst()->FindRes<CTexture>(L"UIReadyBar");
		pReadyBarObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pUIReadyBarTex.GetPointer());
		
		pPlayerObj->AddChild(pReadyBarObj);
		FindLayer(L"Blue")->AddGameObject(pReadyBarObj);
	}

//-----------------------------------------------------------------------------------------------
// UI - BUTTON
//-----------------------------------------------------------------------------------------------

	// UI - 화면 중앙이 (0, 0)
	// ^
	// |___>
	// 좌상단 Vec3(-res.fWidth/2, res.fHeight/2, 1.f)

	// Start Button
	CGameObject* pUIButtonStart = new CGameObject;
	pUIButtonStart->SetName(L"StartButton");
	pUIButtonStart->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
	pUIButtonStart->AddComponent(new CTransform);
	pUIButtonStart->AddComponent(new CMeshRender);
	pUIButtonStart->AddComponent(new CButtonScript);

	Vec3 vUIButtonStartScale = Vec3(res.fWidth/13, res.fHeight/14, 1.f);
	pUIButtonStart->Transform()->SetLocalPos(Vec3(res.fWidth / 2 - vUIButtonStartScale.x/2, -res.fHeight / 2 + vUIButtonStartScale.y/2, 1.f));
	pUIButtonStart->Transform()->SetLocalScale(vUIButtonStartScale);

	pUIButtonStart->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pUIButtonStartMtrl = new CMaterial;
	pUIButtonStartMtrl->DisableFileSave();
	pUIButtonStartMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	pUIButtonStart->MeshRender()->SetMaterial(pUIButtonStartMtrl);
	Ptr<CTexture> pUIButtonGameStart = CResMgr::GetInst()->FindRes<CTexture>(L"UIButtonGameStart");
	pUIButtonStart->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pUIButtonGameStart.GetPointer());

	pUIButtonStart->GetScript<CButtonScript>()->SetButtonType(BUTTON_TYPE::GAMESTART);

	FindLayer(L"UI")->AddGameObject(pUIButtonStart);





	// Ready Button
	CGameObject* pUIButtonReady = new CGameObject;
	pUIButtonReady->SetName(L"ReadyButton");
	pUIButtonReady->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
	pUIButtonReady->AddComponent(new CTransform);
	pUIButtonReady->AddComponent(new CMeshRender);
	pUIButtonReady->AddComponent(new CButtonScript);

	pUIButtonReady->Transform()->SetLocalPos(Vec3(res.fWidth / 2 - vUIButtonStartScale.x / 2, -res.fHeight / 2 + vUIButtonStartScale.y / 2 + vUIButtonStartScale.y, 1.f));
	pUIButtonReady->Transform()->SetLocalScale(vUIButtonStartScale);

	pUIButtonReady->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pUIButtonReadyMtrl = new CMaterial;
	pUIButtonReadyMtrl->DisableFileSave();
	pUIButtonReadyMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	pUIButtonReady->MeshRender()->SetMaterial(pUIButtonReadyMtrl);
	Ptr<CTexture> pUIButtonReadyTex = CResMgr::GetInst()->FindRes<CTexture>(L"UIReadyBar");
	pUIButtonReady->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pUIButtonReadyTex.GetPointer());
	
	pUIButtonReady->GetScript<CButtonScript>()->SetButtonType(BUTTON_TYPE::READY);

	FindLayer(L"UI")->AddGameObject(pUIButtonReady);




	// Property Button
	Vec3 vUIProPertyButtonScale = Vec3(res.fWidth / 20, res.fWidth / 20, 1.f);
	Ptr<CTexture> pWaterTex = CResMgr::GetInst()->FindRes<CTexture>(L"PropertyWater");
	Ptr<CTexture> pDarkTex = CResMgr::GetInst()->FindRes<CTexture>(L"PropertyDark");
	Ptr<CTexture> pWindTex = CResMgr::GetInst()->FindRes<CTexture>(L"PropertyWind");
	Ptr<CTexture> pThunderTex = CResMgr::GetInst()->FindRes<CTexture>(L"PropertyThunder");
	Ptr<CTexture> pFireTex = CResMgr::GetInst()->FindRes<CTexture>(L"PropertyFire");
	CGameObject* pUIButtonProperty;
	Ptr<CTexture> pLockTex = CResMgr::GetInst()->FindRes<CTexture>(L"UILOCK");
	CGameObject* pUIButtonLock;
	Vec4 vLockColor = Vec4(1.f, 1.f, 1.f, 0.9f);
	Ptr<CMaterial> pUIButtonLockMtrl = new CMaterial;
	pUIButtonLockMtrl->DisableFileSave();
	pUIButtonLockMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"ColorTexShader"));

	for (int i = 0; i < 5; i++) {
		// 잠금버튼
		pUIButtonLock = new CGameObject;
		pUIButtonLock->SetName(L"LockTex");
		pUIButtonLock->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pUIButtonLock->AddComponent(new CTransform);
		pUIButtonLock->AddComponent(new CMeshRender);
		pUIButtonLock->AddComponent(new CButtonScript);
		pUIButtonLock->MeshRender()->SetRender(false);

		pUIButtonLock->Transform()->SetLocalPos(Vec3(res.fWidth / 2 - vUIProPertyButtonScale.x / 2 - vUIProPertyButtonScale.x * i, res.fHeight / 2 - vUIProPertyButtonScale.y / 2, 1.f));
		pUIButtonLock->Transform()->SetLocalScale(vUIProPertyButtonScale);

		pUIButtonLock->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pUIButtonLock->MeshRender()->SetMaterial(pUIButtonLockMtrl);
		pUIButtonLock->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pLockTex.GetPointer());
		pUIButtonLock->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC4_0, &vLockColor);
		FindLayer(L"UI")->AddGameObject(pUIButtonLock);


		pUIButtonProperty = new CGameObject;
		pUIButtonProperty->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pUIButtonProperty->AddComponent(new CTransform);
		pUIButtonProperty->AddComponent(new CMeshRender);
		pUIButtonProperty->AddComponent(new CButtonScript);

		pUIButtonProperty->Transform()->SetLocalPos(Vec3(0.f, 0.f, 1.f));
		pUIButtonProperty->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

		pUIButtonProperty->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pUIButtonStartMtrl = new CMaterial;
		pUIButtonStartMtrl->DisableFileSave();
		pUIButtonStartMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
		pUIButtonProperty->MeshRender()->SetMaterial(pUIButtonStartMtrl);

		switch (i) {
		case 0:
			pUIButtonProperty->SetName(L"WaterPropertyButton");
			pUIButtonProperty->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pWaterTex.GetPointer());
			pUIButtonLock->GetScript<CButtonScript>()->SetButtonType(BUTTON_TYPE::SELECT_WATER);
			break;
		case 1:
			pUIButtonProperty->SetName(L"FirePropertyButton");
			pUIButtonProperty->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pFireTex.GetPointer());
			pUIButtonLock->GetScript<CButtonScript>()->SetButtonType(BUTTON_TYPE::SELECT_FIRE);
			break;
		case 2:
			pUIButtonProperty->SetName(L"DarkPropertyButton");
			pUIButtonProperty->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pDarkTex.GetPointer());
			pUIButtonLock->GetScript<CButtonScript>()->SetButtonType(BUTTON_TYPE::SELECT_DARK);
			break;
		case 3:
			pUIButtonProperty->SetName(L"ThunderPropertyButton");
			pUIButtonProperty->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pThunderTex.GetPointer());
			pUIButtonLock->GetScript<CButtonScript>()->SetButtonType(BUTTON_TYPE::SELECT_THUNDER);
			break;
		case 4:
			pUIButtonProperty->SetName(L"WindPropertyButton");
			pUIButtonProperty->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pWindTex.GetPointer());
			pUIButtonLock->GetScript<CButtonScript>()->SetButtonType(BUTTON_TYPE::SELECT_WIND);
			break;
		}

		pUIButtonLock->AddChild(pUIButtonProperty);
		//FindLayer(L"UI")->AddGameObject(pUIButtonProperty);
	}

//-----------------------------------------------------------------------------------------------
// SKYBOX
//-----------------------------------------------------------------------------------------------

	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");
	pSkyBox->FrustumCheck(false);
	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CMeshRender);
	pSkyBox->FrustumCheck(false);
	pSkyBox->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pSkyBox->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->FindRes<CTexture>(L"Sky01");
	pSkyBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky01.GetPointer());

	FindLayer(L"Default")->AddGameObject(pSkyBox);

}

void CLobbyScene::Exit()
{

}