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

CGameFramework::CGameFramework():m_hMainhWnd(nullptr) {

}
CGameFramework::~CGameFramework() {

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

	CPathMgr::Init();
	CKeyMgr::GetInst()->Init();
	CTimeMgr::GetInst()->Init();

	CResMgr::GetInst()->Init();

	CSceneMgr::GetInst()->Init();
	

	CEventMgr::GetInst()->Init();
	return S_OK;

}

void CGameFramework::Progress()
{
	CKeyMgr::GetInst()->Update();

	CTimeMgr::GetInst()->Update();
	CSound::g_pFMOD->update();
	CEventMgr::GetInst()->Clear();

	CSceneMgr::GetInst()->Update();
	CRenderMgr::GetInst()->Render();

	CEventMgr::GetInst()->Update();

}

void CGameFramework::ProcessInput()
{

	POINT ptCursorPos;
	
		SetCursor(NULL);
		GetCursorPos(&ptCursorPos);
		m_vMouseMove.x = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.f;
		m_vMouseMove.y = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.f;
		m_vMouseMove.y *= -1.f;

		m_ptOldCursorPos = ptCursorPos;
	//	SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	
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
	default:
		break;
	}
	return (0);
}

void CGameFramework::ChangeWindowSize(HWND _hWnd, const tResolution _resolution)
{
	RECT rt = { 0,0,(int)_resolution.fWidth,(int)_resolution.fHeight };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

}
