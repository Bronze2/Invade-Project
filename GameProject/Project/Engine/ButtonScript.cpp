#include "pch.h"
#include "ButtonScript.h"
#include "RenderMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "EventMgr.h"
#include "MeshRender.h"
#include "PlayerScript.h"

void CButtonScript::Awake()
{
	// 로비씬
	// 나!! 메인 플레이어
	m_pPlayerObj = CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Red")->GetParentObj()[1];

	for (int i = 0 ; i < m_pPlayerObj->GetChild().size() ; i++){
		if (m_pPlayerObj->GetChild()[i]->GetName() == L"Helmet") {
			m_pPlayerHelmet = m_pPlayerObj->GetChild()[i];
		}
		else if (m_pPlayerObj->GetChild()[i]->GetName() == L"UIReadyBar") {
			m_pReadyBar = m_pPlayerObj->GetChild()[i];
		}
	}

	m_pReadyNonePressedMtrl = new CMaterial;
	m_pReadyNonePressedMtrl->DisableFileSave();
	m_pReadyNonePressedMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	m_pReadyNonePressedTex = CResMgr::GetInst()->FindRes<CTexture>(L"UIReadyBar");

	m_pReadyPressedMtrl = new CMaterial;
	m_pReadyPressedMtrl->DisableFileSave();
	m_pReadyPressedMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TexShader"));
	m_pReadyPressedTex = CResMgr::GetInst()->FindRes<CTexture>(L"UIReadyPressed");
}

void CButtonScript::Update()
{
	StatusCheck();

	if (m_bIsPressed) {
		if (m_eType == BUTTON_TYPE::READY || !m_pPlayerObj->GetScript<CPlayerScript>()->GetReady()) {
			Execute();
			m_bIsPressed = false;
		}
	}

	if (m_pPlayerObj->GetScript<CPlayerScript>()->GetReady() && m_pPlayerObj->GetScript<CPlayerScript>()->GetAnimaionState() == PLAYER_STATE::IDLE) {
		m_pReadyBar->SetActive(true);
	}
}

void CButtonScript::Execute()
{
	switch (m_eType) {
	case BUTTON_TYPE::READY:
		if (!m_pPlayerObj->GetScript<CPlayerScript>()->GetReady()) {
			m_pPlayerObj->GetScript<CPlayerScript>()->SetAnimationState(PLAYER_STATE::JUMP);
			GetObj()->MeshRender()->SetMaterial(m_pReadyPressedMtrl);
			GetObj()->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, m_pReadyPressedTex.GetPointer());
			m_pPlayerObj->GetScript<CPlayerScript>()->SetReady(true);
			// 잠금버튼
			UINT iType = m_pPlayerObj->GetScript<CPlayerScript>()->GetType();
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->GetParentObj()[2 + iType]->MeshRender()->SetRender(true);

		}
		else {
			m_pReadyBar->SetActive(false);
			GetObj()->MeshRender()->SetMaterial(m_pReadyNonePressedMtrl);
			GetObj()->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, m_pReadyNonePressedTex.GetPointer());
			m_pPlayerObj->GetScript<CPlayerScript>()->SetReady(false);

			UINT iType = m_pPlayerObj->GetScript<CPlayerScript>()->GetType();
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->GetParentObj()[2 + iType]->MeshRender()->SetRender(false);

		}
		break;
	case BUTTON_TYPE::GAMESTART:
		ChangeScene(SCENE_TYPE::INGAME);
		break;
	case BUTTON_TYPE::SELECT_WATER:
		if (m_pPlayerObj->GetLayerIdx() == 3) {
			m_pPlayerHelmet->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Blue01.fbx")->GetMesh());
		}
		else if (m_pPlayerObj->GetLayerIdx() == 4) {
			m_pPlayerHelmet->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Red01.fbx")->GetMesh());
		}
		m_pPlayerObj->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::WATER);
		break;
	case BUTTON_TYPE::SELECT_FIRE:
		if (m_pPlayerObj->GetLayerIdx() == 3) {
			m_pPlayerHelmet->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Blue02.fbx")->GetMesh());
		}
		else if (m_pPlayerObj->GetLayerIdx() == 4) {
			m_pPlayerHelmet->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Red02.fbx")->GetMesh());
		}
		m_pPlayerObj->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::FIRE);
		break;
	case BUTTON_TYPE::SELECT_DARK:
		if (m_pPlayerObj->GetLayerIdx() == 3) {
			m_pPlayerHelmet->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Blue03.fbx")->GetMesh());
		}
		else if (m_pPlayerObj->GetLayerIdx() == 4) {
			m_pPlayerHelmet->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Red03.fbx")->GetMesh());
		}
		m_pPlayerObj->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::DARK);
		break;
	case BUTTON_TYPE::SELECT_THUNDER:
		if (m_pPlayerObj->GetLayerIdx() == 3) {
			m_pPlayerHelmet->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Blue04.fbx")->GetMesh());
		}
		else if (m_pPlayerObj->GetLayerIdx() == 4) {
			m_pPlayerHelmet->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Red04.fbx")->GetMesh());
		}
		m_pPlayerObj->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::THUNDER);
		break;
	case BUTTON_TYPE::SELECT_WIND:
		if (m_pPlayerObj->GetLayerIdx() == 3) {
			m_pPlayerHelmet->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Blue05.fbx")->GetMesh());
		}
		else if (m_pPlayerObj->GetLayerIdx() == 4) {
			m_pPlayerHelmet->MeshRender()->SetMesh(CResMgr::GetInst()->LoadFBX(L"FBX\\Helmet_Red05.fbx")->GetMesh());
		}
		m_pPlayerObj->GetScript<CPlayerScript>()->SetType(ELEMENT_TYPE::WIND);
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

	m_ptCursorPos = CKeyMgr::GetInst()->GetMousePos();
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