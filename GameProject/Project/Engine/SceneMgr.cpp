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
#include "SkillMgr.h"

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
#include "BowScript.h"
#include "EmptyPlayerScript.h"
#include "LoginScene.h"
#include "TitleScene.h"
#include "Font.h"
#include "FontMgr.h"
#include "ResultScene.h"

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
	// 로비씬
	if (m_pCurScene == m_arrScene[(UINT)_Type])
		return;
	m_pCurScene->Exit();
	SAFE_DELETE(m_pCurScene);
	switch (_Type)
	{
	case SCENE_TYPE::TITLE:
		m_pCurScene = new CTitleScene;
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
		m_pCurScene = new CResultScene;				// 결과씬
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
	//for (int i = 0; i < (UINT)SCENE_TYPE::END; ++i) {
	//	SAFE_DELETE(m_arrScene[i]);
	//}
	delete m_pCurScene;
	m_pCurScene = nullptr;
}

void CSceneMgr::Init()
{
	// 로비씬
	CResMgr::GetInst()->Load<CTexture>(L"bow_big", L"Texture\\bow_big.png");
	CResMgr::GetInst()->Load<CTexture>(L"BaseLine", L"Texture\\Crosshair\\UnknownsCircleFull2048.png");
	CResMgr::GetInst()->Load<CTexture>(L"DarkUITex", L"Texture\\splash.png");
	CResMgr::GetInst()->Load<CTexture>(L"TestAnim2D", L"Texture\\font02_0.png");
	CResMgr::GetInst()->Load<CTexture>(L"UIButtonGameStart", L"Texture\\UI\\START.png");
	CResMgr::GetInst()->Load<CTexture>(L"UIReadyBar", L"Texture\\UI\\READY.png");
	CResMgr::GetInst()->Load<CTexture>(L"UIReadyPressed", L"Texture\\UI\\READY_PRESSED.png");

	// 결과씬
	CResMgr::GetInst()->Load<CTexture>(L"UIWIN", L"Texture\\UI\\WIN.png");
	CResMgr::GetInst()->Load<CTexture>(L"UILOSE", L"Texture\\UI\\LOSE.png");
	CResMgr::GetInst()->Load<CTexture>(L"UILOCK", L"Texture\\UI\\lock.png");

	// 에이치피바
	CResMgr::GetInst()->Load<CTexture>(L"helmet_01_Blue", L"Texture\\helmet\\helmet_01_blue.png");
	CResMgr::GetInst()->Load<CTexture>(L"helmet_02_Blue", L"Texture\\helmet\\helmet_02_blue.png");
	CResMgr::GetInst()->Load<CTexture>(L"helmet_03_Blue", L"Texture\\helmet\\helmet_03_blue.png");
	CResMgr::GetInst()->Load<CTexture>(L"helmet_04_Blue", L"Texture\\helmet\\helmet_04_blue.png");
	CResMgr::GetInst()->Load<CTexture>(L"helmet_05_Blue", L"Texture\\helmet\\helmet_05_blue.png");
	CResMgr::GetInst()->Load<CTexture>(L"helmet_01_Red", L"Texture\\helmet\\helmet_01_red.png");
	CResMgr::GetInst()->Load<CTexture>(L"helmet_02_Red", L"Texture\\helmet\\helmet_02_red.png");
	CResMgr::GetInst()->Load<CTexture>(L"helmet_03_Red", L"Texture\\helmet\\helmet_03_red.png");
	CResMgr::GetInst()->Load<CTexture>(L"helmet_04_Red", L"Texture\\helmet\\helmet_04_red.png");
	CResMgr::GetInst()->Load<CTexture>(L"helmet_05_Red", L"Texture\\helmet\\helmet_05_red.png");

	CResMgr::GetInst()->Load<CTexture>(L"PlayerBlue", L"Texture\\ArchersTextureBlue.png");
	CResMgr::GetInst()->Load<CTexture>(L"PlayerRed", L"Texture\\ArchersTextureRed.png");
	CResMgr::GetInst()->Load<CTexture>(L"PropertyWater", L"Texture\\property_water.png");
	CResMgr::GetInst()->Load<CTexture>(L"PropertyDark", L"Texture\\property_dark.png");
	CResMgr::GetInst()->Load<CTexture>(L"PropertyWind", L"Texture\\property_wind.png");
	CResMgr::GetInst()->Load<CTexture>(L"PropertyThunder", L"Texture\\property_thunder.png");
	CResMgr::GetInst()->Load<CTexture>(L"PropertyFire", L"Texture\\property_fire.png");

	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"TestTex", L"Texture\\Health.png");
	Ptr<CTexture> pExplosionTex = CResMgr::GetInst()->Load<CTexture>(L"Explosion", L"Texture\\Explosion\\Explosion80.png");
	Ptr<CTexture> pBlackTex = CResMgr::GetInst()->Load<CTexture>(L"Black", L"Texture\\asd.png");
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky03.png");
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

	CResMgr::GetInst()->Load<CTexture>(L"Flame", L"Texture\\Particle\\Flame.png");
	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Tile\\TILE_03.tga");
	Ptr<CTexture> pWhite = CResMgr::GetInst()->Load<CTexture>(L"white", L"Texture\\Tile\\1234.png");

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

	Ptr<CTexture> pTreeTex1 = CResMgr::GetInst()->Load<CTexture>(L"TreeTex1", L"Texture\\Tree01.png");
	Ptr<CTexture> pTreeTex2 = CResMgr::GetInst()->Load<CTexture>(L"TreeTex2", L"Texture\\Tree02.png");
	Ptr<CTexture> pLobbyTex = CResMgr::GetInst()->Load<CTexture>(L"LobbyTex", L"Texture\\Lobby.png");
	Ptr<CTexture> pTransparency = CResMgr::GetInst()->Load<CTexture>(L"Transparency", L"Texture\\Transparency.png");
	Ptr<CMaterial> pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
//	Ptr<CMaterial> pMtrl = new CMaterial;
	pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	CResMgr::GetInst()->AddRes(L"TransparencyMtrl", pMtrl);


	SKILL* Skill= new SKILL;
	Skill->Name = L"Water0";
	Skill->Code =0;
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
	Skill->DotDamage =0.f;
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
	CSkillMgr::GetInst()->AddSkill(Skill->Code, Skill); 
	
	Skill = new SKILL;
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

	// 로비씬
	//m_arrScene[(UINT)SCENE_TYPE::LOBBY] = new CLobbyScene;
	//m_arrScene[(UINT)SCENE_TYPE::INGAME] = new CInGameScene;
	//m_arrScene[(UINT)SCENE_TYPE::INGAME] = new CInGameScene;
	
	m_pCurScene = new CLobbyScene;
	m_pCurScene->SetType(SCENE_TYPE::LOBBY);

	//m_pCurScene = new CInGameScene;
	//m_pCurScene->SetType(SCENE_TYPE::INGAME);

	//m_pCurScene = new CResultScene;
	//m_pCurScene->SetType(SCENE_TYPE::RESULT);
	
	m_pCurScene->Init();
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

	CSensorMgr::GetInst()->Update();
	// �浹 ó��
	CCollisionMgr::GetInst()->Update();

	if (KEY_TAB(KEY_TYPE::KEY_NUM7)) {
		if (m_pCurScene->GetCurScene() != SCENE_TYPE::LOBBY) {
			tEvent evn = {};
			evn.wParam = (DWORD_PTR)SCENE_TYPE::LOBBY;
			evn.eType = EVENT_TYPE::CHANGE_SCENE;
			CEventMgr::GetInst()->AddEvent(evn);
		}
	}
	if (KEY_TAB(KEY_TYPE::KEY_NUM8)) {
		if (m_pCurScene->GetCurScene() != SCENE_TYPE::INGAME) {
			tEvent evn = {};
			evn.wParam = (DWORD_PTR)SCENE_TYPE::INGAME;
			evn.eType = EVENT_TYPE::CHANGE_SCENE;
			CEventMgr::GetInst()->AddEvent(evn);
		}
	}
	if (KEY_TAB(KEY_TYPE::KEY_NUM9)) {
		if (m_pCurScene->GetCurScene() != SCENE_TYPE::RESULT) {
			tEvent evn = {};
			evn.wParam = (DWORD_PTR)SCENE_TYPE::RESULT;
			evn.eType = EVENT_TYPE::CHANGE_SCENE;
			CEventMgr::GetInst()->AddEvent(evn);
		}
	}
}

void CSceneMgr::Update_Tool()
{
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

