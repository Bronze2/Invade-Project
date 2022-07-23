#include "pch.h"
#include "ResultScene.h"
#include "Layer.h"
#include "GameObject.h"
#include "RenderMgr.h"
#include "Transform.h"
#include "MeshRender.h"
#include "FontMgr.h"
#include "Light3D.h"
#include "Camera.h"
#include "CameraScript.h"
#include "PlayerScript.h"
#include "Animator3D.h"

void CResultScene::Init()
{
	ShowCursor(false);

	GetLayer(0)->SetName(L"Default");
	GetLayer(3)->SetName(L"Blue");
	GetLayer(4)->SetName(L"Red");
	GetLayer(8)->SetName(L"Tile");
	GetLayer(30)->SetName(L"UI");

	tResolution res = CRenderMgr::GetInst()->GetResolution();

//-----------------------------------------------------------------------------------------------
// ī�޶�
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
// ����
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
// ��
//-----------------------------------------------------------------------------------------------

	// Ÿ��
	CGameObject* pTileObj = new CGameObject;
	pTileObj->SetName(L"Tile");
	pTileObj->AddComponent(new CTransform);
	pTileObj->AddComponent(new CMeshRender);

	pTileObj->Transform()->SetLocalPos(Vec3(0.f, -1.f, 2000.f));
	pTileObj->Transform()->SetLocalScale(Vec3(5000.f, 5000.f, 1.f));
	pTileObj->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	pTileObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	Ptr<CMaterial> pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3DShader"));
	CResMgr::GetInst()->AddRes(L"TileMtrl", pMtrl);
	pTileObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMtrl"));
	Ptr<CTexture> pColor = CResMgr::GetInst()->FindRes<CTexture>(L"Tile");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->FindRes<CTexture>(L"Tile_n");
	pTileObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pTileObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());
	pTileObj->MeshRender()->SetDynamicShadow(false);

	FindLayer(L"Tile")->AddGameObject(pTileObj);

	// ��
	CGameObject* pWallObj = new CGameObject;
	pWallObj->SetName(L"Tile");
	pWallObj->AddComponent(new CTransform);
	pWallObj->AddComponent(new CMeshRender);

	pWallObj->Transform()->SetLocalPos(Vec3(0.f, -1.f, 2000.f));
	pWallObj->Transform()->SetLocalScale(Vec3(5000.f, 500.f, 1.f));
	pWallObj->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pWallObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pWallObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMtrl"));
	pWallObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pWallObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());
	pWallObj->MeshRender()->SetDynamicShadow(false);

	FindLayer(L"Tile")->AddGameObject(pWallObj);

//-----------------------------------------------------------------------------------------------
// �÷��̾� fbx
//-----------------------------------------------------------------------------------------------

	// Player (helmet - PlayerScript����)
	Ptr<CMeshData> pPlayerBlueMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player_blue.mdat", L"MeshData\\player_blue.mdat");
	Ptr<CMeshData> pPlayerRedMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player_red.mdat", L"MeshData\\player_red.mdat");
	Ptr<CTexture> pBlueTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerBlue");
	Ptr<CTexture> pRedTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerRed");

	for (int i = 0; i < 8; i++) {
		CGameObject* pPlayerObj = new CGameObject;

		if (i < 4) {
			pPlayerObj = pPlayerBlueMeshData->Instantiate();
			pPlayerObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlueTex.GetPointer());
			pPlayerObj->Transform()->SetLocalPos(Vec3(-300.f + i * 200.f, 0.f, 700.f));
		}
		else {
			pPlayerObj = pPlayerRedMeshData->Instantiate();
			pPlayerObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pRedTex.GetPointer());
			pPlayerObj->Transform()->SetLocalPos(Vec3(-1000.f + i * 200.f, 0.f, 1300.f));
		}

		pPlayerObj->SetName(L"Monster");
		pPlayerObj->AddComponent(new CPlayerScript);
		pPlayerObj->FrustumCheck(false);
		pPlayerObj->Transform()->SetLocalScale(Vec3(0.4f, 0.4f, 0.5f));
		pPlayerObj->Transform()->SetLocalRot(Vec3(XMConvertToRadians(-90.f), XMConvertToRadians(0.f), 0.f));
		pPlayerObj->MeshRender()->SetDynamicShadow(false);

		CAnimation* pPlayerAnimation = new CAnimation;
		pPlayerAnimation->InsertAnimClip(L"IDLE", 0, 37);
		pPlayerAnimation->InsertAnimClip(L"WALK", 44, 73);
		pPlayerAnimation->InsertAnimClip(L"JUMP", 290, 298);		// 81, 100
		pPlayerAnimation->InsertAnimClip(L"DIE", 242, 261);

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

		if (i < 4) {
			FindLayer(L"Blue")->AddGameObject(pPlayerObj);
			pPlayerObj->GetScript<CPlayerScript>()->SetResultAnim(true);
		}
		else {
			FindLayer(L"Red")->AddGameObject(pPlayerObj);
			pPlayerObj->GetScript<CPlayerScript>()->SetResultAnim(false);
		}

		CGameObject* pReadyBarObj = new CGameObject;
		pReadyBarObj->SetName(L"UIReadyBar");
		pReadyBarObj->FrustumCheck(false);	// ����ü �ø� ������� ����
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

//-----------------------------------------------------------------------------------------------
// UI
//-----------------------------------------------------------------------------------------------
	
	CGameObject* pResult = new CGameObject;
	pResult->SetName(L"UIDark");
	pResult->FrustumCheck(false);
	pResult->AddComponent(new CTransform);
	pResult->AddComponent(new CMeshRender);
	Vec3 vResultTexScale = Vec3(res.fWidth / 2, res.fHeight / 5, 1.f);
	pResult->Transform()->SetLocalPos(Vec3(0.f, res.fHeight/2 - res.fHeight/6 , 1.f));
	pResult->Transform()->SetLocalScale(vResultTexScale);
	pResult->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pResultMtrl = new CMaterial;
	pResultMtrl->DisableFileSave();
	pResultMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	pResult->MeshRender()->SetMaterial(pResultMtrl);
	// �̰����� (������ UILOSE)
	Ptr<CTexture> pResultTex = CResMgr::GetInst()->FindRes<CTexture>(L"UIWIN");
	pResult->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pResultTex.GetPointer());
	FindLayer(L"UI")->AddGameObject(pResult);

}