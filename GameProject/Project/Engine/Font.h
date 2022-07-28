#pragma once
#include "Resource.h"
#include "Ptr.h"
#include "Material.h"

class CFont
	: public CResource
{
private:
	wstring m_name;				// 폰트 이름
	wstring m_fontImage;
	int m_iSize;					// 폰트 사이즈
	float m_fLineHeight;
	float m_fBaseHeight;
	int m_iTextureWidth;
	int m_iTextureHeight;
	int m_iNumCharacters;
	tFontChar* m_tCharList;
	int m_iNumKernings;
	tFontKerning* m_tKerningsList;

	float m_fLeftPadding;
	float m_fTopPadding;
	float m_fRightPadding;
	float m_fBottomPadding;

	int m_iMaxNumTextCharacters;			// 프레임당 그릴 최대 글자 수
	ComPtr<ID3D12Resource> m_pTextBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_tTextVtxView;
	UINT8* m_TextVBGPUAddress;
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandle; // the font srv

	Ptr<CMaterial> m_pMtrl;
	Ptr<CTexture> m_pTex;

	UINT srvHandleSize;

	ComPtr<ID3D12Resource> textVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW textVertexBufferView; // a view for our text vertex buffer
	UINT8* textVBGPUAddress; // this is a pointer to each of the text constant buffers


public:
	CFont();
	virtual ~CFont();

	float GetKerning(wchar_t first, wchar_t second);
	tFontChar* GetChar(wchar_t c);

	void LoadFont(LPCWSTR filename);
	void Render(wstring text, Vec2 vPos, Vec2 vScale, Vec2 vPadding, Vec4 vColor);

	void CreateVB();
	void CreateSrv();

	void SetMaterial(Ptr<CMaterial> _pMtrl) { m_pMtrl = _pMtrl; }
	Ptr<CMaterial> GetMaterial() { return m_pMtrl; }
	void SetTexture(Ptr<CTexture> _pTex) { m_pTex = _pTex; }
	Ptr<CTexture> GetTexture() { return m_pTex; }

	virtual void Load(const wstring& _strFullPath);
	virtual void Save(const wstring& _strPath);
};

