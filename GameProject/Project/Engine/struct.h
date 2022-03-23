#pragma once

#include "define.h"

struct tResolution
{
	float fWidth;
	float fHeight;
};

// ���� ����ü
struct VTX
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;

	Vec3 vTangent;
	Vec3 vNormal;
	Vec3 vBinormal;

	Vec4 vWeights;
	Vec4 vIndices;
};

struct tDBG
{
	DBG_TYPE eType;
	wstring  strDBG;
};


struct tLightColor
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
};

struct tParticle {
	Vec3 vWorldPos;
	Vec3 vWorldDir;
	float m_fCurTime;
	float m_fLifeTime;
	int iAlive;
	int arrPadding[3];
};

struct tParticleShared {
	int iAddCount;
	int arrPadding[3];
};

union uInstID {
	struct {
		UINT iMesh;
		WORD iMtrl;
		WORD iMtrlIdx;
	};
	ULONG64 IIID;
};
union uInstBufferID {
	struct {
		UINT iMesh;
		UINT iMtrl;
	};
	ULONG64 IIID;
};
class CGameObject;
struct tInstObj {
	CGameObject* pObj;
	UINT iMtrlIdx;
};



// ������� �޸�
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
	Matrix matWV;
	Matrix matWVP;
	Matrix matWorldInv;
	Matrix matViewInv;
	Matrix matProjInv;
};

extern tTransform g_transform;



struct tMtrlParam
{
	Vec4			m_vDiff; // ���� ���
	Vec4			m_vSpec; // �ݻ籤 ���
	Vec4			m_vEmv;  // �߱� ���

	int				m_arrInt[(UINT)SHADER_PARAM::INT_END - (UINT)SHADER_PARAM::INT_0];
	float			m_arrFloat[(UINT)SHADER_PARAM::FLOAT_END - (UINT)SHADER_PARAM::FLOAT_0];
	Vec2			m_arrVec2[(UINT)SHADER_PARAM::VEC2_END - (UINT)SHADER_PARAM::VEC2_0];
	Vec4			m_arrVec4[(UINT)SHADER_PARAM::VEC4_END - (UINT)SHADER_PARAM::VEC4_0];
	Matrix			m_arrMat[(UINT)SHADER_PARAM::MATRIX_END - (UINT)SHADER_PARAM::MATRIX_0];

	int				m_iArrTex[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];
};


struct tAnim2D
{
	Vec2 vLT;			// UV �»��
	Vec2 vLen;			// UV ����
	Vec2 vOffset;		// Offset Pos
	int  IsAnim2D;		// Animation2D ������Ʈ ��뿩��
	int  iPadding;
};

struct tLight2D
{
	Vec4 vLightPos;
	Vec4 vLightColor;
	Vec4 vLightDir;
	int   iLightType;
	float fRange;
	float fAngle;
	int   ipadding;
};

struct tLight2DInfo
{
	tLight2D	arrLight2D[100];
	UINT		iCount;
};

struct tLight3D
{
	tLightColor tColor;
	Vec4		vLightPos;
	Vec4		vLightDir;
	int			iLightType;
	float		fRange;
	float		fAngle;
	int			iPadding;
};

struct tLight3DInfo
{
	tLight3D arrLight3D[100];
	UINT	 iCurCount;
	Vec3     vPadding;
};

struct tGlobalValue {
	tResolution vResolution;
	float fDT;
	float fAccTime;
};



//================
// Struct of FBX 
//=================
typedef struct _tagMtrlInfo
{
	Vec4	vMtrlDiff;
	Vec4	vMtrlSpec;
	Vec4	vMtrlAmb;
	Vec4	vMtrlEmiv;
	_tagMtrlInfo()
		: vMtrlDiff(1.f, 1.f, 1.f, 1.f)
		, vMtrlSpec(1.f, 1.f, 1.f, 1.f)
		, vMtrlAmb(1.f, 1.f, 1.f, 1.f)
		, vMtrlEmiv(1.f, 1.f, 1.f, 1.f)
	{}

}tMtrlInfo;

typedef struct _tagFbxMat
{
	tMtrlInfo	tMtrl;
	wstring     strMtrlName;
	wstring     strDiff;
	wstring		strNormal;
	wstring		strSpec;
}tFbxMaterial;

typedef struct _tagWeightsAndIndices
{
	int		iBoneIdx;
	double	dWeight;
}tWeightsAndIndices;

typedef struct _tagContainer
{
	wstring								strName;
	vector<Vec3>						vecPos;
	vector<Vec3>						vecTangent;
	vector<Vec3>						vecBinormal;
	vector<Vec3>						vecNormal;
	vector<Vec2>						vecUV;

	vector<Vec4>						vecIndices;
	vector<Vec4>						vecWeights;

	vector<vector<UINT>>				vecIdx;
	vector<tFbxMaterial>				vecMtrl;

	// Animation ���� ����
	bool								bAnimation;
	vector<vector<tWeightsAndIndices>>	vecWI;

	void Resize(UINT _iSize)
	{
		vecPos.resize(_iSize);
		vecTangent.resize(_iSize);
		vecBinormal.resize(_iSize);
		vecNormal.resize(_iSize);
		vecUV.resize(_iSize);
		vecIndices.resize(_iSize);
		vecWeights.resize(_iSize);
		vecWI.resize(_iSize);
	}

}tContainer;

struct tFrameTrans {
	Vec4 vTranslate;
	Vec4 vScale;
	Vec4 qRot;
};

// Animation
struct tMTKeyFrame
{
	double	dTime;
	int		iFrame;
	Vec3	vTranslate;
	Vec3	vScale;
	Vec4	qRot;
};

struct tMTBone
{
	wstring				strBoneName;
	int					iDepth;
	int					iParentIndx;
	Matrix				matOffset;	// Offset ���(�� -> ��Ʈ ������ ���)
	Matrix				matBone;   // �̰� �Ⱦ�
	vector<tMTKeyFrame>	vecKeyFrame;
};

struct tMTAnimClip
{
	wstring		strAnimName;
	int			iStartFrame;
	int			iEndFrame;
	int			iFrameLength;

	double		dStartTime;
	double		dEndTime;
	double		dTimeLength;
	float		fUpdateTime; // �̰� �Ⱦ�

	FbxTime::EMode eMode;
};

struct tBillBoard {
	Vec3 vWorldPos;
	Vec3 vWorldDir;
	Vec2 vPadding;
};