#pragma once
#include "Entity.h"
class CConstantBuffer :public CEntity
{
private:
	ComPtr<ID3D12Resource> m_pBuffer;
	ComPtr<ID3D12DescriptorHeap> m_pCBV;
	D3D12_CPU_DESCRIPTOR_HANDLE m_hCBV;
	UINT m_iBufferSize;
	UINT m_iMaxCount;
	UINT m_iCurCount;
	CONST_REGISTER m_eRegisterNum;
	BYTE* m_pData;

	D3D12_DESCRIPTOR_HEAP_DESC m_tHeapDesc;

public:
	void Create(UINT _iBufferSize, UINT _iMaxCount, CONST_REGISTER _eRegisterNum);
	UINT AddData(void* _pSrc);
	void Clear() { m_iCurCount = 0; }

	ComPtr<ID3D12DescriptorHeap> GetCBV() { return m_pCBV; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() { return m_hCBV; }
	CONST_REGISTER GetRegisterNum() { return m_eRegisterNum; }

public:
	CConstantBuffer();
	virtual ~CConstantBuffer();

};