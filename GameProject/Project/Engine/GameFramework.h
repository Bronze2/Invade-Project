#pragma once

class CGameFramework
{
	SINGLE(CGameFramework)
public:
	HWND m_hMainhWnd;
public:
	int Init(HWND _hWnd, const tResolution& _resolution, bool _bWindow);
	void Progress();

private:
	void ChangeWindowSize(HWND _hWnd, const tResolution _resolution);

};

