#pragma once
#define _XM_NO_INTRINSICS_

#include <d3d12.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <algorithm>
#include "SimpleMath.h"

#pragma comment(lib,"d3d12")
#pragma comment(lib,"dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory.h>
#include <stdint.h>
#include <assert.h>
#include <sstream>
#include <condition_variable>
#include <d3d11on12.h>
#include <fbxsdk.h>

#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>


// D2D (for Render Text)
#include <d2d1_3.h>
#include <dwrite.h>
#include <d3d11on12.h>
#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/x64/debug/libfbxsdk-md.lib")
#else
#pragma comment(lib, "FBXLoader/x64/release/libfbxsdk-md.lib")
#endif

using std::vector;
using std::list;
using std::map;
using std::string;
using std::wstring;
using std::make_pair;

#include "d3dx12.h"
#include "define.h"
#include "struct.h"
#include "func.h"
#include "extern.h"

