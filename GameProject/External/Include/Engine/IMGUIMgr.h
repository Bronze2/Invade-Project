#pragma once

class CTexture;
class CIMGUIMgr
{

	SINGLE(CIMGUIMgr);

private:
	int                   HistoryPos;
	ID3D12DescriptorHeap* m_pHeap;
	bool doOnce = false;
	bool show_login = true;
	bool m_bOpen = false;
	bool m_MakeRoomOpen = false;

	ImVector<char*>       Items;
	ImVector<char*>       History;
	tResolution m_tResloution;
	char buffer[255];
	char chatItems[255];
	ImVector<const char*> Commands;
	D3D12_GPU_DESCRIPTOR_HANDLE GPU_Handle;
	UINT Image_Width;
	UINT Image_Height;
	CTexture* m_pTexture;
	CTexture* m_pTextureLobby;
	D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle;
	char RoomName[255];


public:
	 int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
	 char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		// FIXME-OPT
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		Items.push_back(Strdup(buf));
	}

	
	int TextEditCallback(ImGuiInputTextCallbackData* data);
	void SetInit() { doOnce = false; show_login = true; m_bOpen = false; }
	void Init();
	void Progress();
	void load_styles();
};

