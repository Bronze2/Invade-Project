#pragma once
#include "pch.h"
#include "func.h"
#include "Material.h"
#include"ParticleScript.h"


namespace RES_TYPE_STR
{
	const wchar_t* MATERIAL = L"MATERIAL";
	const wchar_t* MESH = L"MESH";
	const wchar_t* TEXTURE = L"TEXTURE";
	const wchar_t* SOUND = L"SOUND";
	const wchar_t* SHADER = L"SHADER";
};

const wchar_t* RES_TYPE_NAME[(UINT)RES_TYPE::END] =
{
	L"MATERIAL",
	L"MESH",
	L"TEXTURE",
	L"SOUND",
	L"SHADER",
};

int GetSizeofFormat(DXGI_FORMAT _eFormat)
{
	int iRetByte = 0;
	switch (_eFormat)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		iRetByte = 128;
		break;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		iRetByte = 96;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		iRetByte = 64;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		iRetByte = 16;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		iRetByte = 8;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
		iRetByte = 128;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R1_UNORM:
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		iRetByte = 64;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		iRetByte = 32;
		break;
		// These are compressed, but bit-size information is unclear.
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_UNKNOWN:
	default:
		return -1;
	}

	return iRetByte / 8;
}
#include "Layer.h"
#include "Scene.h"


void SaveWString(FILE* _pFile, const wstring& _str)
{
	BYTE c = (BYTE)_str.length();
	fwrite(&c, 1, 1, _pFile);
	fwrite(_str.c_str(), 2, c, _pFile);
}

wchar_t* LoadWString(FILE* _pFile)
{
	static wchar_t szStr[255] = {};

	BYTE c = 0;
	fread(&c, 1, 1, _pFile);
	fread(szStr, 2, c, _pFile);
	szStr[c] = 0;

	return szStr;
}

Matrix GetMatrix(FbxAMatrix& _mat)
{
	Matrix mat;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat.m[i][j] = (float)_mat.Get(i, j);
		}
	}
	return mat;
}
bool closeEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon())
{
	return (epsilon > std::abs(a - b));
}
Vec3 DeComposeRotMat(const Matrix& _matRot)
{
	Vec4 vMat[4];
	XMStoreFloat4(&vMat[0], _matRot.Right());   // 1
	XMStoreFloat4(&vMat[1], _matRot.Up());      // 2
	XMStoreFloat4(&vMat[2], _matRot.Front());   // 3
	XMStoreFloat4(&vMat[3], _matRot.Translation());//4

	Vec3 vNewRot;
	if (closeEnough(vMat[0].z, -1.0f))
	{
		float x = 0; //gimbal lock, value of x doesn't matter
		float y = XM_PI / 2;
		float z = x + atan2(vMat[1].x, vMat[2].x);
		vNewRot = Vec3{ x, y, z };
	}
	else if (closeEnough(vMat[0].z, 1.0f)) {
		float x = 0;
		float y = -XM_PI / 2;
		float z = -x + atan2(-vMat[1].x, -vMat[2].x);
		vNewRot = Vec3{ x, y, z };
	}
	else { //two solutions exist
		float y1 = -asin(vMat[0].z);
		float y2 = XM_PI - y1;

		float x1 = atan2f(vMat[1].z / cos(y1), vMat[2].z / cos(y1));
		float x2 = atan2f(vMat[1].z / cos(y2), vMat[2].z / cos(y2));

		float z1 = atan2f(vMat[0].y / cos(y1), vMat[0].x / cos(y1));
		float z2 = atan2f(vMat[0].y / cos(y2), vMat[0].x / cos(y2));

		//choose one solution to return
		//for example the "shortest" rotation
		if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
		{
			vNewRot = Vec3{ x1, y1, z1 };
		}
		else {
			vNewRot = Vec3{ x2, y2, z2 };
		}
	}
	return vNewRot;
}

float Dot(const Vec3& _Value1,const Vec3& _Value2)
{
	XMVECTOR v1 = XMLoadFloat3(&_Value1);
	XMVECTOR v2 = XMLoadFloat3(&_Value2);
	XMVECTOR x = XMVector3Dot(v1, v2);
	
	return XMVectorGetX(x);
}

Vec3 Cross(const Vec3& _Value1, const Vec3& _Value2)
{
	XMVECTOR v1 = XMLoadFloat3(&_Value1);
	XMVECTOR v2 = XMLoadFloat3(&_Value2);
	XMVECTOR R = XMVector3Cross(_Value1, _Value2);
	Vec3 result;
	XMStoreFloat3(&result, R);
	return result;
}

#include "GameObject.h"
#include "Transform.h"
bool lengthCompare(Vec3 _vbeforeObject,  Vec3 _vAfterPos, CGameObject* _pAfterObject, Vec3 _vTargetObject)
{
	Vec3 vScale=_pAfterObject->Transform()->GetLocalScale();
	Vec3 vRot = _pAfterObject->Transform()->GetLocalRot();

	Matrix matTranslation = XMMatrixTranslation(_vAfterPos.x, _vAfterPos.y, _vAfterPos.z);
	Matrix matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);

	Matrix matRot = XMMatrixRotationX(vRot.x);
	matRot *= XMMatrixRotationY(vRot.y);
	matRot *= XMMatrixRotationZ(vRot.z);

	Matrix matWorld = matScale * matRot * matTranslation;
	Vec3 vPos = matWorld.Translation();

	float length1=sqrt(pow(_vbeforeObject.x - _vTargetObject.x, 2) + pow(_vbeforeObject.z - _vTargetObject.z, 2));
	float length2 = sqrt(pow(vPos.x - _vTargetObject.x, 2) + pow(vPos.z - _vTargetObject.z, 2));
	if (length1 < length2) {
		return false;
	}
	else
		return true;
}
#include "EventMgr.h"

void ChangeScene(SCENE_TYPE _eNextScene)
{

	tEvent evn = {};
	evn.wParam = (DWORD_PTR)_eNextScene;
	evn.eType = EVENT_TYPE::CHANGE_SCENE;
	CEventMgr::GetInst()->AddEvent(evn);
}

Vec2 GetDiagnal(const float& _fDestination,const float& _fxvalue,const float& _fzvalue)
{
	float diagnal = sqrt(pow(_fxvalue, 2) + pow(_fzvalue, 2));
	float x = (_fDestination * _fxvalue) / diagnal;
	float z = (_fDestination * _fzvalue) / diagnal;
	return Vec2(x,z);
}


int TickCheck(std::chrono::system_clock::time_point start, int _Time)
{
	std::chrono::duration<double>sec = std::chrono::system_clock::now() - start;
	if (_Time <= sec.count()) {
		_Time += 1;
	}
	return _Time;
}

bool CoolTimeCheck(std::chrono::system_clock::time_point start, int _Time)
{
	std::chrono::duration<double>sec = std::chrono::system_clock::now() - start;
	if (_Time <= sec.count()) {
		return true;
	}
	return false;
}

bool SkillFinalCheck(SKILL* _pSkill)
{
	if (_pSkill->bFinal) {
		delete _pSkill;
		return true;
	}
	return false;
}
#include "ParticleSystem.h"
#include "ResMgr.h"
void CreateHitParticleObject(const Vec3& _Pos, const wstring& _strKey)
{
	CGameObject* pHitParticle = new CGameObject;
	pHitParticle->AddComponent(new CTransform);
	pHitParticle->AddComponent(new CParticleSystem);
	pHitParticle->AddComponent(new CParticleScript);
	pHitParticle->Transform()->SetLocalPos(_Pos);
	pHitParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(_strKey), L"ParticleUpdate2Mtrl");
	pHitParticle->ParticleSystem()->SetStartColor(Vec4(0.5f, 0.5f, 0.f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	pHitParticle->ParticleSystem()->SetEndColor(Vec4(0.8f, 1.f, 0.f, 1.0f));
	pHitParticle->ParticleSystem()->SetStartScale(5.f);
	pHitParticle->ParticleSystem()->SetEndScale(10.f);
	pHitParticle->GetScript<CParticleScript>()->SetCoolTime(0.5f);
	pHitParticle->GetScript<CParticleScript>()->SetTime();
	pHitParticle->FrustumCheck(false);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pHitParticle);


}
void CreateBoomParticleObject(const Vec3& _Pos, const wstring& _strKey)
{
	CGameObject* pHitParticle = new CGameObject;
	pHitParticle->AddComponent(new CTransform);
	pHitParticle->AddComponent(new CParticleSystem);
	pHitParticle->AddComponent(new CParticleScript);
	pHitParticle->Transform()->SetLocalPos(_Pos);
	pHitParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(_strKey), L"ParticleUpdate2Mtrl");
	pHitParticle->ParticleSystem()->SetStartColor(Vec4(0.f, 0.f, 0.f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	pHitParticle->ParticleSystem()->SetEndColor(Vec4(0.3f, 0.3f, 0.4f, 0.5f));
	pHitParticle->ParticleSystem()->SetStartScale(100.f);
	pHitParticle->ParticleSystem()->SetEndScale(10.f);
	pHitParticle->GetScript<CParticleScript>()->SetCoolTime(1.5f);
	pHitParticle->GetScript<CParticleScript>()->SetTime();
	pHitParticle->ParticleSystem()->SetMinLifeTime(1.f);
	pHitParticle->ParticleSystem()->SetMaxLifeTime(1.5f);
	pHitParticle->FrustumCheck(false);

	Ptr<CSound> m_pSound = CResMgr::GetInst()->FindRes<CSound>(L"explosion");
	m_pSound->PlaySound3D(_Pos, 1000.f);

	pHitParticle->GetScript<CParticleScript>()->SetSound(m_pSound);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pHitParticle);

}
#include "ThunderSkill1Script.h"

#include "MeshRender.h"
void CreateThunderObject(const Vec3& _Pos, int e_camp)
{
	CGameObject* pThunderObject = new CGameObject;
	pThunderObject->AddComponent(new CTransform);
	pThunderObject->AddComponent(new CParticleSystem);
	pThunderObject->AddComponent(new CThunderSkill1Script);
	pThunderObject->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Thunder"), L"ParticleUpdate5Mtrl");
	pThunderObject->ParticleSystem()->SetStartColor(Vec4(0.8f, 0.8f, 0.f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	pThunderObject->ParticleSystem()->SetEndColor(Vec4(1.f, 1.f, 1.f, 1.0f));
	pThunderObject->ParticleSystem()->SetStartScale(10.f);
	pThunderObject->ParticleSystem()->SetEndScale(10.f);
	pThunderObject->ParticleSystem()->SetMinLifeTime(1.f);
	pThunderObject->ParticleSystem()->SetMaxLifeTime(3.f);
	pThunderObject->FrustumCheck(false);
	pThunderObject->SetActive(true);
	Ptr<CTexture> pMagicTexture = nullptr;
	if ((CAMP_STATE)e_camp == CAMP_STATE::BLUE) {
		pMagicTexture = CResMgr::GetInst()->FindRes<CTexture>(L"MagicCircle");
	}
	else {
		pMagicTexture = CResMgr::GetInst()->FindRes<CTexture>(L"MagicCircle2");
	}
	Ptr<CSound> m_pSound = CResMgr::GetInst()->FindRes<CSound>(L"SparkSound");
	m_pSound->PlaySound3D(_Pos, 1000.f);

	pThunderObject->GetScript<CThunderSkill1Script>()->SetSound(m_pSound);


	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(_Pos.x, 2.f, _Pos.z));
	pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(800.f, 1.f, 800.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"Rect2Mesh"));
	CAMP_STATE temp;
	if ((CAMP_STATE)e_camp == CAMP_STATE::BLUE) {
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Texture00"));
		temp = CAMP_STATE::BLUE;
		cout << "Blue Camp Thunder" << endl;
	}
	else {
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Texture01"));
		temp = CAMP_STATE::RED;
		cout << "Red Camp Thunder" << endl;

	}
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Texture00"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pMagicTexture.GetPointer());
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);
	pThunderObject->GetScript<CThunderSkill1Script>()->SetTarget(pObject);
	//pThunderObject->GetScript<CThunderSkill1Script>()->SetLayer(_iLayerIdx);
	pThunderObject->GetScript<CThunderSkill1Script>()->SetCampState(temp);
	pThunderObject->GetScript<CThunderSkill1Script>()->SetIsMain(true);

	pThunderObject->Transform()->SetLocalPos(_Pos);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pThunderObject);

}



void Net_CreateThunderObject(const Vec3& _Pos, int e_camp)
{
	CGameObject* pThunderObject = new CGameObject;
	pThunderObject->AddComponent(new CTransform);
	pThunderObject->AddComponent(new CParticleSystem);
	pThunderObject->AddComponent(new CThunderSkill1Script);
	pThunderObject->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(L"Thunder"), L"ParticleUpdate5Mtrl");
	pThunderObject->ParticleSystem()->SetStartColor(Vec4(0.8f, 0.8f, 0.f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	pThunderObject->ParticleSystem()->SetEndColor(Vec4(1.f, 1.f, 1.f, 1.0f));
	pThunderObject->ParticleSystem()->SetStartScale(10.f);
	pThunderObject->ParticleSystem()->SetEndScale(10.f);
	pThunderObject->ParticleSystem()->SetMinLifeTime(1.f);
	pThunderObject->ParticleSystem()->SetMaxLifeTime(3.f);
	pThunderObject->FrustumCheck(false);
	pThunderObject->SetActive(true);
	Ptr<CTexture> pMagicTexture = nullptr;
	if ((CAMP_STATE)e_camp == CAMP_STATE::BLUE) {
		pMagicTexture = CResMgr::GetInst()->FindRes<CTexture>(L"MagicCircle");
	}
	else {
		pMagicTexture = CResMgr::GetInst()->FindRes<CTexture>(L"MagicCircle2");
	}

	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(_Pos.x, 2.f, _Pos.z));
	pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(800.f, 1.f, 800.f));
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"Rect2Mesh"));
	CAMP_STATE temp;
	if ((CAMP_STATE)e_camp == CAMP_STATE::BLUE) {
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Texture00"));
		temp = CAMP_STATE::BLUE;
		cout << "Blue Camp Thunder" << endl;
	}
	else {
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Texture01"));
		temp = CAMP_STATE::RED;
		cout << "Red Camp Thunder" << endl;

	}
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Texture00"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pMagicTexture.GetPointer());
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);
	pThunderObject->GetScript<CThunderSkill1Script>()->SetTarget(pObject);
	pThunderObject->GetScript<CThunderSkill1Script>()->SetCampState(temp);
	pThunderObject->GetScript<CThunderSkill1Script>()->SetIsMain(false);

	pThunderObject->Transform()->SetLocalPos(_Pos);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pThunderObject);

}

void CreateDestroyParticleObject(const Vec3& _Pos, const wstring& _strKey)
{
	CGameObject* pHitParticle = new CGameObject;
	pHitParticle->AddComponent(new CTransform);
	pHitParticle->AddComponent(new CParticleSystem);
	pHitParticle->AddComponent(new CParticleScript);
	pHitParticle->Transform()->SetLocalPos(_Pos);
	pHitParticle->ParticleSystem()->Init(CResMgr::GetInst()->FindRes<CTexture>(_strKey), L"ParticleUpdate2Mtrl");
	pHitParticle->ParticleSystem()->SetStartColor(Vec4(1.f, 1.f, 1.f, 1.f));//,m_vStartColor(Vec4(0.4f,0.4f,0.8f,1.4f)),m_vEndColor(Vec4(1.f,1.f,1.f,1.0f))
	pHitParticle->ParticleSystem()->SetEndColor(Vec4(0.3f, 0.3f, 0.4f, 0.5f));
	pHitParticle->ParticleSystem()->SetStartScale(900.f);
	pHitParticle->ParticleSystem()->SetEndScale(100.f);
	pHitParticle->GetScript<CParticleScript>()->SetCoolTime(1.5f);
	pHitParticle->GetScript<CParticleScript>()->SetTime();
	pHitParticle->ParticleSystem()->SetMinLifeTime(5.f);
	pHitParticle->ParticleSystem()->SetMaxLifeTime(5.f);


	//Ptr<CSound> m_pSound = CResMgr::GetInst()->FindRes<CSound>(L"explosion");
	//m_pSound->PlaySound3D(_Pos, 1000.f);

	//pHitParticle->GetScript<CParticleScript>()->SetSound(m_pSound);


	pHitParticle->FrustumCheck(false);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pHitParticle);

}


#include "BoxScript.h"
#include "Collider3D.h"
#include "BoxColScript.h"
void CreateBoxObject(const Vec3& _vPos, const UINT& _idx , BUFF_TYPE buff)
{
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\box_Up.mdat", L"MeshData\\box_Up.mdat");
	CGameObject* pBoxUp = pMeshData->Instantiate();

	pBoxUp->FrustumCheck(false);
	pBoxUp->SetActive(true);

	pBoxUp->Transform()->SetLocalPos(Vec3(_vPos.x, 70.f, _vPos.z + 45));
	pBoxUp->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));

	//'	Ptr<CSound> m_pSound = CResMgr::GetInst()->FindRes<CSound>(L"SparkSound");
	//'	m_pSound->PlaySound3D(_Pos, 1000.f);
	//'
	//'	pThunderObject->GetScript<CThunderSkill1Script>()->SetSound(m_pSound);

	pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\box_Bottom.mdat", L"MeshData\\box_Bottom.mdat");
	CGameObject* pBoxBottom = pMeshData->Instantiate();
	pBoxBottom->AddComponent(new CBoxScript);
	pBoxBottom->GetScript<CBoxScript>()->SetUp(pBoxUp);
	pBoxBottom->GetScript<CBoxScript>()->SetBuff(buff);
	pBoxBottom->GetScript<CBoxScript>()->SetIdx(_idx);
	pBoxBottom->Transform()->SetLocalPos(Vec3(_vPos.x, 50.f, _vPos.z));
	pBoxBottom->Transform()->SetLocalRot(Vec3(-PI / 2, 0.f, 0.f));
	pBoxBottom->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pBoxBottom->GetScript<CBoxScript>()->Init();
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Box")->AddGameObject(pBoxBottom);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Box")->AddGameObject(pBoxUp);

	Ptr<CMaterial>pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TransparencyMtrl");
	Ptr<CTexture> pTransparency = CResMgr::GetInst()->FindRes<CTexture>(L"Transparency");
	CGameObject* pColObj = new CGameObject;
	pColObj->SetName(L"BoxCol");
	pColObj->AddComponent(new CCollider3D);
	pColObj->AddComponent(new CBoxColScript);
	pColObj->AddComponent(new CTransform);
	pColObj->AddComponent(new CMeshRender);
	pColObj->Transform()->SetLocalPos(pBoxBottom->Transform()->GetLocalPos());
	pColObj->Transform()->SetLocalScale(pBoxBottom->Transform()->GetLocalScale());
	pColObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pColObj->MeshRender()->SetMaterial(pMtrl);
	pColObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTransparency.GetPointer());
	pColObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pColObj->Collider3D()->SetOffsetScale(Vec3(120.f, 100.f, 120.f));      // 80.f, 200.f, 80.f ?????
	pColObj->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	pBoxBottom->GetScript<CBoxScript>()->SetBoxCol(pColObj);
	pColObj->GetScript<CBoxColScript>()->SetBox(pBoxBottom);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Box")->AddGameObject(pColObj, false);

}




//»ç¿îµå
CSound* SetSound2D(const wstring& _Path)
{
	CSound* pSound = new CSound;
	wstring strFullPath = CPathMgr::GetResPath();
	strFullPath += _Path;
	pSound->Load(strFullPath);

	return pSound;
}

CSound* SetSound3D(const wstring& _Path)
{
	CSound* pSound = new CSound;
	wstring strFullPath = CPathMgr::GetResPath();
	strFullPath += _Path;
	pSound->Load3D(strFullPath);

	return pSound;
}


void SetListener(const Matrix& _ViewMatrix, const Vec3& _PlayerPos, const Vec3& _PlayerlastPos)
{
	Matrix invView = _ViewMatrix;
	FMOD_3D_ATTRIBUTES listener;
	listener.position = VecToFMOD(_PlayerPos);
	Vec3 vZAxis = invView.Front();
	vZAxis.Normalize();
	Vec3 vYAxis = invView.Up();
	vYAxis.Normalize();
	listener.forward = VecToFMOD(vZAxis);
	listener.up = VecToFMOD(vYAxis);
	listener.velocity = { (_PlayerPos.x - _PlayerlastPos.x) * DT,(_PlayerPos.y - _PlayerlastPos.y) * DT,(_PlayerPos.z - _PlayerlastPos.z) * DT };
	CSound::g_pFMOD->set3DListenerAttributes(0, &listener.position, &listener.velocity, &listener.forward, &listener.up);
}

void SetListener(const Vec3& _vFrontDir, const Vec3& _vUpDir, const Vec3& _PlayerPos, const Vec3& _PlayerlastPos)
{
	FMOD_3D_ATTRIBUTES listener;
	listener.position = VecToFMOD(_PlayerPos);
	Vec3 vZAxis = _vFrontDir;

	Vec3 vYAxis = _vUpDir;
	listener.forward = VecToFMOD(vZAxis);
	listener.up = VecToFMOD(vYAxis);
	listener.velocity = { (_PlayerPos.x - _PlayerlastPos.x) * DT,(_PlayerPos.y - _PlayerlastPos.y) * DT,(_PlayerPos.z - _PlayerlastPos.z) * DT };
	CSound::g_pFMOD->set3DListenerAttributes(0, &listener.position, &listener.velocity, &listener.forward, &listener.up);
}