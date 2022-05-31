#pragma once
class CIMGUIMgr
{

	SINGLE(CIMGUIMgr);

private:
	ID3D12DescriptorHeap* m_pHeap;
public:
	void Init();
	void Progress();

};

