#include "pch.h"
#include "PlayerScript.h"
#include "Collider3D.h"
#include "ArrowScript.h"
#include "Server.h"

void CPlayerScript::Init()
{
	m_iCurHp = 1500;
	m_uiMaxHp = 1500;
}

void CPlayerScript::GetDamage(const UINT& _uiDamage)
{
	m_iCurHp -= _uiDamage;

	if (m_iCurHp <= 0.f) {
		CServer::GetInst()->send_damage_player(m_GetId(), _uiDamage);
		//죽음상태 추가
	}
	else {
		cout << "데미지" << _uiDamage << endl;
		CServer::GetInst()->send_damage_player(m_GetId(), _uiDamage);
	}
}

void CPlayerScript::Awake()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene(index);


	for (int i = 0; i < 20; ++i) {
		m_pArrow[i] = new CGameObject;
		m_pArrow[i]->SetName(L"Arrow");

		//m_pArrow[i]->AddComponent(new CCollider3D());
		m_pArrow[i]->AddComponent(new CTransform());
		m_pArrow[i]->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		m_pArrow[i]->Transform()->SetLocalScale(Vec3(80.f, 1.f, 1.f));
		m_pArrow[i]->AddComponent(new CCollider3D);
		m_pArrow[i]->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
		m_pArrow[i]->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		m_pArrow[i]->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));

		//m_pArrow[i]->AddComponent(new CCollider2D);
		//m_pArrow[i]->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);

		m_pArrow[i]->AddComponent(new CArrowScript(ELEMENT_TYPE::AllElements));
		m_pArrow[i]->GetScript<CArrowScript>()->SetCamp(GetCamp());
		m_pArrow[i]->GetScript<CArrowScript>()->SetIndex(index);
		m_pArrow[i]->GetScript<CArrowScript>()->SetState(ARROW_STATE::IDLE);
		m_pArrow[i]->GetScript<CArrowScript>()->m_SetId(i);



		pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow[i]);
		//pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow[i]);
		m_pArrow[i]->SetActive(false);
		GetObj()->AddChild(index,m_pArrow[i]);
	}
}

void CPlayerScript::Update()
{

	if (!isDead) {
		SHARED_DATA::g_clients[m_id].Pos.y = 0;
		Transform()->SetLocalPos(SHARED_DATA::g_clients[m_id].Pos);
	}
	else {
		if (playerSpawner < std::chrono::high_resolution_clock::now()) {
			isDead = false;
			SHARED_DATA::g_clients[m_id].Pos = SpawnPlace;
			SHARED_DATA::g_clients[m_id].Pos.y = 0;
			CServer::GetInst()->send_player_spawn(m_id, SpawnPlace);
		}
	}
	//Transform()->SetLocalRot(SHARED_DATA::g_clients[m_id].Rot);

}

void CPlayerScript::InitArrow(int ArrowId, Vec3 Pos, Vec3 Rot, Vec3 Dir, float Power,PACKET_SKILL skill)
{
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetParent(GetObj());
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->Init();
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetIndex(index);
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK);
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetSpeed(Power*2);
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetDir(Dir);
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetParentId(m_GetId());
	//m_pArrow[ArrowId]->GetScript<CArrowScript>()->m_SetId(ArrowId);
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetCamp(GetCamp());
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetAttackDamge(Power/7);
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetPacketSkill(skill);

	m_pArrow[ArrowId]->ClearParent(index);
	m_pArrow[ArrowId]->Transform()->SetLocalPos(Pos);
	m_pArrow[ArrowId]->Transform()->SetLocalRot(Rot);
	m_pArrow[ArrowId]->SetActive(true);
	//cout << "플레이어 ID: " <<m_GetId() <<"화살 ID "<< ArrowId <<" 생성 , 파워"<< Power<<endl;
	CServer::GetInst()->send_create_arrow_packet(m_GetId(), ArrowId, Pos, Rot, skill);

}
#include "Collider3D.h"
void CPlayerScript::OnCollision3DEnter(CCollider3D* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"obstalce") {
		cout << " obstacle Collsion" << endl;
	}
}

void CPlayerScript::SetPlayerSpawner()
{
	cout << "PlayerSpawner On" << endl;
	isDead = true;
	playerSpawner = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(10000);
}



CPlayerScript::CPlayerScript() :CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
{
}

CPlayerScript::~CPlayerScript()
{
}

