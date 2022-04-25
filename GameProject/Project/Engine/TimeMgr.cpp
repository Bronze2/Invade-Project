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
	QueryPerformanceCounter(&m_playerOldMoveCount);
}

bool CTimeMgr::GetPlayerMoveFPS()
{
	//SetPlayerCurMoveCount();
	//float elapsed = m_playerCurMoveCount.QuadPart - m_playerOldMoveCount.QuadPart;
	//float duringtime = (double)elapsed / (double)m_llFrequency.QuadPart;
	//duringtime *= 1000;	//ms로 변환
	////cout << duringtime << endl;
	//if (duringtime > 100) {
	//	SetPlayerOldMoveCount();
	//	return true;
	//}
	//else return false;
	if (m_fMoveDuringTime > 100) {
		SetPlayerOldMoveCount();
		return true;
	}
	else return false;
}

void CTimeMgr::SetNetworkEnd()
{
	QueryPerformanceCounter(&m_CheckNetworkTimeEnd);
	float elapsed = (float)(m_llCurCount.QuadPart - m_playerOldMoveCount.QuadPart) / (float)m_llFrequency.QuadPart;

	float NetWorkDuring = elapsed * 1000;

//	std::cout<<"패킷 송수신 시간:" << NetWorkDuring << "ms"<<std::endl;
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llOldCount.QuadPart) / (float)m_llFrequency.QuadPart;
	float elapsed = (float)(m_llCurCount.QuadPart - m_playerOldMoveCount.QuadPart) / (float)m_llFrequency.QuadPart;

	m_fMoveDuringTime = elapsed * 1000;


	m_llOldCount = m_llCurCount;
	m_fAccTime += m_fDeltaTime;
	if (m_bFreeze) {
		m_bFreeze = false;
		m_fDeltaTime = 1.f / 60.f;
	}
	static int iCallCount = 0;
	++iCallCount;
	if (m_fAccTime > 1.f) {
		QueryPerformanceFrequency(&m_llFrequency);
		m_fAccTime = 0.f;
		m_fFPS = 1 / m_fDeltaTime;
		wchar_t szFPS[50] = L"";
		
		wsprintf(szFPS, L"FPS: %d", (int)iCallCount);
		iCallCount = 0;
		SetWindowText(CGameFramework::GetInst()->m_hMainhWnd, szFPS);
	}
	g_global.fDT = m_fDeltaTime;
	g_global.fAccTime += m_fDeltaTime;

}
