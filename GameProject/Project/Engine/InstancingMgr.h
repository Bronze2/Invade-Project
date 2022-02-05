#pragma once
#include "InstancingBuffer.h"
class CInstancingMgr
{
	SINGLE(CInstancingMgr)
private:
	vector<CInstancingBuffer*>m_vecInstancingBuffer;
	UINT m_iInstIdx;
public:
	void Init();
	void Clear();
	UINT GetCurrentInstancingIdx() { return m_iInstIdx; }
	void AddInstancingData(tInstancingData& _tData, bool _bAnim);
	UINT GetInstanceCount(UINT _iInstancingIdx) {
		return m_vecInstancingBuffer[_iInstancingIdx]->GetInstanceCount();
	}
	CInstancingBuffer* GetInstancingBuffer(long long _lbufferID);
	CInstancingBuffer* AllocBuffer(long long _lbufferid);
	void SetData();
};

