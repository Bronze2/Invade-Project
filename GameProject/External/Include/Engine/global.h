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
#include <fstream>
#include <comdef.h>
#include <memory.h>
#include <stdint.h>
#include <assert.h>
#include <sstream>
#include <condition_variable>

#include <fbxsdk.h>
#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/x64/debug/libfbxsdk-md.lib")
#else
#pragma comment(lib, "FBXLoader/x64/release/libfbxsdk-md.lib")
#endif

#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "mswsock.lib")

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
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "IMGUIMgr.h"


struct Vertex
{
	Vertex() {}
	Vertex(
		const DirectX::XMFLOAT3& p,
		const DirectX::XMFLOAT3& n,
		const DirectX::XMFLOAT3& t,
		const DirectX::XMFLOAT2& uv) :
		Pos(p),
		Normal(n),
		Tangent(t),
		TexC(uv) {}
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v) :
		Pos(px, py, pz),
		Normal(nx, ny, nz),
		Tangent(tx, ty, tz),
		TexC(u, v) {};

	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexC;
	DirectX::XMFLOAT3 Tangent;
	DirectX::XMFLOAT3 Binormal;

	bool operator==(const Vertex& other) const
	{
		if (Pos.x != other.Pos.x || Pos.y != other.Pos.y || Pos.z != other.Pos.z)
			return false;

		if (Normal.x != other.Normal.x || Normal.y != other.Normal.y || Normal.z != other.Normal.z)
			return false;

		if (TexC.x != other.TexC.x || TexC.y != other.TexC.y)
			return false;

		return true;
	}
};