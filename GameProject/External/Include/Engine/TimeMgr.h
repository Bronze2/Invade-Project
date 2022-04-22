#pragma once
class CTimeMgr
{
	SINGLE(CTimeMgr);

private:
	LARGE_INTEGER	m_llOldCount;
	LARGE_INTEGER   m_llCurCount;
	LARGE_INTEGER   m_llFrequency;
	LARGE_INTEGER	m_playerOldMoveCount;
	LARGE_INTEGER	m_playerCurMoveCount;

	float			m_fDeltaTime;
	float			m_fAccTime;
	float			m_fFPS;

	bool			m_bFreeze; // DT �� ���ϰ� �Ѿ�� ����
	float m_fMoveDuringTime;
public:
	void Init();
	void Update();

public:
	float GetDeltaTime() { return m_fDeltaTime; }
	void DTFreeze() { m_bFreeze = true; }
	void SetPlayerOldMoveCount() { QueryPerformanceCounter(&m_playerOldMoveCount); }
	void SetPlayerCurMoveCount() { QueryPerformanceCounter(&m_playerCurMoveCount); }
	bool GetPlayerMoveFPS();

};

