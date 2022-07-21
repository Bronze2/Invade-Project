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
	ComPtr<ID3D12Resource> m_pTextVB[3];
	D3D12_VERTEX_BUFFER_VIEW m_tTextVtxView[3];
	UINT8* m_TextVBGPUAddress[3];

	Ptr<CMaterial> m_pMtrl;

public:
	CFont();
	virtual ~CFont();

	float GetKerning(wchar_t first, wchar_t second);
	tFontChar* GetChar(wchar_t c);

	void LoadFont(LPCWSTR filename);
	void Render(wstring text, Vec3 vPos, Vec3 vScale, Vec2 vPadding, Vec4 vColor);

	void SetMaterial(Ptr<CMaterial> _pMtrl) { m_pMtrl = _pMtrl; }
	Ptr<CMaterial> GetMaterial() { return m_pMtrl; }

	virtual void Load(const wstring& _strFullPath);
	virtual void Save(const wstring& _strPath);
};

