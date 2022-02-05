#pragma once
#include "Entity.h"
class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D12Resource> m_pBuffer;
    ComPtr<ID3D12DescriptorHeap> m_pSRV;
    ComPtr<ID3D12DescriptorHeap> m_pUAV;
    UINT m_iElementSize;
    UINT m_iElementCount;
    D3D12_RESOURCE_STATES m_eResState;
public:
    void Create(UINT _iElementSize, UINT _iElementCount, void* _pSysMem);
    void UpdateData(TEXTURE_REGISTER _iRegister);
    void UpdateData_CS(TEXTURE_REGISTER _iRegister);
    void UpdateRWData(UAV_REGISTER _iRegister);

    ComPtr<ID3D12DescriptorHeap> GetSRV() { return m_pSRV; }
    ComPtr<ID3D12DescriptorHeap> GetUAV() { return m_pUAV; }

    void SetResState(D3D12_RESOURCE_STATES _eState) { m_eResState = _eState; }
    D3D12_RESOURCE_STATES GetResState() { return m_eResState; }
    ComPtr<ID3D12Resource> GetBuffer() { return m_pBuffer; }

    UINT GetElementSize() { return m_iElementSize; }
    UINT GetElementCount() { return m_iElementCount; }
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }

public:
    CStructuredBuffer();
    virtual ~CStructuredBuffer();
    
};

