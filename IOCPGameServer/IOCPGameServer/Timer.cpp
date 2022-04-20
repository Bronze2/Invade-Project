#include "Timer.h"


CTimeMgr::CTimeMgr(){

}
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
	static int iCallCount = 0;
	++iCallCount;
	if (m_fAccTime > 1.f) {
		QueryPerformanceFrequency(&m_llFrequency);
		m_fAccTime = 0.f;
		m_fFPS = 1 / m_fDeltaTime;

	
		iCallCount = 0;
	}


}
