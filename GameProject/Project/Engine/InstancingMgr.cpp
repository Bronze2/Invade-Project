#include "pch.h"
#include "InstancingMgr.h"
CInstancingMgr::CInstancingMgr():m_iInstIdx(0) {

}
CInstancingMgr::~CInstancingMgr() {
    for (size_t i = 0; i < m_vecInstancingBuffer.size(); ++i) {
        SAFE_DELETE(m_vecInstancingBuffer[i]);
    }
}


void CInstancingMgr::Init()
{
    CInstancingBuffer* pInstBuffer = new CInstancingBuffer;
    pInstBuffer->Init();
    m_vecInstancingBuffer.push_back(pInstBuffer);
}

void CInstancingMgr::Clear()
{
    m_iInstIdx = 0;
    for (size_t i = 0; i < m_vecInstancingBuffer.size(); ++i) {
        m_vecInstancingBuffer[i]->Clear();
    }
}

void CInstancingMgr::AddInstancingData(tInstancingData& _tData, bool _bAnim)
{
    m_vecInstancingBuffer[m_iInstIdx]->AddInstancingData(_tData, _bAnim);
}

CInstancingBuffer* CInstancingMgr::GetInstancingBuffer(long long _lbufferID)
{
    for (auto& pBuffer : m_vecInstancingBuffer) {
        if (pBuffer->GetBufferID() == _lbufferID)
            return pBuffer;
    }
    return nullptr;
}

CInstancingBuffer* CInstancingMgr::AllocBuffer(long long _lbufferid)
{

    if (m_iInstIdx >= (UINT)m_vecInstancingBuffer.size()) {
        CInstancingBuffer* pInstBuffer = new CInstancingBuffer;
        pInstBuffer->Init();
        m_vecInstancingBuffer.push_back(pInstBuffer);
    }
    m_vecInstancingBuffer[m_iInstIdx]->SetBufferID(_lbufferid);
    return m_vecInstancingBuffer[m_iInstIdx];
}

void CInstancingMgr::SetData()
{
    m_vecInstancingBuffer[m_iInstIdx]->SetData();
    ++m_iInstIdx;
}
