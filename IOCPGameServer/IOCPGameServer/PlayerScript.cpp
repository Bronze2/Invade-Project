#include "pch.h"
#include "PlayerScript.h"
#include "Collider2D.h"
#include "ArrowScript.h"

void CPlayerScript::Init()
{
}

void CPlayerScript::Awake()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	for (int i = 0; i < 20; ++i) {
		m_pArrow[i] = new CGameObject;
		m_pArrow[i]->SetName(L"Arrow");


		m_pArrow[i]->AddComponent(new CTransform());
		m_pArrow[i]->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		m_pArrow[i]->Transform()->SetLocalScale(Vec3(80.f, 1.f, 1.f));

		m_pArrow[i]->AddComponent(new CCollider2D);
		m_pArrow[i]->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);

		m_pArrow[i]->AddComponent(new CArrowScript(ELEMENT_TYPE::AllElements));
		pCurScene->FindLayer(L"Blue")->AddGameObject(m_pArrow[i]);
		//pCurScene->FindLayer(L"Arrow")->AddGameObject(m_pArrow[i]);
		m_pArrow[i]->SetActive(false);
		GetObj()->AddChild(m_pArrow[i]);
	}
}

void CPlayerScript::Update()
{
	Transform()->SetLocalPos(SHARED_DATA::g_clients[m_id].Pos);
	Transform()->SetLocalRot(SHARED_DATA::g_clients[m_id].Rot);
}




CPlayerScript::CPlayerScript() :CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
{
}

CPlayerScript::~CPlayerScript()
{
}

