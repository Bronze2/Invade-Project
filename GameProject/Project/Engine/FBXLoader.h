#pragma once
#include "global.h"

struct tKeyFrame {
	FbxAMatrix matTransform;
	double dTime;
};

struct tBone {
	wstring strBoneName;
	int iDepth;
	int iParentIndex;
	FbxAMatrix matOffset;
	FbxAMatrix matBone;
	vector<tKeyFrame> vecKeyFrame;
};
struct tAnimClip {
	wstring strName;
	FbxTime tStartTime;
	FbxTime tEndTime;
	FbxLongLong llTimeLength;
	FbxTime::EMode eMode;
};

class CMesh;

class CFBXLoader
{
private:
	FbxManager* m_pManager;
	FbxScene* m_pScene;
	FbxImporter* m_pImporter;
	vector<tContainer> m_vecContainer;
	vector<tBone*> m_vecBone;
	FbxArray<FbxString*> m_arrAnimName;
	vector<tAnimClip*> m_vecAnimClip;

	// 220711 추가
	//Vec3 vPosition;
	//Vec3 vTangent;
	//Vec3 vBinormal;
	//Vec3 vNormal;
	//Vec2 vUV;
	//vector<FbxVertex> vertices;
	//vector<UINT> indices;
	//unordered_map<FbxVertex, UINT> indexMapping;

public:
	void Init();
	void LoadFbx(const wstring& _strPath);

	int GetContainerCount() { return (int)m_vecContainer.size(); }
	const tContainer& GetContainer(int _iIdx) { return m_vecContainer[_iIdx]; }
	vector<tBone*>& GetBones() { return m_vecBone; }
	vector<tAnimClip*>& GetAnimClip() { return m_vecAnimClip; }

private:
	void LoadMeshDataFromNode(FbxNode* _pRoot);
	void LoadMesh(FbxMesh* _pFbxMesh);
	void LoadMaterial(FbxSurfaceMaterial* _pMtrlSur);
	void GetTangent(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetBinormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetNormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder);
	void GetUV(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iUVIndex, int _iLayerCnt);

	// 220711 추가
	Vec3 GetTangent(FbxMesh* _pMesh, int _iIdx, int _iVtxOrder);
	Vec3 GetBinormal(FbxMesh* _pMesh, int _iIdx, int _iVtxOrder);
	Vec3 GetNormal(FbxMesh* _pMesh, int _iIdx, int _iVtxOrder);
	Vec2 GetUV(FbxMesh* _pMesh, int _iIdx, int _iUVIndex);
	void InsertVertex(const Vec3& position, const Vec3& tangent, const Vec3& binormal, const Vec3& normal, const Vec2& uv);

	Vec4 GetMtrlData(FbxSurfaceMaterial* _pSurface, const char* _pMtrlName, const char* _pMtrlFactorName);
	wstring GetMtrlTextureName(FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty);

	void LoadTexture();
	void CreateMaterial();
	void LoadSkeleton(FbxNode* _pNode);
	void LoadSkeleton_Re(FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx);
	void LoadAnimationClip();
	void Triangulate(FbxNode* _pNode);
	void LoadAnimationData(FbxMesh* _pMesh, tContainer* _pContainer);
	void LoadWeightsAndIndices(FbxCluster* _pCluster, int _iBoneIdx, tContainer* _pContainer);
	void LoadOffsetMatrix(FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform, int _iBoneIdx, tContainer* _pContainer);
	void LoadKeyFrameTransform(FbxNode* _pNode, FbxCluster* _pCluster
		, const FbxAMatrix& _matNodeTransform, int _iBoneIdx, tContainer* _pContainer);

	int FindBoneIndex(string _strBoneName);
	FbxAMatrix GetTransform(FbxNode* _pNode);
	void CheckWeightAndIndices(FbxMesh* _pMesh, tContainer* _pContainer);
public:
	CFBXLoader();
	~CFBXLoader();

};

