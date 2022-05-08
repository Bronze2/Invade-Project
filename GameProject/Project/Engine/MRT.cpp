#include "pch.h"
#include "MRT.h"
#include "Device.h"
#include "RenderMgr.h"

void CMRT::Create(UINT _iCount, tRT* _arrRT, Ptr<CTexture> _pDSTex)
{
	assert(nullptr != _pDSTex);
	m_iRTCount = _iCount;
	memcpy(m_arrRT, _arrRT, sizeof(tRT) * _iCount);
	m_pDSTex = _pDSTex;

	D3D12_DESCRIPTOR_HEAP_DESC tDesc = {};
	tDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	tDesc.NumDescriptors = m_iRTCount;
	tDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	tDesc.NodeMask = 0;
	DEVICE->CreateDescriptorHeap(&tDesc, IID_PPV_ARGS(&m_pRTV));
	D3D12_CPU_DESCRIPTOR_HANDLE pRTVHeap = m_pRTV->GetCPUDescriptorHandleForHeapStart();
	UINT iRTVSize = CRenderMgr::GetInst()->GetRTVHeapSize();
	for (UINT i = 0; i < m_iRTCount; ++i) {
		UINT iDestRange = 1;
		UINT iSrcRange = 1;
		D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_pRTV->GetCPUDescriptorHandleForHeapStart();
		hDescHandle.ptr += iRTVSize * i;
		ComPtr<ID3D12DescriptorHeap> pRTVHeap = m_arrRT[i].pTarget->GetRTV();
		D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = pRTVHeap->GetCPUDescriptorHandleForHeapStart();

		DEVICE->CopyDescriptors(1, &hDescHandle, &iDestRange, 1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	for (UINT i = 0; i < m_iRTCount; ++i) {
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_arrRT[i].pTarget->GetTex2D().Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		m_TargetToRes[i] = barrier;

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		m_ResToTarget[i] = barrier;
	}
	m_tVP = D3D12_VIEWPORT{ 0.f,0.f,m_arrRT[0].pTarget->Width(),m_arrRT[0].pTarget->Height(),0.f,1.f };
	m_tScissorRect = D3D12_RECT{ 0,0,(LONG)m_arrRT[0].pTarget->Width(),(LONG)m_arrRT[0].pTarget->Height() };

}

void CMRT::OMSet(UINT _iCount, UINT _iOffset)
{
	UINT iRTVSize = CRenderMgr::GetInst()->GetRTVHeapSize();
	D3D12_CPU_DESCRIPTOR_HANDLE hRTVHandle = m_pRTV->GetCPUDescriptorHandleForHeapStart();
	hRTVHandle.ptr += iRTVSize * _iOffset;

	D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = m_pDSTex->GetDSV()->GetCPUDescriptorHandleForHeapStart();

	CMDLIST->RSSetViewports(1, &m_tVP);
	CMDLIST->RSSetScissorRects(1, &m_tScissorRect);


	CMDLIST->OMSetRenderTargets(_iCount, &hRTVHandle, FALSE, &hDSVHandle);
}

void CMRT::OMSet()
{
	D3D12_CPU_DESCRIPTOR_HANDLE hRTVHandle = m_pRTV->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = m_pDSTex->GetDSV()->GetCPUDescriptorHandleForHeapStart();
	CMDLIST->RSSetViewports(1, &m_tVP);
	CMDLIST->RSSetScissorRects(1, &m_tScissorRect);

	CMDLIST->OMSetRenderTargets(m_iRTCount, &hRTVHandle, TRUE, &hDSVHandle);
}

void CMRT::TargetToResBarrier()
{
	CMDLIST->ResourceBarrier(m_iRTCount, m_TargetToRes);
}

void CMRT::ResToTargetBarrier()
{
	CMDLIST->ResourceBarrier(m_iRTCount, m_ResToTarget);
}

void CMRT::Clear()
{
	ResToTargetBarrier();
	UINT iRTVSize = CRenderMgr::GetInst()->GetRTVHeapSize();
	for (UINT i = 0; i < m_iRTCount; ++i) {
		D3D12_CPU_DESCRIPTOR_HANDLE hRTVHandle = m_pRTV->GetCPUDescriptorHandleForHeapStart();
		hRTVHandle.ptr += iRTVSize * i;
		float arrClearColor[4] = { m_arrRT[i].vClearColor.x,m_arrRT[i].vClearColor.y,m_arrRT[i].vClearColor.z,m_arrRT[i].vClearColor.w };
		CMDLIST->ClearRenderTargetView(hRTVHandle, arrClearColor, 0, nullptr);
	}
	if (nullptr != m_pDSTex) {
		D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = m_pDSTex->GetDSV()->GetCPUDescriptorHandleForHeapStart();
		CMDLIST->ClearDepthStencilView(hDSVHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
	}
}

void CMRT::Clear(UINT _iRTIdx)
{
	UINT iRTVSize = CRenderMgr::GetInst()->GetRTVHeapSize();
	D3D12_CPU_DESCRIPTOR_HANDLE hRTVHandle = m_pRTV->GetCPUDescriptorHandleForHeapStart();

	hRTVHandle.ptr += iRTVSize * _iRTIdx;
	float arrClearColor[4] = { m_arrRT[_iRTIdx].vClearColor.x,m_arrRT[_iRTIdx].vClearColor.y,m_arrRT[_iRTIdx].vClearColor.z,m_arrRT[_iRTIdx].vClearColor.w };

	CMDLIST->ClearRenderTargetView(hRTVHandle, arrClearColor, 0, nullptr);
	if (nullptr != m_pDSTex)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = m_pDSTex->GetDSV()->GetCPUDescriptorHandleForHeapStart();
		CMDLIST->ClearDepthStencilView(hDSVHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
	}
}

CMRT::CMRT() :m_arrRT{}, m_iRTCount(), m_ResToTarget()
{
}

CMRT::~CMRT()
{
}