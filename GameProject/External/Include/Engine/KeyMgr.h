#pragma once
enum class KEY_TYPE
{
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,

	KEY_SPACE,
	KEY_ENTER,
	KEY_LSHIFT,
	KEY_LCTRL,
	KEY_LALT,
	KEY_ESC,

	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,

	KEY_Q,
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_R,
	KEY_Z,
	KEY_L,
	KEY_K,

	KEY_LBTN,
	KEY_RBTN,

	KEY_NUM0,
	KEY_NUM1,
	KEY_NUM2,
	KEY_NUM3,
	KEY_NUM4,
	KEY_NUM5,
	KEY_NUM6,
	KEY_NUM7,
	KEY_NUM8,
	KEY_NUM9,

	KEY_END,
};
enum class KEY_STATE
{
	STATE_NONE,		
	STATE_TAB,		
	STATE_HOLD,		
	STATE_AWAY,		
};
struct tKeyState
{
	KEY_STATE	eState;
	bool		bPrevPress;
};
class CKeyMgr
{
	SINGLE(CKeyMgr);
private:
	vector<tKeyState>	m_vecKey;
	POINT				m_ptMouse;
	POINT				m_ptOldMouse;

	Vec2 m_vDragDir;
public:
	void Init();
	void Update();

public:
	KEY_STATE GetKeyState(KEY_TYPE _eKey) { return m_vecKey[(UINT)_eKey].eState; }
	const POINT& GetMousePos() { return m_ptMouse; }
	const POINT& GetPrevMousePos() { return m_ptOldMouse; }
	Vec2 GetDragDir() { return m_vDragDir; }
};

