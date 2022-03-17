#pragma once
#include "Resource.h"
#include "FBXLoader.h"
#include "Ptr.h"
#include "Texture.h"
#include "StructuredBuffer.h"

class CInstancingBuffer;
struct tIndexInfo {
	ComPtr<ID3D12Resource> pIB;
	D3D12_INDEX_BUFFER_VIEW tIdxView;
	DXGI_FORMAT eIdxFormat;
	UINT iIdxCount;
	void* pIdxSysMem;
};
class CMesh
	:public CResource
{
private:
	ComPtr<ID3D12Resource> m_pVB;
	D3D12_VERTEX_BUFFER_VIEW m_tVtxView;
	
	vector<tIndexInfo>m_vecIdxInfo;

	UINT m_iVtxSize;
	UINT m_iVtxCount;
	void* m_pVtxSysMem;

	vector<tMTAnimClip> m_vecAnimClip;
	vector<tMTBone> m_vecBones;


	CStructuredBuffer* m_pBoneFrameData;
	CStructuredBuffer* m_pBoneOffset;
public:
	void Create(UINT _iVtxSize, UINT _iVtxCount, BYTE* _pVtxSysMem, DXGI_FORMAT _iIdxFormat, UINT _iIdxCount, BYTE* _pIdxSysMem);
	static CMesh* CreateFromContainer(CFBXLoader& _Loader);
	void Render(UINT _iSubset=0);
	void Render_Particle(UINT _iInstanceCount, UINT _iSubset = 0);

	CMesh();
	virtual ~CMesh();

	UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }
	const vector<tMTBone>* GetBones() { return &m_vecBones; }
	const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }

	bool IsAnimMesh() { return !m_vecAnimClip.empty(); }
	CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; }
	CStructuredBuffer* GetBoneOffsetBuffer() { return m_pBoneOffset;}
	UINT GetBoneCount() { return (UINT)m_vecBones.size(); }


	virtual void Load(const wstring& _strFullPath,bool _bFBX=false);
	virtual void Save(const wstring& _strPath);

};

