#include "pch.h"

#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "Transform.h"
#include "InGameScene.h"
//#include "Collider3D.h"
#include "CollisionMgr.h"
//#include "EventMgr.h"
#include "SensorMgr.h"
//
//#include "GridScript.h"
//#include "PlayerScript.h"
//#include "MonsterScript.h"
//#include "ArrowScript.h"
//#include "MinionScript.h"
//#include "Animation3D.h"
//#include "SpawnScript.h"
//#include "Sensor.h"
//#include "TowerScript.h"


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
		//m_pCurScene = new CLobbyScene;
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


	//m_arrScene[(UINT)SCENE_TYPE::LOBBY] = new CLobbyScene;
	m_arrScene[(UINT)SCENE_TYPE::INGAME] = new CInGameScene;
	m_arrScene[(UINT)SCENE_TYPE::INGAME]->SetName(L"PlayScene");
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::INGAME];
	m_pCurScene->Init();

}

void CSceneMgr::Update()
{
	m_pCurScene->Update();

	m_pCurScene->LateUpdate();

	m_pCurScene->FinalUpdate();
	CSensorMgr::GetInst()->Update();
	// 충돌 처리
	//CCollisionMgr::GetInst()->Update();

}

void CSceneMgr::EnterGame()
{
	ChangeScene(SCENE_TYPE::INGAME);
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

//void CSceneMgr::net_setAnimationByID(int id, int state)
//{
//	m_pCurScene->FindLayer(L"Blue")->GetGameObjectById(id)->GetScript<CPlayerScript>()->SetState(state);
//}



