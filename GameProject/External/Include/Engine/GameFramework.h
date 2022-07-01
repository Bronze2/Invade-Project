
#pragma once
class CGameFramework
{
	SINGLE(CGameFramework)
public:
	HWND m_hMainhWnd;
private:

	POINT m_ptOldCursorPos;
	Vec2 m_vMouseMove;
	tResolution m_tResolution;

public:
	int Init(HWND _hWnd, const tResolution& _resolution, bool _bWindow);
	void Progress();
	void ProcessInput();
	void OnProcessingMouseMessage(HWND _hWnd, UINT _uMessageID, WPARAM _wParam, LPARAM _lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND _hWnd, UINT _uMessageID, WPARAM _wParam, LPARAM _lParam);

	const Vec2& GetMouseMove() { return m_vMouseMove; }
	const tResolution& Resolution() { return m_tResolution; }
	void CleanUp();
	HWND GetMainHwnd() { return m_hMainhWnd; }
private:
	void ChangeWindowSize(HWND _hWnd, const tResolution _resolution);

};
