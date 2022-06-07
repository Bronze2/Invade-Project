#pragma once
#include "Resource.h"
#include <DirectTex/DirectXTex.h>
#include <DirectTex/DirectXTex.inl>

#ifdef _DEBUG
#pragma comment(lib, "DirectTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectTex\\DirectXTex.lib")
#endif
class CTexture:public CResource
{
private:
	ScratchImage m_Image;
	ComPtr<ID3D12Resource> m_pTex2D;
	ComPtr<ID3D12DescriptorHeap> m_pSRV;
	D3D12_RESOURCE_DESC m_tDesc;
	ComPtr<ID3D12DescriptorHeap> m_pRTV;
	ComPtr<ID3D12DescriptorHeap> m_pDSV;
	ComPtr<ID3D12DescriptorHeap> m_pUAV;

	D3D12_RESOURCE_STATES m_eState;

public:
	virtual void Load(const wstring& _strFullPath);
	virtual void Save(const wstring& _strFullPath);
	void Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat
		, const D3D12_HEAP_PROPERTIES& _HeapProperty, D3D12_HEAP_FLAGS _eHeapFlag
		, D3D12_RESOURCE_FLAGS _eResFlag, Vec4 _vClearColor = Vec4());

	void CreateFromResource(ComPtr<ID3D12Resource> _pTex2D);
	void Load(const wstring& _strFullPath, D3D12_CPU_DESCRIPTOR_HANDLE _CpuHandle);
	ComPtr<ID3D12Resource> GetTex2D() { return m_pTex2D; }
	ComPtr<ID3D12DescriptorHeap> GetSRV() { return m_pSRV; }
	ComPtr<ID3D12DescriptorHeap> GetRTV() { return m_pRTV; }
	ComPtr<ID3D12DescriptorHeap> GetDSV() { return m_pDSV; }
	ComPtr<ID3D12DescriptorHeap> GetUAV() { return m_pUAV; }

	D3D12_RESOURCE_STATES GetResState() { return m_eState; }
	void SetResState(D3D12_RESOURCE_STATES _eState) { m_eState = _eState; }
	float Width() { return (float)m_tDesc.Width; }
	float Height() { return (float)m_tDesc.Height; }

	CTexture();
	virtual ~CTexture();
};

