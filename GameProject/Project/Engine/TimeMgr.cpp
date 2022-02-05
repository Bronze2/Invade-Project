#include "pch.h"
#include "TimeMgr.h"

#include "GameFramework.h"

CTimeMgr::CTimeMgr()
	:m_llCurCount{}, m_llOldCount{},m_llFrequency{},m_fDeltaTime(0.f),m_fAccTime(0.f),m_bFreeze(false)
{}
CTimeMgr::~CTimeMgr() {

}

void CTimeMgr::Init()
{
	QueryPerformanceFrequency(&m_llFrequency);
	QueryPerformanceCounter(&m_llOldCount);
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llOldCount.QuadPart) / (float)m_llFrequency.QuadPart;
	m_llOldCount = m_llCurCount;
	m_fAccTime += m_fDeltaTime;
	if (m_bFreeze) {
		m_bFreeze = false;
		m_fDeltaTime = 1.f / 60.f;
	}
	if (m_fAccTime > 1.f) {
		QueryPerformanceFrequency(&m_llFrequency);
		m_fAccTime = 0.f;
		m_fFPS = 1 / m_fDeltaTime;
	}
	g_global.fDT = m_fDeltaTime;
	g_global.fAccTime += m_fDeltaTime;
	wchar_t szFPS[50] = L"";
	wsprintf(szFPS, L"FPS: %d", (int)m_fFPS);
	SetWindowText(CGameFramework::GetInst()->m_hMainhWnd, szFPS);
}
