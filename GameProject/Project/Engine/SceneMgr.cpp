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
#include "LoginScene.h"
#include "TitleScene.h"
#include "ProjectileScript.h"
#include "BowScript.h"
#include "LobbyScene.h"
#include "TitleScene.h"
#include "SkillMgr.h"
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
	case SCENE_TYPE::LOGIN:
		m_pCurScene = new CLoginScene;
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
	m_pCurScene->SetType(_Type);
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
	Ptr<CTexture> pFloorRed = CResMgr::GetInst()->Load<CTexture>(L"FloorRed", L"Texture\\FloorTextures\\ColorPaletteBLUE.png");
	Ptr<CTexture> pFloorBlue = CResMgr::GetInst()->Load<CTexture>(L"FloorBlue", L"Texture\\FloorTextures\\ColorPaletteRED.png");
	Ptr<CTexture> pFloorWater = CResMgr::GetInst()->Load<CTexture>(L"FloorWater", L"Texture\\FloorTextures\\Water.png");

	Ptr<CTexture> pPlayerRed = CResMgr::GetInst()->Load<CTexture>(L"Player_Red", L"Texture\\ArchersTextureRed.png");
	Ptr<CTexture> pPlayerBlue = CResMgr::GetInst()->Load<CTexture>(L"Player_Blue", L"Texture\\ArchersTextureBlue.png");
	Ptr<CTexture> pBowBig = CResMgr::GetInst()->Load<CTexture>(L"bow_big", L"Texture\\bow_big.png");
	Ptr<CTexture> pBaseLine = CResMgr::GetInst()->Load<CTexture>(L"BaseLine", L"Texture\\base_line.png");
	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"TestTex", L"Texture\\Health.png");
	Ptr<CTexture> pExplosionTex = CResMgr::GetInst()->Load<CTexture>(L"Explosion", L"Texture\\Explosion\\Explosion80.png");
	Ptr<CTexture> pBlackTex = CResMgr::GetInst()->Load<CTexture>(L"Black", L"Texture\\asd.png");
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky01.png");
	Ptr<CTexture> pSky02 = CResMgr::GetInst()->Load<CTexture>(L"Sky02", L"Texture\\Skybox\\Sky02.jpg");
	CResMgr::GetInst()->Load<CTexture>(L"Snow", L"Texture\\Particle\\Snow50px.png");
	CResMgr::GetInst()->Load<CTexture>(L"Bubble50", L"Texture\\Particle\\Bubbles50px.png");
	CResMgr::GetInst()->Load<CTexture>(L"Bubble99", L"Texture\\Particle\\Bubbles99px.png");
	CResMgr::GetInst()->Load<CTexture>(L"Sparks", L"Texture\\Particle\\Sparks.png");
	CResMgr::GetInst()->Load<CTexture>(L"HardRain", L"Texture\\Particle\\HardRain.png");
	CResMgr::GetInst()->Load<CTexture>(L"Thunder", L"Texture\\Particle\\Thunder.png");
	CResMgr::GetInst()->Load<CTexture>(L"HardRain2", L"Texture\\Particle\\HardRain2.png");
	CResMgr::GetInst()->Load<CTexture>(L"smokeparticle", L"Texture\\Particle\\smokeparticle.png");
	CResMgr::GetInst()->Load<CTexture>(L"HardCircle", L"Texture\\Particle\\HardCircle.png");
	CResMgr::GetInst()->Load<CTexture>(L"particle_00", L"Texture\\Particle\\particle_00.png");
	CResMgr::GetInst()->Load<CTexture>(L"MagicCircle", L"Texture\\MagicCircle.png");
	CResMgr::GetInst()->Load<CTexture>(L"MagicCircle2", L"Texture\\MagicCircle2.png");
	CResMgr::GetInst()->Load<CTexture>(L"Flame", L"Texture\\Particle\\Flame.png");


	//Lobby
	CResMgr::GetInst()->Load<CTexture>(L"UIButtonGameStart", L"Texture\\UIButtonGameStart.png");
	CResMgr::GetInst()->Load<CTexture>(L"UIReadyBar", L"Texture\\UIReadyBar.png");
	CResMgr::GetInst()->Load<CTexture>(L"UIReadyPressed", L"Texture\\UIReadyPressed.png");

	CResMgr::GetInst()->Load<CTexture>(L"PlayerBlue", L"Texture\\ArchersTextureBlue.png");
	CResMgr::GetInst()->Load<CTexture>(L"PlayerRed", L"Texture\\ArchersTextureRed.png");
	CResMgr::GetInst()->Load<CTexture>(L"PropertyWater", L"Texture\\property_water.png");
	CResMgr::GetInst()->Load<CTexture>(L"PropertyDark", L"Texture\\property_dark.png");
	CResMgr::GetInst()->Load<CTexture>(L"PropertyWind", L"Texture\\property_wind.png");
	CResMgr::GetInst()->Load<CTexture>(L"PropertyThunder", L"Texture\\property_thunder.png");
	CResMgr::GetInst()->Load<CTexture>(L"PropertyFire", L"Texture\\property_fire.png");
	//


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


	SKILL* Skill = new SKILL;
	Skill->Name = L"Water0";
	Skill->Code = 0;
	Skill->eSkillType = SKILL_TYPE::ACT;
	Skill->eElementType = ELEMENT_TYPE::WATER;
	Skill->fCoolTime = 10;
	Skill->fDuration = 3;
	Skill->DotDamage = -16.40000000000f;
	Skill->fDamage = 0.f;
	Skill->bUse = false;
	Skill->Sum = 50.f;
	CSkillMgr::GetInst()->AddSkill(0, Skill);

	Skill = new SKILL;
	Skill->Name = L"Water1";
	Skill->Code = 1;
	Skill->eSkillType = SKILL_TYPE::ACT;
	Skill->eElementType = ELEMENT_TYPE::WATER;
	Skill->fCoolTime = 20;
	Skill->fDuration = 5;
	Skill->DotDamage = -20.f;
	Skill->fDamage = 0.f;
	Skill->bUse = false;
	Skill->Sum = 100.f;
	CSkillMgr::GetInst()->AddSkill(Skill->Code, Skill);

	Skill = new SKILL;
	Skill->Name = L"Wind0";
	Skill->Code = 2;
	Skill->eSkillType = SKILL_TYPE::ACT;
	Skill->eElementType = ELEMENT_TYPE::WIND;
	Skill->fCoolTime = 12;
	Skill->fDuration = 5;
	Skill->fDamage = 20.f;
	Skill->DotDamage = 0.f;
	Skill->bUse = false;
	CSkillMgr::GetInst()->AddSkill(Skill->Code, Skill);

	Skill = new SKILL;
	Skill->Name = L"Wind1";
	Skill->Code = 3;
	Skill->eSkillType = SKILL_TYPE::ACT;
	Skill->eElementType = ELEMENT_TYPE::WIND;
	Skill->fCoolTime = 12;
	Skill->fDuration = 0;
	Skill->fDamage = 32.f;
	Skill->DotDamage = 0.f;
	Skill->bUse = false;
	CSkillMgr::GetInst()->AddSkill(Skill->Code, Skill);

	Skill = new SKILL;
	Skill->Name = L"Dark0";
	Skill->Code = 4;
	Skill->eSkillType = SKILL_TYPE::ACT;
	Skill->eElementType = ELEMENT_TYPE::DARK;
	Skill->fCoolTime = 15;
	Skill->fDuration = 3;
	Skill->fDamage = 20.f;
	Skill->DotDamage = 0.f;
	Skill->bUse = false;
	CSkillMgr::GetInst()->AddSkill(Skill->Code, Skill); Skill = new SKILL;
	Skill->Name = L"Dark1";
	Skill->Code = 5;
	Skill->eSkillType = SKILL_TYPE::ACT;
	Skill->eElementType = ELEMENT_TYPE::DARK;
	Skill->fCoolTime = 30;
	Skill->fDuration = 5;
	Skill->fDamage = 0.f;
	Skill->DotDamage = 0.f;
	Skill->bUse = false;
	CSkillMgr::GetInst()->AddSkill(Skill->Code, Skill);
	Skill = new SKILL;
	Skill->Name = L"Thunder0";
	Skill->Code = 6;
	Skill->eSkillType = SKILL_TYPE::ACT;
	Skill->eElementType = ELEMENT_TYPE::THUNDER;
	Skill->fCoolTime = 20;
	Skill->fDuration = 5;
	Skill->fDamage = 15.f;
	Skill->DotDamage = 0.f;
	Skill->bUse = false;
	CSkillMgr::GetInst()->AddSkill(Skill->Code, Skill);
	Skill = new SKILL;
	Skill->Name = L"Thunder1";
	Skill->Code = 7;
	Skill->eSkillType = SKILL_TYPE::ACT;
	Skill->eElementType = ELEMENT_TYPE::THUNDER;
	Skill->fCoolTime = 20;
	Skill->fDuration = 3;
	Skill->fDamage = 0.f;
	Skill->bUse = false;
	Skill->DotDamage = 24.f;
	Skill->Sum = -72.f;
	CSkillMgr::GetInst()->AddSkill(Skill->Code, Skill);

	Skill = new SKILL;
	Skill->Name = L"Fire0";
	Skill->Code = 8;
	Skill->eSkillType = SKILL_TYPE::ACT;
	Skill->eElementType = ELEMENT_TYPE::FIRE;
	Skill->fCoolTime = 8;
	Skill->fDuration = 5;
	Skill->fDamage = 15.f;
	Skill->DotDamage = 5.f;
	Skill->Sum = -25.f;
	Skill->bUse = false;
	CSkillMgr::GetInst()->AddSkill(Skill->Code, Skill); Skill = new SKILL;
	Skill->Name = L"Fire1";
	Skill->Code = 9;
	Skill->eSkillType = SKILL_TYPE::ACT;
	Skill->eElementType = ELEMENT_TYPE::FIRE;
	Skill->fCoolTime = 25;
	Skill->fDuration = 3;
	Skill->fDamage = 20.f;
	Skill->DotDamage = 10.f;
	Skill->Sum = -40.f;
	Skill->bUse = false;
	CSkillMgr::GetInst()->AddSkill(Skill->Code, Skill);






	m_arrScene[(UINT)SCENE_TYPE::LOGIN] = new  CLoginScene;
	m_arrScene[(UINT)SCENE_TYPE::LOGIN]->SetName(L"LoginScene");


//	m_arrScene[(UINT)SCENE_TYPE::LOBBY] = new  CLobbyScene;
//	m_arrScene[(UINT)SCENE_TYPE::LOBBY]->SetName(L"LobbyScene");


	//m_arrScene[(UINT)SCENE_TYPE::INGAME] = new CInGameScene;
	//m_arrScene[(UINT)SCENE_TYPE::INGAME]->SetName(L"PlayScene");


	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::LOGIN];
	m_pCurScene->SetType(SCENE_TYPE::LOGIN);
	m_pCurScene->Init();
	m_pCurScene->Awake();
	m_pCurScene->Start();
	cout << "Done init" << endl;


}

void CSceneMgr::Update()
{



	// rendermgr 카메라 초기화
	//CRenderMgr::GetInst()->RegisterCamera(CRenderMgr::GetInst()->GetCamera());
	CRenderMgr::GetInst()->ClearCamera();
	Network::GetInst()->RecvData();

	m_pCurScene->Update();
	m_pCurScene->LateUpdate();

	m_pCurScene->FinalUpdate();

	CSensorMgr::GetInst()->Update();
	CCollisionMgr::GetInst()->Update();
	// 충돌 처리
	
	CRenderMgr::GetInst()->Render();

	//if (KEY_TAB(KEY_TYPE::KEY_Q) && m_pCurScene == m_arrScene[(UINT)SCENE_TYPE::LOBBY]) {
	//	if (Network::GetInst()->getHost()) {
	//		cout << "현재 방 인원수 :" << Network::GetInst()->getOtherClientSize() + 1  << endl;
	//		if (Network::GetInst()->getOtherClientSize() % 2 == 1) {
	//			Network::GetInst()->send_game_start_packet();			
	//			//ChangeScene(SCENE_TYPE::INGAME);

	//		}
	//	}
	//}
}

#include "ButtonScript.h"
void CSceneMgr::ChangeLobbyTeamState(int blue[4], int red[4])
{
	for (int i = 0; i < 4; ++i) {
		CGameObject* p_tempPlayerBlue = m_pCurScene->FindLayer(L"Player")->GetParentObj()[i];
		p_tempPlayerBlue->GetScript<CPlayerScript>()->m_SetId(blue[i]);
		cout <<"B-"<< blue[i] << endl;
		cout << "R-" << red[i] << endl;

		if (blue[i] != 999) {
			Vec3 Pos = p_tempPlayerBlue->Transform()->GetLocalPos();
			Pos.y = 0;
			p_tempPlayerBlue->Transform()->SetLocalPos(Pos);
			p_tempPlayerBlue->GetScript<CPlayerScript>()->SetCamp(CAMP_STATE::BLUE);
		}
		else {
			Vec3 Pos = p_tempPlayerBlue->Transform()->GetLocalPos();
			Pos.y = 1000;
			p_tempPlayerBlue->Transform()->SetLocalPos(Pos);
		}
		CGameObject* p_tempPlayerRed = m_pCurScene->FindLayer(L"Player")->GetParentObj()[i+4];
		p_tempPlayerRed->GetScript<CPlayerScript>()->m_SetId(red[i]);
		if (red[i] != 999) {
			Vec3 Pos = p_tempPlayerRed->Transform()->GetLocalPos();
			Pos.y = 0;
			p_tempPlayerRed->Transform()->SetLocalPos(Pos);
			p_tempPlayerRed->GetScript<CPlayerScript>()->SetCamp(CAMP_STATE::RED);

		}
		else {
			Vec3 Pos = p_tempPlayerRed->Transform()->GetLocalPos();
			Pos.y = 1000;
			p_tempPlayerRed->Transform()->SetLocalPos(Pos);
		}
	}

	for (auto button : m_pCurScene->FindLayer(L"UI")->GetParentObj())
	{
		button->GetScript<CButtonScript>()->SetPlayerInit(Network::GetInst()->getMainClient().id);
	}

}

void CSceneMgr::LobbySetReady(int id, bool isReady)
{
		for (auto button : m_pCurScene->FindLayer(L"UI")->GetParentObj())
		{
			if (button->GetScript<CButtonScript>()->GetButtonType() == BUTTON_TYPE::READY) {
				button->GetScript<CButtonScript>()->ExecuteReady(id);
			}
		}
}

void CSceneMgr::LobbySetSkill(int id, ELEMENT_TYPE skill)
{
	BUTTON_TYPE tempButton;
	if (skill == ELEMENT_TYPE::DARK)
		tempButton = BUTTON_TYPE::SELECT_DARK;
	else if (skill == ELEMENT_TYPE::FIRE)
		tempButton = BUTTON_TYPE::SELECT_FIRE;
	else if (skill == ELEMENT_TYPE::THUNDER)
		tempButton = BUTTON_TYPE::SELECT_THUNDER;
	else if (skill == ELEMENT_TYPE::WATER)
		tempButton = BUTTON_TYPE::SELECT_WATER;
	else if (skill == ELEMENT_TYPE::WIND)
		tempButton = BUTTON_TYPE::SELECT_WIND;

	for (auto button : m_pCurScene->FindLayer(L"UI")->GetParentObj())
	{
		if (button->GetScript<CButtonScript>()->GetButtonType() == tempButton) {
			button->GetScript<CButtonScript>()->ExecuteChangeSkill(id);
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

//
//void CSceneMgr::net_enterClient(int id ,float x, float y, float z)
//{
//    m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->SetActive(true);
//    m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->Awake();
//    m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->Start();
//
//    m_pCurScene->FindLayer(L"Monster")->GetGameObjectById(id)->Transform()->SetLocalPos(Vec3(x, y, z));
//    std::cout << "생성된 Player 개수 : " << m_pCurScene->FindLayer(L"Blue")->GetParentObj().size() << std::endl; 
//
//}

void CSceneMgr::net_setRotationByID(int id, float x, float y, float z)
{
	Vec3 rot(x, y, z);

	for (auto cl : m_pCurScene->FindLayer(L"Blue")->GetParentObj()) {
		if (cl->GetScript<CPlayerScript>()->m_GetId() == id) {
			cl->GetScript<CPlayerScript>()->SetLerpRot(rot);
			break;
		}
	}
   // m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->GetScript<CPlayerScript>()->SetLerpRot(rot);
}

void CSceneMgr::net_setLerpMoveByID(int id, float x , float y, float z)
{
	Vec3 pos(x, y, z);
	//pos*= DT;
	for (auto cl : m_pCurScene->FindLayer(L"Blue")->GetParentObj()) {
		if (cl->GetScript<CPlayerScript>()->m_GetId() == id) {
			cl->GetScript<CPlayerScript>()->SetLerpPos(pos);
			break;
		}
	}
	//m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->GetScript<CPlayerScript>()->SetLerpPos(pos);
	
	//m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->GetScript<CPlayerScript>()->SetCollCheckFalse();
}

void CSceneMgr::net_setLocalPosByID(int id, float x, float y, float z)
{
	Vec3 pos(x, y, z);
	//pos*= DT;
	for (auto cl : m_pCurScene->FindLayer(L"Blue")->GetParentObj()) {
		if (cl->GetScript<CPlayerScript>()->m_GetId() == id) {
			cl->Transform()->SetLocalPos(pos);
			break;
		}
	}
	//m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->Transform()->SetLocalPos(pos);
}



void CSceneMgr::net_setAnimationByID(int id, int state)
{

	for (auto cl : m_pCurScene->FindLayer(L"Blue")->GetParentObj()) {
		if (cl->GetScript<CPlayerScript>()->m_GetId() == id) {
			cl->GetScript<CPlayerScript>()->SetState(state);
			break;
		}
	}
	//m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->GetScript<CPlayerScript>()->SetState(state);
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
	m_projectile[id] = Pos;
	m_projectile_die[id] = false;
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

void CSceneMgr::net_initArrow(int parentid, int id, Vec3 pos, Vec3 rot, int skill)
{
	m_arrow[parentid][id].Pos = pos;
	m_arrow[parentid][id].Rot = rot;

	for (auto cl : m_pCurScene->FindLayer(L"Blue")->GetParentObj()) {
		if (cl->GetScript<CPlayerScript>()->m_GetId() == parentid) {
			cout << ">>" << endl;
			cl->GetScript<CPlayerScript>()->SetState((int)PLAYER_STATE::ATTACK);
			//cl->GetChild()[0]->GetScript<CBowScript>()->InitArrow(id, pos, rot, (PACKET_SKILL)skill);
			cl->GetChild()[0]->GetScript<CBowScript>()->CreateArrow((PACKET_SKILL)skill);
			break;
		}
	}
	//m_pCurScene->FindLayer(L"Blue")->GetParentObj()[parentid]->GetScript<CPlayerScript>()->SetState((int)PLAYER_STATE::ATTACK);
	//m_pCurScene->FindLayer(L"Blue")->GetParentObj()[parentid]->GetChild()[0]->
	//	GetScript<CBowScript>()->InitArrow(id, pos, rot);
}
void CSceneMgr::net_deleteArrow(int client_id, int arrow_id)
{

	m_arrow[client_id][arrow_id].skill = true;
	for (auto cl : m_pCurScene->FindLayer(L"Blue")->GetParentObj()) {
		if (cl->GetScript<CPlayerScript>()->m_GetId() == client_id) {
			cl->GetChild()[0]->GetScript<CBowScript>()->DeleteArrow(arrow_id);
			break;
		}
	}
	m_arrow[client_id][arrow_id].Pos = Vec3(-1000,-1000,-1000);
	//m_pCurScene->FindLayer(L"Blue")->GetParentObj()[client_id]->GetChild()[0]->
	//	GetScript<CBowScript>()->DeleteArrow(arrow_id);
}

void CSceneMgr::net_DamagedByArrow(int coll_type, int coll_id, int damage)
{

	//0 자기자신 return

	// 1플레이어
	// 2미니언
	// 3 tower;
	if ((PACKET_COLLTYPE)coll_type == PACKET_COLLTYPE::WALL) return;
	else if ((PACKET_COLLTYPE)coll_type == PACKET_COLLTYPE::PLAYER) {
		cout << "PlayerArrow Coll -" <<coll_id << endl;
		for (auto cl : m_pCurScene->FindLayer(L"Blue")->GetParentObj()) {
			if (cl->GetScript<CPlayerScript>()->m_GetId() == coll_id) {
				cl->GetScript<CPlayerScript>()->GetDamage(damage);
				break;
			}
		}
		//m_pCurScene->FindLayer(L"Blue")->GetParentObj()[coll_id]->GetScript<CPlayerScript>()->GetDamage(damage);
	}
	//else if (coll_type == 2) {

	//}
	//else if (coll_type == 3) {

	//}


}
void CSceneMgr::net_deletProjectile(int id)
{
	m_projectile_die[id] = true;
}


void CSceneMgr::net_animUpdate(int id, int state)
{
	for (auto cl : m_pCurScene->FindLayer(L"Blue")->GetParentObj()) {
		if (cl->GetScript<CPlayerScript>()->m_GetId() == id) {
			cl->GetScript<CPlayerScript>()->SetState(state);
			break;
		}
	}
	//m_pCurScene->FindLayer(L"Blue")->GetParentObj()[id]->GetScript<CPlayerScript>()->
}

void CSceneMgr::net_playerHelmetUpdate(int id,Vec3 LocalPos, Vec4 Quaternion, Vec3 LocalRot, Vec3 RevolutionRot)
{
	for (auto cl : m_pCurScene->FindLayer(L"Blue")->GetParentObj()) {
		if (cl->GetScript<CPlayerScript>()->m_GetId() == id) {
			cl->GetScript<CPlayerScript>()->UpdateHelmet(LocalPos, Quaternion, LocalRot, RevolutionRot);
			break;
		}
	}
}

void CSceneMgr::net_updateArrow(int id, int arrow_id , Vec3 LocalPos, Vec4 Quaternion)
{
	for (auto cl : m_pCurScene->FindLayer(L"Blue")->GetParentObj()) {
		if (cl->GetScript<CPlayerScript>()->m_GetId() == id) {
			cl->GetChild()[0]->GetScript<CBowScript>()->UpdateArrow(arrow_id, LocalPos, Quaternion);
			break;
		}
	}
}
