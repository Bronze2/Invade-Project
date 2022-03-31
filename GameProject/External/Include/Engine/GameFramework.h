#pragma once
class CGameFramework
{
	SINGLE(CGameFramework)
public:
	HWND m_hMainhWnd;
private:

	POINT m_ptOldCursorPos;
	Vec2 m_vMouseMove;

public:
	int Init(HWND _hWnd, const tResolution& _resolution, bool _bWindow);
	void Progress();
	void ProcessInput();
	void OnProcessingMouseMessage(HWND _hWnd, UINT _uMessageID, WPARAM _wParam, LPARAM _lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND _hWnd, UINT _uMessageID, WPARAM _wParam, LPARAM _lParam);

	const Vec2& GetMouseMove() { return m_vMouseMove; }

private:
	void ChangeWindowSize(HWND _hWnd, const tResolution _resolution);

};

