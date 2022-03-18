#pragma once

#include "Network.h"

class CGameFramework
{
	SINGLE(CGameFramework)
public:
	HWND m_hMainhWnd;
public:
	int Init(HWND _hWnd, const tResolution& _resolution, bool _bWindow);
	void Progress();
	Network m_Network;

private:
	void ChangeWindowSize(HWND _hWnd, const tResolution _resolution);

};

