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
#include "TowerScript.h"

#include "Network.h"

void CLobbyScene::Init()
{
	ShowCursor(true);


	GetLayer(0)->SetName(L"Default");

	CGameObject* pMainCam = nullptr;

	// Camera Object
	pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHGRAPHIC);

	//pMainCam->Transform()->SetLocalRot(Vec3(0.f, 3.14f, 0.f));
	pMainCam->Camera()->SetFar(100.f);
	pMainCam->Camera()->SetLayerCheck(0,true);
	pMainCam->Camera()->SetWidth(CRenderMgr::GetInst()->GetResolution().fWidth);
	pMainCam->Camera()->SetHeight(CRenderMgr::GetInst()->GetResolution().fHeight);

	FindLayer(L"Default")->AddGameObject(pMainCam);





	CRenderMgr::GetInst()->SetCamera(pMainCam->Camera());


	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"UI Object");
	pObject->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CTowerScript);
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 500.f));
	// Transform 설정
	tResolution res = CRenderMgr::GetInst()->GetResolution();

	Ptr<CTexture> pLobbyTex = CResMgr::GetInst()->FindRes<CTexture>(L"LobbyTex");
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
	pObject->Transform()->SetLocalScale(Vec3(100.f, 100.f, 1.f));
	pObject->MeshRender()->SetMaterial(pMtrl->Clone());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pLobbyTex.GetPointer());

	// AddGameObject
	FindLayer(L"Default")->AddGameObject(pObject);
	cout << "!" << endl;

	
	Network::GetInst()->send_login_packet();
	Awake();
	Start();
}

void CLobbyScene::Update()
{
	CScene::Update();
}
