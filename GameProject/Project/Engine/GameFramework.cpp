#include "pch.h"
#include "GameFramework.h"
#include "Device.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "PathMgr.h"
#include "ConstantBuffer.h"
#include "ResMgr.h"
#include "RenderMgr.h"
#include "SceneMgr.h"
#include "EventMgr.h"
#include "InstancingMgr.h"
#include "MRT.h"

CGameFramework::CGameFramework():m_hMainhWnd(nullptr) {

}
CGameFramework::~CGameFramework() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}


int CGameFramework::Init(HWND _hWnd, const tResolution& _resolution, bool _bWindow)
{
	m_hMainhWnd = _hWnd;
	ChangeWindowSize(m_hMainhWnd, _resolution);
	ShowWindow(_hWnd, true);
	if (FAILED(CDevice::GetInst()->Init(_hWnd, _resolution, _bWindow)))
		return E_FAIL;
	CRenderMgr::GetInst()->Init(_hWnd, _resolution, _bWindow);
	CDevice::GetInst()->CreateConstantBuffer(L"TRANSFORM_MATRIX", sizeof(tTransform), 512, CONST_REGISTER::b0);
	CDevice::GetInst()->CreateConstantBuffer(L"MATERIAL_PARAM", sizeof(tMtrlParam), 512, CONST_REGISTER::b1);
	CDevice::GetInst()->CreateConstantBuffer(L"ANIM2D", sizeof(tMtrlParam), 512, CONST_REGISTER::b2);
	
	CDevice::GetInst()->CreateConstantBuffer(L"LIGHT2D", sizeof(tLight2DInfo), 1, CONST_REGISTER::b3,true);
	CDevice::GetInst()->CreateConstantBuffer(L"LIGHT3D", sizeof(tLight3DInfo), 1, CONST_REGISTER::b4, true);
	CDevice::GetInst()->CreateConstantBuffer(L"GLOBAL VALUE", sizeof(tGlobalValue), 1, CONST_REGISTER::b5);

	CDevice::GetInst()->SetGlobalConstBufferToRegister(CDevice::GetInst()->GetCB(CONST_REGISTER::b3), 0);
	CDevice::GetInst()->SetGlobalConstBufferToRegister(CDevice::GetInst()->GetCB(CONST_REGISTER::b4), 0);


	CDevice::GetInst()->SetGlobalConstBufferToRegister(CDevice::GetInst()->GetCB(CONST_REGISTER::b5), 0);

	CInstancingMgr::GetInst()->Init();
	CPathMgr::Init();
	CKeyMgr::GetInst()->Init();
	CTimeMgr::GetInst()->Init();

	CResMgr::GetInst()->Init();

	CSceneMgr::GetInst()->Init();
	

	CEventMgr::GetInst()->Init();
	CIMGUIMgr::GetInst()->Init();
	return S_OK;

}
void CGameFramework::Progress()
{
	CKeyMgr::GetInst()->Update();

	CTimeMgr::GetInst()->Update();
	CSound::g_pFMOD->update();

	CEventMgr::GetInst()->Update();
	CSceneMgr::GetInst()->Update();
	CRenderMgr::GetInst()->Render();

	CEventMgr::GetInst()->Clear();
	CInstancingMgr::GetInst()->Clear();
}

void CGameFramework::ProcessInput()
{
	POINT ptCursorPos;
	//SetCursor(NULL);
	ShowCursor(TRUE);
	GetCursorPos(&ptCursorPos);
	POINT ptCursorPos2;
	GetCursorPos(&ptCursorPos2);
	RECT rtWindow;
	RECT rtClient;
	GetWindowRect(CRenderMgr::GetInst()->GethWnd(), &rtWindow);
	GetClientRect(CRenderMgr::GetInst()->GethWnd(), &rtClient);
	LONG CursorPosy = ptCursorPos2.y;
	ScreenToClient(CRenderMgr::GetInst()->GethWnd(), &ptCursorPos);
	if (rtWindow.right - 3 <= ptCursorPos.x) {
		ptCursorPos.x = rtWindow.left + 3;
		m_ptOldCursorPos.x = rtWindow.left;
		SetCursorPos(ptCursorPos.x, CursorPosy);
	}
	if (3 >= ptCursorPos.x) {
		ptCursorPos.x = rtWindow.right - 3;
		m_ptOldCursorPos.x = rtWindow.right;
		SetCursorPos(ptCursorPos.x, CursorPosy);
	}

	m_vMouseMove.x = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.f;
	m_vMouseMove.y = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.f;
	m_vMouseMove.y *= -1.f;

	m_ptOldCursorPos = ptCursorPos;	
}

void CGameFramework::OnProcessingMouseMessage(HWND _hWnd, UINT _uMessageID, WPARAM _wParam, LPARAM _lParam)
{
	switch (_uMessageID)
	{
		
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		SetCapture(_hWnd);
		GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		ReleaseCapture();
	default:
		break;
	}
}

LRESULT CGameFramework::OnProcessingWindowMessage(HWND _hWnd, UINT _uMessageID, WPARAM _wParam, LPARAM _lParam)
{
	switch (_uMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(_hWnd, _uMessageID, _wParam, _lParam);
		break;
	case VK_F9:
	{
		BOOL bFullScreenState = FALSE;
		CDevice::GetInst()->GetSwapChain()->GetFullscreenState(&bFullScreenState, NULL);
		CDevice::GetInst()->GetSwapChain()->SetFullscreenState(!bFullScreenState, NULL);
		CRenderMgr::GetInst()->Init(_hWnd, tResolution{ 640, 480 }, true);
		break;
	}
	default:
		break;
	}
	return (0);
}

void CGameFramework::ChangeWindowSize(HWND _hWnd, const tResolution _resolution)
{
	RECT rt = { 0,0,(int)_resolution.fWidth,(int)_resolution.fHeight };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(_hWnd, nullptr, -10, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

}
