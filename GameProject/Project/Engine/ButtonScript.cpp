#include "pch.h"
#include "ButtonScript.h"
#include "RenderMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "EventMgr.h"
#include "MeshRender.h"
#include "PlayerScript.h"
#include "Network.h"

void CButtonScript::Awake()
{
	// 로비씬
	// 나!! 메인 플레이어

	//for (auto cl : CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Red")->GetParentObj())
	//{
	////	cl->GetScript<CPlayerScript>()->
	//}
	//m_pPlayerObj = CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Red")->GetParentObj()[1];

	//for (int i = 0 ; i < m_pPlayerObj->GetChild().size() ; i++){
	//	if (m_pPlayerObj->GetChild()[i]->GetName() == L"Helmet") {
	//		m_pPlayerHelmet = m_pPlayerObj->GetChild()[i];
	//	}
	//	else if (m_pPlayerObj->GetChild()[i]->GetName() == L"UIReadyBar") {
	//		m_pReadyBar = m_pPlayerObj->GetChild()[i];
	//	}
	//}

	m_isPlayerInit = false;

	m_pReadyNonePressedMtrl = new CMaterial;
	m_pReadyNonePressedMtrl->DisableFileSave();
	m_pReadyNonePressedMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	m_pReadyNonePressedTex = CResMgr::GetInst()->FindRes<CTexture>(L"UIReadyBar");

	m_pReadyPressedMtrl = new CMaterial;
	m_pReadyPressedMtrl->DisableFileSave();
	m_pReadyPressedMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	m_pReadyPressedTex = CResMgr::GetInst()->FindRes<CTexture>(L"UIReadyPressed");
}

void CButtonScript::SetPlayerInit(int _mainPalyerId)
{


	for (int i = 0; i < 8; ++i)
	{
		m_pPlayerObj[i] = CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player")->GetParentObj()[i];
		cout << i << endl;
	}
	//for (auto cl : CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player")->GetParentObj())
	//{
	//	m_pPlayerObj[i] = cl;
	//}

	//for (auto cl : CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Blue")->GetParentObj())
	//{
	//	if (cl->GetScript<CPlayerScript>()->m_GetId() == _mainPalyerId) {
	//		m_pPlayerObj = cl;
	//		break;
	//	}
	//}
	m_mainPlayerId = _mainPalyerId;
	for (int j = 0; j < 8; ++j) {
		for (int i = 0; i < m_pPlayerObj[j]->GetChild().size(); i++) {
			if (m_pPlayerObj[j]->GetChild()[i]->GetName() == L"Helmet") {
				m_pPlayerHelmet[j] = m_pPlayerObj[j]->GetChild()[i];
			}
			else if (m_pPlayerObj[j]->GetChild()[i]->GetName() == L"UIReadyBar") {
				m_pReadyBar[j] = m_pPlayerObj[j]->GetChild()[i];
			}
		}
	}
	m_isPlayerInit = true;

}

void CButtonScript::SetPlayer(int _mainPalyerId)
{

}

void CButtonScript::Update()
{
	if (m_isPlayerInit) {
		StatusCheck();

		if (m_bIsPressed) {
			if (m_eType == BUTTON_TYPE::READY || !m_pPlayerObj[m_mainPlayerId]->GetScript<CPlayerScript>()->GetReady()) {
				Execute();
				m_bIsPressed = false;
			}
		}


		for (int i = 0; i < 8; ++i) {
			if (m_pPlayerObj[i]->GetScript<CPlayerScript>()->GetReady() && m_pPlayerObj[i]->GetScript<CPlayerScript>()->GetAnimaionState() == PLAYER_STATE::IDLE) {
				m_pReadyBar[i]->SetActive(true);
			}
		}
	}
}

void CButtonScript::Execute()
{
	for (int i = 0; i < 8; ++i) {
		if (m_pPlayerObj[i]->GetScript<CPlayerScript>()->m_GetId() == m_mainPlayerId) {
			switch (m_eType) {
			case BUTTON_TYPE::READY:

				if (!m_pPlayerObj[i]->GetScript<CPlayerScript>()->GetReady()) {
					GetObj()->MeshRender()->SetMaterial(m_pReadyPressedMtrl);
					GetObj()->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, m_pReadyPressedTex.GetPointer());
					Network::GetInst()->send_lobby_ready(true);
					UINT iType = m_pPlayerObj[i]->GetScript<CPlayerScript>()->GetType();
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->GetParentObj()[2 + iType]->MeshRender()->SetRender(true);
				}
				else {
					GetObj()->MeshRender()->SetMaterial(m_pReadyNonePressedMtrl);
					GetObj()->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, m_pReadyNonePressedTex.GetPointer());
					Network::GetInst()->send_lobby_ready(false);
					UINT iType = m_pPlayerObj[i]->GetScript<CPlayerScript>()->GetType();
					CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->GetParentObj()[2 + iType]->MeshRender()->SetRender(false);
				}
				break;
			case BUTTON_TYPE::GAMESTART:
					Network::GetInst()->send_game_start_packet();
				break;
			case BUTTON_TYPE::SELECT_WATER:
				if (!m_pPlayerObj[i]->GetScript<CPlayerScript>()->GetReady())
					Network::GetInst()->send_lobby_change_skill(ELEMENT_TYPE::WATER);
				break;
			case BUTTON_TYPE::SELECT_DARK:
				if (!m_pPlayerObj[i]->GetScript<CPlayerScript>()->GetReady())
					Network::GetInst()->send_lobby_change_skill(ELEMENT_TYPE::DARK);

				break;
			case BUTTON_TYPE::SELECT_WIND:
				if (!m_pPlayerObj[i]->GetScript<CPlayerScript>()->GetReady())
					Network::GetInst()->send_lobby_change_skill(ELEMENT_TYPE::WIND);

				break;
			case BUTTON_TYPE::SELECT_THUNDER:
				if (!m_pPlayerObj[i]->GetScript<CPlayerScript>()->GetReady())
					Network::GetInst()->send_lobby_change_skill(ELEMENT_TYPE::THUNDER);

				break;
			case BUTTON_TYPE::SELECT_FIRE:
				if (!m_pPlayerObj[i]->GetScript<CPlayerScript>()->GetReady())
					Network::GetInst()->send_lobby_change_skill(ELEMENT_TYPE::FIRE);

				break;
			}
		}
	}
}

void CButtonScript::ExecuteReady(int id)
{
	for (int i = 0; i < 8; ++i) {
		if (m_pPlayerObj[i]->GetScript<CPlayerScript>()->m_GetId() == id) {
			if (!m_pPlayerObj[i]->GetScript<CPlayerScript>()->GetReady()) {
				m_pPlayerObj[i]->GetScript<CPlayerScript>()->SetAnimationState(PLAYER_STATE::JUMP);
				m_pPlayerObj[i]->GetScript<CPlayerScript>()->SetReady(true);
			}
			else {
				m_pReadyBar[i]->SetActive(false);
				m_pPlayerObj[i]->GetScript<CPlayerScript>()->SetReady(false);
			}
		}
	}
}

//void CButtonScript::ExecuteChangeSkill(int id)
//{
//	int select_id = 999;
//	for (int i = 0; i < 8; ++i) {
//		if (m_pPlayerObj[i]->GetScript<CPlayerScript>()->m_GetId() == id) {
//			select_id = i;
//		}
//	}
//	switch (m_eType) {
//	case BUTTON_TYPE::SELECT_WATER:
//		if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::BLUE) {
//			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Blue01.fbx")->GetMesh());
//		}
//		else if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::RED) {
//			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Red01.fbx")->GetMesh());
//		}
//		break;
//	case BUTTON_TYPE::SELECT_DARK:
//		if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::BLUE) {
//			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Blue02.fbx")->GetMesh());
//		}
//		else if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::RED) {
//			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Red02.fbx")->GetMesh());
//		}
//		break;
//	case BUTTON_TYPE::SELECT_WIND:
//		if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::BLUE) {
//			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Blue03.fbx")->GetMesh());
//		}
//		else if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::RED) {
//			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Red03.fbx")->GetMesh());
//		}
//		break;
//	case BUTTON_TYPE::SELECT_THUNDER:
//		if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::BLUE) {
//			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Blue04.fbx")->GetMesh());
//		}
//		else if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::RED) {
//			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Red04.fbx")->GetMesh());
//		}
//		break;
//	case BUTTON_TYPE::SELECT_FIRE:
//		if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::BLUE) {
//			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Blue05.fbx")->GetMesh());
//		}
//		else if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::RED) {
//			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Red05.fbx")->GetMesh());
//		}
//		break;
//	}
//}


void CButtonScript::ExecuteChangeSkill(int id)
{
	int select_id = 999;
	for (int i = 0; i < 8; ++i) {
		if (m_pPlayerObj[i]->GetScript<CPlayerScript>()->m_GetId() == id) {
			select_id = i;
		}
	}
	switch (m_eType) {
	case BUTTON_TYPE::SELECT_WATER:
		if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::BLUE) {
			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Helmet_Blue05.mdat", L"MeshData\\Helmet_Blue05.mdat")->GetMesh());
		}
		else if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::RED) {
			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Helmet_Red05.mdat", L"MeshData\\Helmet_Red05.mdat")->GetMesh());
		}
		break;
	case BUTTON_TYPE::SELECT_FIRE:
		if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::BLUE) {
			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Helmet_Blue01.mdat", L"MeshData\\Helmet_Blue01.mdat")->GetMesh());
		}
		else if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::RED) {
			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Helmet_Red01.mdat", L"MeshData\\Helmet_Red01.mdat")->GetMesh());
		}
		break;
	case BUTTON_TYPE::SELECT_DARK:
		if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::BLUE) {
			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Helmet_Blue02.mdat", L"MeshData\\Helmet_Blue02.mdat")->GetMesh());
		}
		else if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::RED) {
			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Helmet_Red02.mdat", L"MeshData\\Helmet_Red02.mdat")->GetMesh());
		}
		break;
	case BUTTON_TYPE::SELECT_THUNDER:
		if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::BLUE) {
			// helmet_blue_04.mdat 안됨
			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Helmet_Blue04.mdat", L"MeshData\\Helmet_Blue04.mdat")->GetMesh());
		}
		else if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::RED) {
			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Helmet_Red04.mdat", L"MeshData\\Helmet_Red04.mdat")->GetMesh());
		}
		break;
	case BUTTON_TYPE::SELECT_WIND:
		if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::BLUE) {
			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Helmet_Blue03.mdat", L"MeshData\\Helmet_Blue03.mdat")->GetMesh());
		}
		else if (m_pPlayerObj[select_id]->GetScript<CPlayerScript>()->GetCamp() == CAMP_STATE::RED) {
			m_pPlayerHelmet[select_id]->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Helmet_Red03.mdat", L"MeshData\\Helmet_Red03.mdat")->GetMesh());
		}
		break;
	}
}

void CButtonScript::StatusCheck()
{
	ConvertPos();

	if ((m_ptCursorPos.x >= m_rcImg.left && m_ptCursorPos.x <= m_rcImg.right && m_ptCursorPos.y >= m_rcImg.bottom && m_ptCursorPos.y <= m_rcImg.top)
		&& KEY_TAB(KEY_TYPE::KEY_LBTN))
	{
		wcout << GetObj()->GetName();
		cout << " - 마우스 클릭됨" << endl;
		m_bIsPressed = true;
	}
}

void CButtonScript::ConvertPos()
{
	// 0------->
	// |    ^ 
	// |   	|
	// |	4--->				마우스 커서 좌표계 원점 0 
	// v      					UI 좌표계 원점 4 (화면 중앙)

	GetCursorPos(&m_ptCursorPos);
	ScreenToClient(CRenderMgr::GetInst()->GethWnd(), &m_ptCursorPos);
	tResolution res = CRenderMgr::GetInst()->GetResolution();

	m_ptCursorPos.x = m_ptCursorPos.x - res.fWidth / 2;
	m_ptCursorPos.y = res.fHeight / 2 - m_ptCursorPos.y;


	m_vPos = GetObj()->Transform()->GetLocalPos();
	m_vScale = GetObj()->Transform()->GetLocalScale();

	m_rcImg.left = m_vPos.x - m_vScale.x / 2;
	m_rcImg.right = m_vPos.x + m_vScale.x / 2;
	m_rcImg.top = m_vPos.y + m_vScale.y / 2;
	m_rcImg.bottom = m_vPos.y - m_vScale.y / 2;
}

CButtonScript::CButtonScript() : CScript(0), m_bIsPressed(false)
{
	
}

CButtonScript::~CButtonScript()
{

}