// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN   



// 여기에 미리 컴파일하려는 헤더 추가

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include "global.h"
#include "d3dUtil.h";
#include <WS2tcpip.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <unordered_map>
#include <chrono>

#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// D2D (for Render Text)
#include <d2d1_3.h>
#include <dwrite.h>
#include <d3d11on12.h>



#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3d11.lib")

#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "mswsock.lib")

// 콘솔창 띄우기

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif

using namespace std;
using namespace chrono;

#endif //PCH_H
