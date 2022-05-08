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
#include "LobbyScene.h"
#include "InGameScene.h"
#include "ProjectileScript.h"
#include "BowScript.h"


#include "Network.h";

CScene* CSceneMgr::GetCurScene()
{
	return m_pCurScene;
}

void CSceneMgr::ChangeScene(CScene* _pNextScene)
{
	if (m_pCurScene == _pNextScene)
		return;
	SAFE_DELETE(m_pCurScene);

	m_pCurScene = _pNextScene;
	m_pCurScene->Awake();
	m_pCurScene->Start();
}

void CSceneMgr::ChangeScene(SCENE_TYPE _Type)
{
	if (m_pCurScene == m_arrScene[(UINT)_Type])
		return;
	SAFE_DELETE(m_pCurScene);
	switch (_Type)
	{
	case SCENE_TYPE::TITLE:
		break;
	case SCENE_TYPE::LOBBY:
		m_pCurScene = new CLobbyScene;
		break;
	case SCENE_TYPE::INGAME:
		m_pCurScene = new CInGameScene;
		break;
	case SCENE_TYPE::RESULT:
		break;
	case SCENE_TYPE::END:
		assert(false);
		break;
	default:
		break;
	}

	m_pCurScene->Init();
	m_pCurScene->Awake();
	m_pCurScene->Start();
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
	Ptr<CTexture> pBowBig = CResMgr::GetInst()->Load<CTexture>(L"bow_big", L"Texture\\bow_big.png");
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
	Ptr<CTexture> pTreeTex1 = CResMgr::GetInst()->Load<CTexture>(L"TreeTex1", L"Texture\\Tree01.png");
	Ptr<CTexture> pTreeTex2 = CResMgr::GetInst()->Load<CTexture>(L"TreeTex2", L"Texture\\Tree02.png");
	Ptr<CTexture> pLobbyTex = CResMgr::GetInst()->Load<CTexture>(L"LobbyTex", L"Texture\\Lobby.png");
	Ptr<CTexture> pTransparency = CResMgr::GetInst()->Load<CTexture>(L"Transparency", L"Texture\\Transparency.png");

	Ptr<CMaterial> pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	CResMgr::GetInst()->AddRes(L"TransparencyMtrl", pMtrl);
	m_arrScene[(UINT)SCENE_TYPE::LOBBY] = new CLobbyScene;
	m_arrScene[(UINT)SCENE_TYPE::INGAME] = new CInGameScene;
	m_arrScene[(UINT)SCENE_TYPE::INGAME]->SetName(L"PlayScene");
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::LOBBY];
	m_pCurScene->Init();



}

void CSceneMgr::Update()
{
	m_pCurScene->Update();
	m_pCurScene->LateUpdate();

	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();
	//CRenderMgr::GetInst()->RegisterCamera(CRenderMgr::GetInst()->GetCamera());

	m_pCurScene->FinalUpdate();


	CSensorMgr::GetInst()->Update();
	// 충돌 처리
	CCollisionMgr::GetInst()->Update();


	Network::GetInst()->RecvData();

	if (KEY_TAB(KEY_TYPE::KEY_Q) && m_pCurScene == m_arrScene[(UINT)SCENE_TYPE::LOBBY]) {
		if (Network::GetInst()->getHost()) {
			cout << "현재 방 인원수 :" << Network::GetInst()->getOtherClientSize() + 1  << endl;
			if (Network::GetInst()->getOtherClientSize() % 2 == 1) {
				Network::GetInst()->send_game_start_packet();			
				//ChangeScene(SCENE_TYPE::INGAME);

			}
		}
	}
}

void CSceneMgr::EnterGame()
{
	ChangeScene(SCENE_TYPE::INGAME);
}


void CSceneMgr::Update_Tool()
{
    // rendermgr 카메라 초기화
    CRenderMgr::GetInst()->ClearCamera();
    m_pCurScene->FinalUpdate();
}

void CSceneMgr::Init(SCENE_TYPE _eType)
{
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


void CSceneMgr::net_enterClient(int id ,float x, float y, float z)
{
    m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->SetActive(true);
    m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->Awake();
    m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->Start();

    m_pCurScene->FindLayer(L"Monster")->GetGameObjectById(id)->Transform()->SetLocalPos(Vec3(x, y, z));
    std::cout << "생성된 Player 개수 : " << m_pCurScene->FindLayer(L"Blue")->GetParentObj().size() << std::endl; 

}

void CSceneMgr::net_setRotationByID(int id, float x, float y, float z)
{
	Vec3 rot(x, y, z);

    m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->GetScript<CPlayerScript>()->SetLerpRot(rot);
}

void CSceneMgr::net_setLerpMoveByID(int id, float x , float y, float z)
{
	Vec3 pos(x, y, z);
	//pos*= DT;
	m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->GetScript<CPlayerScript>()->SetLerpPos(pos);

}

void CSceneMgr::net_setLocalPosByID(int id, float x, float y, float z)
{
	Vec3 pos(x, y, z);
	//pos*= DT;
	m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->Transform()->SetLocalPos(pos);
}

void CSceneMgr::net_setMainClient(int id, float x, float y, float z)
{


    //m_pCurScene->FindLayer(L"Default")->GetGameObjectById(0)->Transform()->SetLocalPos(Vec3(-60, 40, -10));
    ////   pMainCam->Transform()->SetLocalScale(Vec3(15000.f, 15000.f, 15000.f));
    //m_pCurScene->FindLayer(L"Default")->GetGameObjectById(0)->Transform()->SetLocalRot(Vec3(0, PI / 2, -PI / 18));



    //std::cout << "Add Camera" << std::endl;
    //m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->AddChild(m_pCurScene->FindLayer(L"Default")->GetGameObjectById(0));
    //m_pCurScene->FindLayer(L"Default")->GetGameObjectById(0)->Awake();
    //m_pCurScene->FindLayer(L"Default")->GetGameObjectById(0)->Start();


	std::cout << "Set Loacl Pos" << std::endl;
	m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->Transform()->SetLocalPos(Vec3(x, y, z));
	m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->GetScript<CPlayerScript>()->SetMain();
    m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->SetActive(true);
    m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->Awake();
    m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->Start();
	std::cout << "End MainClient" << std::endl;


}
void CSceneMgr::net_setEnableClient(int id)
{
    m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->Enable();

}

void CSceneMgr::net_setAnimationByID(int id, int state)
{
	m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->GetScript<CPlayerScript>()->SetState(state);
}

void CSceneMgr::net_spawnMinion_blue(int id, int mtype,float x, float y ,float z)
{
	Vec3 Pos{ x,y,z };
	m_minion[id].pos = Pos;

	m_pCurScene->FindLayer(L"BlueSpawnPlace")->GetGameObjectById(0)->GetScript<CSpawnScript>()->SpawnObject_Blue(id, Pos, (MINION_ATTACK_TYPE)mtype);
}
void CSceneMgr::net_spawnMinion_red(int id, int mtype,float x, float y, float z)
{
	Vec3 Pos{ x,y,z };
	m_minion[id].pos = Pos;

	m_pCurScene->FindLayer(L"RedSpawnPlace")->GetGameObjectById(0)->GetScript<CSpawnScript>()->SpawnObject_Red(id, Pos, (MINION_ATTACK_TYPE)mtype);
}

void CSceneMgr::net_moveMinion(int id, float x, float y, float z, float r_x, float r_y, float r_z, int state)
{
	m_minion[id].pos.x = x;
	m_minion[id].pos.y = y;
	m_minion[id].pos.z = z;
	m_minion[id].rot.x = r_x;
	m_minion[id].rot.y = r_y;
	m_minion[id].rot.z = r_z;
	if(state == 0)
		m_minion[id].state = MINION_STATE::IDLE;
	if(state == 1)
		m_minion[id].state = MINION_STATE::WALK;
	if (state == 2)
		m_minion[id].state = MINION_STATE::ATTACK;
	if (state == 3)
		m_minion[id].state = MINION_STATE::DIE;

}

void CSceneMgr::net_animMinion(int id, float x, float y, float z, float r_x, float r_y, float r_z, int state)
{
	m_minion[id].pos.x = x;
	m_minion[id].pos.y = y;
	m_minion[id].pos.z = z;
	m_minion[id].rot.x = r_x;
	m_minion[id].rot.y = r_y;
	m_minion[id].rot.z = r_z;
	cout << state << endl;
	if(state == 0)
		m_minion[id].state = MINION_STATE::IDLE;
	if (state == 1)
		m_minion[id].state = MINION_STATE::WALK;
	if (state == 2)
		m_minion[id].state = MINION_STATE::ATTACK;
	if (state == 3)
		m_minion[id].state = MINION_STATE::DIE;
}

void CSceneMgr::net_deleteMinion(int id)
{
	m_minion[id].state = MINION_STATE::DIE;
}

void CSceneMgr::net_setRotTower(int id,	Vec3 Rot)
{
	m_tower[id] = Rot;
}

void CSceneMgr::net_spawnProjectile(int id, Vec3 Pos)
{
	//Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\blueball.mdat", L"MeshData\\blueball.mdat");
	//CGameObject* pObject = pMeshData->Instantiate();
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CCollider3D);
	//pObject->AddComponent(new CProjectileScript);
	//pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	//pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
	//pObject->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//pObject->FrustumCheck(false);
	//pObject->Transform()->SetLocalPos(Pos);
	//pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pObject->Transform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	//pObject->GetScript<CProjectileScript>()->Init();
	//m_pCurScene->FindLayer(L"Red")->AddGameObject(pObject);
	m_projectile[id] = Pos;
	m_pCurScene->FindLayer(L"BlueSpawnPlace")->GetGameObjectById(0)->GetScript<CSpawnScript>()->SpawnObject_Pro(id, Pos);


}

void CSceneMgr::net_moveProjectile(int id, Vec3 Pos)
{
	m_projectile[id] = Pos;
}

void CSceneMgr::net_moveArrow(int parentid, int id, Vec3 pos, Vec3 rot)
{
	m_arrow[parentid][id].Pos = pos;
	m_arrow[parentid][id].Rot = rot;
}

void CSceneMgr::net_initArrow(int parentid, int id, Vec3 pos, Vec3 rot)
{
	m_arrow[parentid][id].Pos = pos;
	m_arrow[parentid][id].Rot = rot;
	m_pCurScene->FindLayer(L"Blue")->GetParentObj()[parentid]->GetScript<CPlayerScript>()->SetState((int)PLAYER_STATE::ATTACK);
	m_pCurScene->FindLayer(L"Blue")->GetParentObj()[parentid]->GetChild()[0]->
		GetScript<CBowScript>()->InitArrow(id, pos, rot);
}
void CSceneMgr::net_deleteArrow(int client_id, int arrow_id)
{
	m_arrow[client_id][arrow_id].Pos = Vec3(1000,1000,1000);
	m_pCurScene->FindLayer(L"Blue")->GetParentObj()[client_id]->GetChild()[0]->
		GetScript<CBowScript>()->DeleteArrow(arrow_id);
}

void CSceneMgr::net_DamagedByArrow(int coll_type, int coll_id, int damage)
{
	//0 자기자신 return

	// 1플레이어
	// 2미니언
	// 3 tower;
	if (coll_type == 0) return;
	else if (coll_type == 1) {
		cout << "PlayerArrow Coll" << endl;
		m_pCurScene->FindLayer(L"Blue")->GetParentObj()[coll_id]->GetScript<CPlayerScript>()->GetDamage(damage);
	}
	else if (coll_type == 2) {

	}
	else if (coll_type == 3) {

	}


}



void CSceneMgr::net_animUpdate(int id, int state)
{
	m_pCurScene->FindLayer(L"Blue")->GetParentObj()[id]->GetScript<CPlayerScript>()->SetState(state);
}
