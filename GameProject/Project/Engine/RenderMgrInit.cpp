#include "pch.h"
#include "RenderMgr.h"

#include "Device.h"
#include "ResMgr.h"
#include "GameFramework.h"

void CRenderMgr::Init(HWND _hWnd, const tResolution& _res, bool _bWindow)
{
	m_hWnd = _hWnd;
	m_tResolution = _res;
	m_bWindowed = _bWindow;
	CreateMRT();
}

