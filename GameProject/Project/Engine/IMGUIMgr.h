#pragma once

class CTexture;
class CIMGUIMgr
{

	SINGLE(CIMGUIMgr);

private:
	ID3D12DescriptorHeap* m_pHeap;
	bool doOnce = false;
	bool show_login = true;
	bool m_bOpen = false;
	tResolution m_tResloution;
	char buffer[255];
	char chatItems[255];

	D3D12_GPU_DESCRIPTOR_HANDLE GPU_Handle;
	UINT Image_Width;
	UINT Image_Height;
	CTexture* m_pTexture;
public:
	void SetInit() { doOnce = false; show_login = true; m_bOpen = false; }
	void Init();
	void Progress();
	void load_styles();
};

