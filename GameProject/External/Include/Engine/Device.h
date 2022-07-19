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
	ComPtr<ID3D11Device> d3d11Device;
	ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;
	ComPtr <ID3D11On12Device> m_d3d11On12Device;
	ComPtr<ID2D1Factory3> m_d2dFactory;
	ComPtr<ID2D1Device> m_d2dDevice;
	ComPtr<ID2D1DeviceContext> m_d2dDeviceContext;
	ComPtr<IDWriteFactory> m_dWriteFactory;
	ComPtr<ID3D11Resource> m_wrappedBackBuffers[2];
	D2D1_BITMAP_PROPERTIES1 m_bitmapProperties;
	ComPtr<ID2D1Bitmap1> m_d2dRenderTargets[2];
	//위에 폰트
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

	ID3D12DescriptorHeap* GetDscriptorHeap() { return m_vecDummyDescriptor[m_iCurDummyIdx].Get(); }

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
	ComPtr<ID2D1Bitmap1> GetD2DRenderTarget(const UINT& _Index) { return m_d2dRenderTargets[_Index]; }
	D2D1_BITMAP_PROPERTIES1 GetBitMapProPerties() { return m_bitmapProperties; }
	ComPtr<ID2D1DeviceContext> GetD2DDeviceContext() { return m_d2dDeviceContext; }
	ComPtr<ID3D11Resource> GetBackBuffer(const UINT& _Index) { return m_wrappedBackBuffers[_Index]; }
	ComPtr <ID3D11On12Device> GetD11OnD12Device() { return m_d3d11On12Device; }
	ComPtr<ID3D12CommandAllocator> GetCommandAlloc(){ return m_pCmdAlloc; }
	ComPtr<ID3D12GraphicsCommandList>GetCmdList() {return m_pCmdListGraphic;}
	ComPtr<ID3D12GraphicsCommandList>GetCmdListRes() { return m_pCmdListRes; }
	ComPtr<ID3D12GraphicsCommandList>GetCmdListCompute() { return m_pCmdListCompute; }
	ComPtr<ID3D12Device> GetDevice() { return m_pDevice; }
	ComPtr<ID3D12RootSignature> GetRootSignature(ROOT_SIG_TYPE _eType) { return m_arrSig[(UINT)_eType]; }
	CConstantBuffer* GetCB(CONST_REGISTER _eRegister) { return m_vecCB[(UINT)_eRegister]; }
	UINT GetSwapChainIndex() { return m_iCurTargetIdx; }
	ComPtr<IDXGISwapChain> GetSwapChain() { return m_pSwapChain; }
};

