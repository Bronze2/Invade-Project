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
#include "Service.h"
//#include "GridScript.h"
#include "PlayerScript.h"
//#include "MonsterScript.h"
//#include "ArrowScript.h"
//#include "MinionScript.h"
//#include "Animation3D.h"
//#include "SpawnScript.h"
//#include "Sensor.h"
//#include "TowerScript.h"


CScene* CSceneMgr::GetCurScene(int index)
{
	return m_pCurScene[index];
}

void CSceneMgr::ChangeScene(CScene* _pNextScene, int index)
{
	if (m_pCurScene[index] == _pNextScene)
		return;
	SAFE_DELETE(m_pCurScene[index]);

	m_pCurScene[index] = _pNextScene;
	m_pCurScene[index]->Awake();
	m_pCurScene[index]->Start();
}

void CSceneMgr::ChangeScene(SCENE_TYPE _Type, int index)
{
	if (m_pCurScene[index] == m_arrScene[(UINT)_Type])
		return;
	SAFE_DELETE(m_pCurScene[index]);
	switch (_Type)
	{
	case SCENE_TYPE::TITLE:
		break;
	case SCENE_TYPE::LOBBY:
		//m_pCurScene = new CLobbyScene;
		break;
	case SCENE_TYPE::INGAME:
		m_pCurScene[index] = new CInGameScene;
		break;
	case SCENE_TYPE::RESULT:
		break;
	case SCENE_TYPE::END:
		assert(false);
		break;
	default:
		break;
	}

	m_pCurScene[index]->Init(index);
	m_pCurScene[index]->Awake();
	m_pCurScene[index]->Start();
}

CSceneMgr::CSceneMgr()
{
}

CSceneMgr::~CSceneMgr()
{
	//for(auto scene : m_pCurScene)
	//	SAFE_DELETE(scene.first);
}

void CSceneMgr::Init(int index)
{
	// 필요한 리소스 로딩
	// Texture 로드


	//m_arrScene[(UINT)SCENE_TYPE::LOBBY] = new CLobbyScene;
	m_arrScene[(UINT)SCENE_TYPE::INGAME] = new CInGameScene;
	m_arrScene[(UINT)SCENE_TYPE::INGAME]->SetName(L"PlayScene");
	m_pCurScene[index] = m_arrScene[(UINT)SCENE_TYPE::INGAME];
	m_pCurScene[index]->Init(index);
	m_pCurScene[index]->isinit = true;

	for (auto& cl : SHARED_DATA::g_clients) {
		if (cl.second.room_id == index) {
			cout << "ID ["<<cl.second.m_id << "] 방입장 패킷 전송" << endl;
			CService::GetInst()->enter_game(cl.second.m_id);
		}
	}
}

void CSceneMgr::Update()
{

	for (auto scene : m_pCurScene) {
		if (scene.second->isinit) {
			scene.second->Update();
			scene.second->LateUpdate();
			scene.second->FinalUpdate();
			CSensorMgr::GetInst()->Update(scene.first);
			CCollisionMgr::GetInst()->Update(scene.first);
			CEventMgr::GetInst()->Update(scene.first);
		}
	}
	// 충돌 처리

}

void CSceneMgr::EnterGame(int index)
{
	ChangeScene(SCENE_TYPE::INGAME , index);
}



void CSceneMgr::Init(SCENE_TYPE _eType)
{
}



void CSceneMgr::FindGameObjectByTag(int index, const wstring& _strTag, vector<CGameObject*>& _vecFindObj )
{
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        const vector<CGameObject*>& vecObject = m_pCurScene[index]->GetLayer(i)->GetObjects();
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

void CSceneMgr::InitArrowByPlayerId(int index , int ClientId,int ArrowId ,Vec3 Pos, Vec3 Rot, Vec3 Dir, float Power, CAMP_STATE camp , PACKET_SKILL skill)
{
	if (camp == CAMP_STATE::RED) {
		for (auto obj : m_pCurScene[index]->FindLayer(L"Red")->GetParentObj()) {
			if (obj->GetScript<CPlayerScript>()->m_GetId() == ClientId) {
				obj->GetScript<CPlayerScript>()->InitArrow(ArrowId, Pos, Rot, Dir, Power , skill);
				break;
			}
		}
	}

	if (camp == CAMP_STATE::BLUE) {
		for (auto obj : m_pCurScene[index]->FindLayer(L"Blue")->GetParentObj()) {
			if (obj->GetScript<CPlayerScript>()->m_GetId() == ClientId) {
				obj->GetScript<CPlayerScript>()->InitArrow(ArrowId, Pos, Rot, Dir, Power, skill);
				break;

			}
		}
	}
}

#include "MinionScript.h"
void CSceneMgr::collisionArrow(int index, int coll_id, PACKET_COLLTYPE coll_type, CAMP_STATE Arrow_camp)
{
	if (coll_type == PACKET_COLLTYPE::MONSTER) {
		if (Arrow_camp == CAMP_STATE::RED) {
			for (auto &obj : m_pCurScene[index]->FindLayer(L"Blue")->GetParentObj()) {
				if (obj->GetScript<CMinionScript>() != nullptr && obj->GetScript<CMinionScript>()->m_GetId() == coll_id) {
					obj->GetScript<CMinionScript>()->GetDamage(500);
					//obj->GetScript<CPlayerScript>()->InitArrow(ArrowId, Pos, Rot, Dir, Power, skill);
					break;
				}
			}
		}

		if (Arrow_camp == CAMP_STATE::BLUE) {
			for (auto &obj : m_pCurScene[index]->FindLayer(L"Red")->GetParentObj()) {
				if (obj->GetScript<CMinionScript>() != nullptr &&  obj->GetScript<CMinionScript>()->m_GetId() == coll_id) {
					obj->GetScript<CMinionScript>()->GetDamage(500);
					//obj->GetScript<CPlayerScript>()->InitArrow(ArrowId, Pos, Rot, Dir, Power, skill);
					break;

				}
			}
		}
	}
}

void CSceneMgr::setDamage(int index, int coll_id, PACKET_COLLTYPE coll_type, CAMP_STATE camp , int damage)
{
	if (coll_type == PACKET_COLLTYPE::MONSTER) {
		if (camp == CAMP_STATE::RED) {
			for (auto& obj : m_pCurScene[index]->FindLayer(L"Blue")->GetParentObj()) {
				if (obj->GetScript<CMinionScript>() != nullptr && obj->GetScript<CMinionScript>()->m_GetId() == coll_id) {
					obj->GetScript<CMinionScript>()->GetDamage(damage);
					//obj->GetScript<CPlayerScript>()->InitArrow(ArrowId, Pos, Rot, Dir, Power, skill);
					break;
				}
			}
		}

		if (camp == CAMP_STATE::BLUE) {
			for (auto& obj : m_pCurScene[index]->FindLayer(L"Red")->GetParentObj()) {
				if (obj->GetScript<CMinionScript>() != nullptr && obj->GetScript<CMinionScript>()->m_GetId() == coll_id) {
					obj->GetScript<CMinionScript>()->GetDamage(damage);
					//obj->GetScript<CPlayerScript>()->InitArrow(ArrowId, Pos, Rot, Dir, Power, skill);
					break;

				}
			}
		}
	}
}