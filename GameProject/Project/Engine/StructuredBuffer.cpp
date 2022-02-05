#include "pch.h"
#include "StructuredBuffer.h"
#include "Device.h"
CStructuredBuffer::CStructuredBuffer():m_pBuffer(nullptr),m_pSRV(nullptr),m_pUAV(nullptr),m_iElementCount(0),m_iElementSize(0),m_eResState(D3D12_RESOURCE_STATE_COMMON)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}



void CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, void* _pSysMem)
{

	m_pBuffer = nullptr;
	m_pSRV = nullptr;
	m_pUAV = nullptr;
	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;
	if (_pSysMem)
		m_eResState = D3D12_RESOURCE_STATE_COPY_DEST;
	else
		m_eResState = D3D12_RESOURCE_STATE_COMMON;


	D3D12_RESOURCE_DESC tBufferDesc = {};
	tBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	tBufferDesc.Width = m_iElementSize * m_iElementCount;
	tBufferDesc.Height = 1;
	tBufferDesc.DepthOrArraySize = 1;
	tBufferDesc.MipLevels = 1;
	tBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	tBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	tBufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	tBufferDesc.SampleDesc.Count = 1;
	tBufferDesc.SampleDesc.Quality = 0;
	CD3DX12_HEAP_PROPERTIES value = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	DEVICE->CreateCommittedResource(&value, D3D12_HEAP_FLAG_NONE, &tBufferDesc, m_eResState, nullptr, IID_PPV_ARGS(&m_pBuffer));



	if (_pSysMem)
	{
		// �б� ���� ����
		ComPtr<ID3D12Resource> pReadBuffer = nullptr;

		// ReadBuffer Desc
		D3D12_RESOURCE_DESC tReadBufferDesc = {};
		tReadBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		tReadBufferDesc.Width = m_iElementSize * m_iElementCount;
		tReadBufferDesc.Height = 1;
		tReadBufferDesc.DepthOrArraySize = 1;
		tReadBufferDesc.MipLevels = 1;
		tReadBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
		tReadBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		tReadBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		tReadBufferDesc.SampleDesc.Count = 1;
		tReadBufferDesc.SampleDesc.Quality = 0;
		CD3DX12_HEAP_PROPERTIES value = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// Buffer Create
		DEVICE->CreateCommittedResource(&value,D3D12_HEAP_FLAG_NONE,&tReadBufferDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,IID_PPV_ARGS(&pReadBuffer));

		UINT8* pVertexDataBegin = nullptr;
		D3D12_RANGE readRange{ 0, 0 }; // We do not intend to read from this resource on the CPU.	
		pReadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
		memcpy(pVertexDataBegin, _pSysMem, (tReadBufferDesc.Width * tReadBufferDesc.Height));
		pReadBuffer->Unmap(0, nullptr);
		CD3DX12_RESOURCE_BARRIER value2 = CD3DX12_RESOURCE_BARRIER::Transition(m_pBuffer.Get()
			, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON);
		// Resource Copy
		CMDLIST_RES->CopyBufferRegion(m_pBuffer.Get(), 0, pReadBuffer.Get(), 0, m_iElementSize * m_iElementCount);
		CMDLIST_RES->ResourceBarrier(1, &value2);
		CDevice::GetInst()->ExcuteResourceLoad();

		m_eResState = D3D12_RESOURCE_STATE_COMMON;
	}
	D3D12_DESCRIPTOR_HEAP_DESC uavHeapDesc = {};
	uavHeapDesc.NumDescriptors = 1;
	uavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	uavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DEVICE->CreateDescriptorHeap(&uavHeapDesc, IID_PPV_ARGS(&m_pUAV));

	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_pUAV->GetCPUDescriptorHandleForHeapStart();
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = m_iElementCount;
	uavDesc.Buffer.StructureByteStride = m_iElementSize;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	DEVICE->CreateUnorderedAccessView(m_pBuffer.Get(), nullptr, &uavDesc, handle);

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_pSRV));
	handle = m_pSRV->GetCPUDescriptorHandleForHeapStart();
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;

	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = m_iElementCount;
	srvDesc.Buffer.StructureByteStride = m_iElementSize;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	DEVICE->CreateShaderResourceView(m_pBuffer.Get(), &srvDesc, handle);
}

void CStructuredBuffer::UpdateData(TEXTURE_REGISTER _iRegister)
{
	CDevice::GetInst()->SetBufferToRegister(this, _iRegister);
}

void CStructuredBuffer::UpdateData_CS(TEXTURE_REGISTER _iRegister)
{
	CDevice::GetInst()->SetBufferToSRVRegister_CS(this, _iRegister);
}

void CStructuredBuffer::UpdateRWData(UAV_REGISTER _iRegister)
{
	CDevice::GetInst()->SetBufferToUAVRegister_CS(this, _iRegister);
}

