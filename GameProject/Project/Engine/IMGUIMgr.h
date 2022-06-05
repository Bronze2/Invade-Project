#pragma once
class CIMGUIMgr
{

	SINGLE(CIMGUIMgr);

private:
	ID3D12DescriptorHeap* m_pHeap;
	bool doOnce = false;
	bool show_login = true;
	bool m_bOpen = true;
public:
	void Init();
	void Progress();
	void load_styles();
};

