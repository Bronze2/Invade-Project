#pragma once
struct tEvent
{
	EVENT_TYPE eType;
	DWORD_PTR  wParam;
	DWORD_PTR  lParam;
};

class CGameObject;
class CEventMgr
{
	SINGLE(CEventMgr);
private:
	vector<tEvent>			m_vecEvent;
	vector<tEvent>			m_vecDeadEvent;
	vector<CGameObject*>	m_vecDead; // ���� ���� ������Ʈ

public:
	void Update(int index);
	void Clear() { m_vecEvent.clear(); }
	void AddEvent(tEvent& _event);

	void Init(int index );

	const vector<tEvent>& GetEventList() { return m_vecEvent; }

private:
	void Execute(int index, tEvent& _event);
};

