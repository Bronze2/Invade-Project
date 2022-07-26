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

template<typename T>
void Safe_Delete_Vector2(vector<T> _vec)
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
int TickCheck(std::chrono::system_clock::time_point start, int _Time);
bool CoolTimeCheck(std::chrono::system_clock::time_point start, int _Time);
bool SkillFinalCheck(SKILL* _pSkill);

void CreateHitParticleObject(const Vec3& _Pos,const wstring& _strKey);
void CreateBoomParticleObject(const Vec3& _Pos, const wstring& _strKey);


void CreateThunderObject(const Vec3& _Pos,const UINT& _iLayerIdx);

//추가
void CreateThunderObject(const Vec3& _Pos, const CAMP_STATE& _iLayerIdx);

void SetListener(const Matrix& _ViewMatrix, const Vec3& _PlayerPos, const Vec3& _PlayerlastPos);

void SetListener(const Vec3& _vFrontDir,const Vec3& _vUpDir, const Vec3& _PlayerPos, const Vec3& _PlayerlastPos);

void CreateBoxObject(const Vec3& _vPos,const UINT&_idx);




//사운드

class CSound;


CSound* SetSound2D( const wstring& _Path);
CSound* SetSound3D(const wstring& _Path);
