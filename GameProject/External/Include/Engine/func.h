#pragma once
#include "SceneMgr.h"
int GetSizeofFormat(DXGI_FORMAT _eFormat);

template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	for (auto& pair : _map)
	{
		SAFE_DELETE(pair.second);
	}

	_map.clear();
}

template<typename T, int iSize>
void Safe_Delete_Array(T* (&_arr)[iSize])
{
	for (UINT i = 0; i < iSize; ++i)
	{
		SAFE_DELETE(_arr[i]);
	}
}


template<typename T>
void Safe_Delete_Vector(vector<T*> _vec)
{
	for (UINT i = 0; i < _vec.size(); ++i)
	{
		SAFE_DELETE(_vec[i]);
	}
}

void SaveWString(FILE* _pFile, const wstring& _str);

wchar_t* LoadWString(FILE* _pFile);
Matrix GetMatrix(FbxAMatrix& _mat);
Vec3 DeComposeRotMat(const Matrix& _matRot);

float Dot(const Vec3& _Value1,const Vec3& _Value2);
Vec3 Cross(const Vec3& _Value1, const Vec3& _Value2);
class CGameObject;
bool lengthCompare(Vec3 _vbeforeObject, Vec3 _vAfterPos,CGameObject* _pAfterObject, Vec3 _vTargetObject);


class CMaterial;

void ChangeScene(SCENE_TYPE _eNextScene);


Vec2 GetDiagnal(const float& _fDestination, const float& _fxvalue, const float& _fzvalue);
