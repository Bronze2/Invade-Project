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
#include "Network.h"

void CResultScene::Init()
{
    //ShowCursor(false);

    GetLayer(0)->SetName(L"Default");
    GetLayer(3)->SetName(L"Blue");
    GetLayer(4)->SetName(L"Red");
    GetLayer(8)->SetName(L"Tile");
    GetLayer(30)->SetName(L"UI");

    tResolution res = CRenderMgr::GetInst()->GetResolution();

    //-----------------------------------------------------------------------------------------------
    // ī ޶ 
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

    pMainCam->Transform()->SetLocalPos(Vec3(0, 100, 0));      // -300, 130, -50
    pMainCam->Transform()->SetLocalRot(Vec3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));      // -15.f

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


    CResMgr::GetInst()->FindRes<CSound>(L"InGameBGM")->Stop();
    CResMgr::GetInst()->FindRes<CSound>(L"ResultBGM")->Play(0, 0.3f, 0);


    //-----------------------------------------------------------------------------------------------
    //     
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
    //   
    //-----------------------------------------------------------------------------------------------

    Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\lobby3.mdat", L"MeshData\\lobby3.mdat");

    CGameObject* pLobbyObj = pMeshData->Instantiate();

    pLobbyObj->FrustumCheck(false);
    pLobbyObj->Transform()->SetLocalPos(Vec3(-72.f, -100.f, 700.f));      //0.f, 370.f, 0.f
    pLobbyObj->Transform()->SetLocalRot(Vec3(-PI / 2, -PI / 2, 0.f));
    pLobbyObj->Transform()->SetLocalScale(Vec3(0.2f, 0.2f, 0.2f));
    pLobbyObj->MeshRender()->SetDynamicShadow(false);

    FindLayer(L"Default")->AddGameObject(pLobbyObj);

    //-----------------------------------------------------------------------------------------------
    //  ÷  ̾  fbx
    //-----------------------------------------------------------------------------------------------

       // Player (helmet - PlayerScript    )
    Ptr<CMeshData> pPlayerBlueMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player_blue.mdat", L"MeshData\\player_blue.mdat");
    Ptr<CMeshData> pPlayerRedMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player_red.mdat", L"MeshData\\player_red.mdat");
    Ptr<CTexture> pBlueTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerBlue");
    Ptr<CTexture> pRedTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerRed");

    for (int i = 0; i < CSceneMgr::GetInst()->GetResult().size(); i++) {
        CGameObject* pPlayerObj = new CGameObject;

        if ((CAMP_STATE)CSceneMgr::GetInst()->GetResult()[i].camp == CAMP_STATE::BLUE) {
            pPlayerObj = pPlayerBlueMeshData->Instantiate();
            pPlayerObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pBlueTex.GetPointer());
            pPlayerObj->Transform()->SetLocalPos(Vec3(-400.f + i * 100.f, -70.f, 650.f));   // -525.f + i * 150.f, 0.f, 1000.f

        }
        else {
            pPlayerObj = pPlayerRedMeshData->Instantiate();
            pPlayerObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pRedTex.GetPointer());
            pPlayerObj->Transform()->SetLocalPos(Vec3(100.f + (i - 4) * 100.f, -70.f, 650.f));   // -525.f + i * 150.f, 0.f, 1000.f
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
        pPlayerAnimation->InsertAnimClip(L"JUMP", 81, 104);      // 81, 100
        pPlayerAnimation->InsertAnimClip(L"DIE", 242, 261);

        pPlayerObj->Animator3D()->SetAnimation(pPlayerAnimation);
        pPlayerObj->Animator3D()->SetAnimClip(pPlayerAnimation->GetAnimClip());

  
        pPlayerObj->GetScript<CPlayerScript>()->SetType(CSceneMgr::GetInst()->GetResult()[i].element);

        if ((CAMP_STATE)CSceneMgr::GetInst()->GetResult()[i].camp == CAMP_STATE::BLUE) {
            FindLayer(L"Blue")->AddGameObject(pPlayerObj);
            pPlayerObj->GetScript<CPlayerScript>()->SetResultAnim(true);      //  ¸   
        }
        else {
            FindLayer(L"Red")->AddGameObject(pPlayerObj);
            pPlayerObj->GetScript<CPlayerScript>()->SetResultAnim(false);      //  й   
            pPlayerObj->Transform()->SetLocalPos(Vec3(100.f + (i - 4) * 100.f, -70.f, 580.f));   //  й     ִϸ  ̼Ƕ                       
        }

        //CGameObject* pReadyBarObj = new CGameObject;
        //pReadyBarObj->SetName(L"UIReadyBar");
        //pReadyBarObj->FrustumCheck(false);   //     ü  ø              
        //pReadyBarObj->AddComponent(new CTransform);
        //pReadyBarObj->AddComponent(new CMeshRender);
        //pReadyBarObj->SetActive(false);

        //Vec3 vUIReadyBarScale = Vec3(100.f, 40.f, 1.f);
        //pReadyBarObj->Transform()->SetLocalPos(Vec3(0.f, 0.f, 300.f));
        //pReadyBarObj->Transform()->SetLocalRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
        //pReadyBarObj->Transform()->SetLocalScale(vUIReadyBarScale);
        //pReadyBarObj->Transform()->SetBillBoard(false);
        //pReadyBarObj->Transform()->SetCamera(pMainCam);

        //pReadyBarObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
        //Ptr<CMaterial> pUIReadyBarMtrl = new CMaterial;
        //pUIReadyBarMtrl->DisableFileSave();
        //pUIReadyBarMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
        //pReadyBarObj->MeshRender()->SetMaterial(pUIReadyBarMtrl);
        //Ptr<CTexture> pUIReadyBarTex = CResMgr::GetInst()->FindRes<CTexture>(L"UIReadyBar");
        //pReadyBarObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pUIReadyBarTex.GetPointer());

        //pPlayerObj->AddChild(pReadyBarObj);
        //FindLayer(L"Blue")->AddGameObject(pReadyBarObj);
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
    pResult->SetName(L"UIResult");
    pResult->FrustumCheck(false);
    pResult->AddComponent(new CTransform);
    pResult->AddComponent(new CMeshRender);
    Vec3 vResultTexScale = Vec3(res.fWidth / 2, res.fHeight / 5, 1.f);
    pResult->Transform()->SetLocalPos(Vec3(0.f, res.fHeight / 2 - res.fHeight / 6, 1.f));
    pResult->Transform()->SetLocalScale(vResultTexScale);
    pResult->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
    Ptr<CMaterial> pResultMtrl = new CMaterial;
    pResultMtrl->DisableFileSave();
    pResultMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
    pResult->MeshRender()->SetMaterial(pResultMtrl);
    Ptr<CTexture> pResultTex;


    if (CSceneMgr::GetInst()->getIsWin()) {
       pResultTex = CResMgr::GetInst()->FindRes<CTexture>(L"UIWIN");
    }
    else {
       pResultTex = CResMgr::GetInst()->FindRes<CTexture>(L"UILOSE");
    }

    pResult->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pResultTex.GetPointer());
    FindLayer(L"UI")->AddGameObject(pResult);
    CFontMgr::GetInst()->Clear();
}