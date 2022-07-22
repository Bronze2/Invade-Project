#pragma once
class CConstantBuffer;
class CStructuredBuffer;
#include "Ptr.h"
#include "Texture.h"
class CDevice
{
	SINGLE(CDevice)
public:
	HWND m_hWnd;
	tResolution m_tResolution;
	bool m_bWindowed;

private:

	ComPtr<ID3D12Device> m_pDevice;
	ComPtr<ID3D12CommandQueue> m_pCmdQueue;

	ComPtr<ID3D12CommandAllocator> m_pCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> m_pCmdListGraphic;

	ComPtr<ID3D12CommandAllocator> m_pCmdAllocRes;
	ComPtr<ID3D12GraphicsCommandList> m_pCmdListRes;


	ComPtr<ID3D12CommandQueue> m_pCmdQueueCompute;
	ComPtr<ID3D12CommandAllocator> m_pCmdAllocCompute;
	ComPtr<ID3D12GraphicsCommandList> m_pCmdListCompute;

	ComPtr<ID3D12Fence> m_pFence;
	ComPtr<IDXGIFactory> m_pFactory;
	ComPtr<ID3D12Debug> m_pDbgCtrl;

	ComPtr<IDXGISwapChain> m_pSwapChain;



	vector<ComPtr<ID3D12DescriptorHeap>> m_vecDummyDescriptor;
	UINT m_iCurDummyIdx;
	ComPtr<ID3D12DescriptorHeap> m_pInitDescriptor;
	UINT m_iCBVIncreSize;

	vector<D3D12_STATIC_SAMPLER_DESC> m_vecSamplerDesc;


	HANDLE m_hFenceEvent;
	size_t m_iFenceValue;
	UINT m_iCurTargetIdx;
	size_t m_iRTVHeapSize;

	vector<CConstantBuffer*> m_vecCB;
	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;

	ComPtr<ID3D12Fence> m_pFenceCompute;
	ComPtr<ID3D12DescriptorHeap> m_pDummyDescriptorCompute;
private:
	ComPtr<ID3D12RootSignature> m_arrSig[(UINT)ROOT_SIG_TYPE::END];
public:
	int Init(HWND _hWnd, const tResolution& _res, bool _bWindow);
	void CreateConstantBuffer(const wstring& _strName, size_t _iBufferSize, size_t _iMaxCount, CONST_REGISTER _eRegisterNum, bool _bGlobal = false);
	void Render_Start(float(&_arrFloat)[4]);
	void Render_Present();
	void WaitForFenceEvent();

	void WaitForFenceEvent_CS();
	void ClearDummyDescriptorHeap_CS();
	void UpdateTable_CS();
	void ExcuteComputeShader();
	void SetConstBufferToRegister_CS(CConstantBuffer* _pCB, UINT _iOffset);
	void SetUAVToRegister_CS(CTexture* _pTex, UAV_REGISTER _eRegister);
	void SetTextureToRegister_CS(CTexture* _pTex, TEXTURE_REGISTER _eRegister);



	void SetConstBufferToRegister(CConstantBuffer* _pCB, UINT _iOffset);
	void SetGlobalConstBufferToRegister(CConstantBuffer* _pCB, UINT _iOffset);
	void SetTextureToRegister(CTexture* _pTex, TEXTURE_REGISTER _eRegister);
	void ClearDummyDescriptorHeap(UINT _iDummyIndex);

	void UpdateTable();
	void ExcuteResourceLoad();



	void SetBufferToRegister(CStructuredBuffer* _pBuffer, TEXTURE_REGISTER _eRegister);
	void SetBufferToSRVRegister_CS(CStructuredBuffer* _pBuffer, TEXTURE_REGISTER _eRegister);
	void SetBufferToUAVRegister_CS(CStructuredBuffer* _pBuffer, UAV_REGISTER _eRegister);
private:
	void CreateSwapChain();
	void CreateRootSignature();
	void CreateSamplerDesc();

public:
	ComPtr<ID3D12GraphicsCommandList>GetCmdList() { return m_pCmdListGraphic; }
	ComPtr<ID3D12GraphicsCommandList>GetCmdListRes() { return m_pCmdListRes; }
	ComPtr<ID3D12GraphicsCommandList>GetCmdListCompute() { return m_pCmdListCompute; }
	ComPtr<ID3D12Device> GetDevice() { return m_pDevice; }
	ComPtr<ID3D12RootSignature> GetRootSignature(ROOT_SIG_TYPE _eType) { return m_arrSig[(UINT)_eType]; }
	CConstantBuffer* GetCB(CONST_REGISTER _eRegister) { return m_vecCB[(UINT)_eRegister]; }
	UINT GetSwapChainIndex() { return m_iCurTargetIdx; }
	ComPtr<IDXGISwapChain> GetSwapChain() { return m_pSwapChain; }
};

