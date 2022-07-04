#include "pch.h"
#include "PlayerScript.h"
#include "Collider3D.h"
#include "ArrowScript.h"
#include "Server.h"

void CPlayerScript::Init()
{
	m_iCurHp = 1000;
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


		pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow[i]);
		//pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow[i]);
		m_pArrow[i]->SetActive(false);
		GetObj()->AddChild(index,m_pArrow[i]);
	}
}

void CPlayerScript::Update()
{
	Transform()->SetLocalPos(SHARED_DATA::g_clients[m_id].Pos);
	Transform()->SetLocalRot(SHARED_DATA::g_clients[m_id].Rot);

}

void CPlayerScript::InitArrow(int ArrowId, Vec3 Pos, Vec3 Rot, Vec3 Dir, float Power,PACKET_SKILL skill)
{
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->Init();
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetIndex(index);
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetState(ARROW_STATE::ATTACK);
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetSpeed(Power*2);
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetDir(Dir);
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->SetParentId(m_GetId());
	m_pArrow[ArrowId]->GetScript<CArrowScript>()->m_SetId(ArrowId);
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
}



CPlayerScript::CPlayerScript() :CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
{
}

CPlayerScript::~CPlayerScript()
{
}

