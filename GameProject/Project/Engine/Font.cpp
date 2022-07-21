#include "pch.h"
#include "Font.h"
#include "RenderMgr.h"
#include "ResMgr.h"
#include "Texture.h"
#include "Device.h"

void CFont::Load(const wstring& _strFullPath)
{
    LoadFont(_strFullPath.c_str());
    cout << "Font Load Complete!" << endl;

    CResMgr::GetInst()->Load<CTexture>(L"FontTex", m_fontImage);
    cout << "FontTex Load Complete!" << endl;

    // 정점버퍼 만들기

    for (int i = 0; i < 3; ++i) {
        ID3D12Resource* vBufferUploadHeap;
        CD3DX12_HEAP_PROPERTIES value = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC value2 = CD3DX12_RESOURCE_DESC::Buffer(m_iMaxNumTextCharacters * sizeof(TextVTX));
        DEVICE->CreateCommittedResource(
            &value, // upload heap
            D3D12_HEAP_FLAG_NONE, // no flags
            &value2, // resource description for a buffer
            D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
            nullptr,
            IID_PPV_ARGS(&m_pTextVB[i]));

        m_pTextVB[i]->SetName(L"Text Vertex Buffer Upload Resource Heap");

        UINT8* pVertexDataBegin = nullptr;
        D3D12_RANGE readRange{ 0, 0 };
        m_pTextVB[i]->Map(0, &readRange, reinterpret_cast<void**>(&m_TextVBGPUAddress[i]));
    }

    for (int i = 0; i < 3; ++i) {
        m_tTextVtxView[i].BufferLocation = m_pTextVB[i]->GetGPUVirtualAddress();
        m_tTextVtxView[i].StrideInBytes = sizeof(TextVTX);
        m_tTextVtxView[i].SizeInBytes = m_iMaxNumTextCharacters * sizeof(TextVTX);
    }
}

void CFont::Save(const wstring& _strPath)
{

}

void CFont::LoadFont(LPCWSTR filename)
{
    wifstream fs;
    fs.open(filename);

    std::wstring tmp;
    int startpos;
    tResolution res = CRenderMgr::GetInst()->GetResolution();

    // extract font name
    fs >> tmp >> tmp; // info face="Arial"
    startpos = tmp.find(L"'") + 1;
    m_name = tmp.substr(startpos, tmp.size() - startpos - 1);

    // get font size
    fs >> tmp; // size=73
    startpos = tmp.find(L"=") + 1;
    m_iSize = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

    // bold, italic, charset, unicode, stretchH, smooth, aa, padding, spacing
    fs >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp; // bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 

    // get padding
    fs >> tmp; // padding=5,5,5,5 
    startpos = tmp.find(L"=") + 1;
    tmp = tmp.substr(startpos, tmp.size() - startpos); // 5,5,5,5

    // get up padding
    startpos = tmp.find(L",") + 1;
    m_fTopPadding = std::stoi(tmp.substr(0, startpos)) / (float)res.fWidth;

    // get right padding
    tmp = tmp.substr(startpos, tmp.size() - startpos);
    startpos = tmp.find(L",") + 1;
    m_fRightPadding = std::stoi(tmp.substr(0, startpos)) / (float)res.fWidth;

    // get down padding
    tmp = tmp.substr(startpos, tmp.size() - startpos);
    startpos = tmp.find(L",") + 1;
    m_fBottomPadding = std::stoi(tmp.substr(0, startpos)) / (float)res.fWidth;

    // get left padding
    tmp = tmp.substr(startpos, tmp.size() - startpos);
    m_fLeftPadding = std::stoi(tmp) / (float)res.fWidth;

    fs >> tmp >> tmp; // spacing=0,0 outline=0

    // get lineheight (how much to move down for each line), and normalize (between 0.0 and 1.0 based on size of font)
    fs >> tmp >> tmp; // common lineHeight=95
    startpos = tmp.find(L"=") + 1;
    m_fLineHeight = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)res.fHeight;

    // get base height (height of all characters), and normalize (between 0.0 and 1.0 based on size of font)
    fs >> tmp; // base=68
    startpos = tmp.find(L"=") + 1;
    m_fBaseHeight = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)res.fHeight;

    // get texture width
    fs >> tmp; // scaleW=512
    startpos = tmp.find(L"=") + 1;
    m_iTextureWidth = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

    // get texture height
    fs >> tmp; // scaleH=512
    startpos = tmp.find(L"=") + 1;
    m_iTextureHeight = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

    // get pages, packed, page id
    fs >> tmp >> tmp; // pages=1 packed=0
    fs >> tmp >> tmp >> tmp >> tmp;     // alphaChnl=1 redChnl=0 greenChnl=0 blueChnl=0
    fs >> tmp >> tmp; // page id=0

    // get texture filename
    std::wstring wtmp;
    fs >> wtmp; // file="Arial.png"
    startpos = wtmp.find(L"'") + 1;
    m_fontImage = wtmp.substr(startpos, wtmp.size() - startpos - 1);

    // get number of characters
    fs >> tmp >> tmp; // chars count=97
    startpos = tmp.find(L"=") + 1;
    m_iNumCharacters = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

    // initialize the character list
    m_tCharList = new tFontChar[m_iNumCharacters];

    for (int c = 0; c < m_iNumCharacters; ++c)
    {
        // get unicode id
        fs >> tmp >> tmp; // char id=0
        startpos = tmp.find(L"=") + 1;
        m_tCharList[c].iId = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

        // get x
        fs >> tmp; // x=392
        startpos = tmp.find(L"=") + 1;
        m_tCharList[c].fU = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)m_iTextureWidth;

        // get y
        fs >> tmp; // y=340
        startpos = tmp.find(L"=") + 1;
        m_tCharList[c].fV = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)m_iTextureHeight;

        // get width
        fs >> tmp; // width=47
        startpos = tmp.find(L"=") + 1;
        tmp = tmp.substr(startpos, tmp.size() - startpos);
        m_tCharList[c].fWidth = (float)std::stoi(tmp) / (float)res.fWidth;
        m_tCharList[c].ftWidth = (float)std::stoi(tmp) / (float)m_iTextureWidth;

        // get height
        fs >> tmp; // height=57
        startpos = tmp.find(L"=") + 1;
        tmp = tmp.substr(startpos, tmp.size() - startpos);
        m_tCharList[c].fHeight = (float)std::stoi(tmp) / (float)res.fHeight;
        m_tCharList[c].ftHeight = (float)std::stoi(tmp) / (float)m_iTextureHeight;

        // get xoffset
        fs >> tmp; // xoffset=-6
        startpos = tmp.find(L"=") + 1;
        m_tCharList[c].fxOffset = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)res.fWidth;

        // get yoffset
        fs >> tmp; // yoffset=16
        startpos = tmp.find(L"=") + 1;
        m_tCharList[c].fyOffset = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)res.fHeight;

        // get xadvance
        fs >> tmp; // xadvance=65
        startpos = tmp.find(L"=") + 1;
        m_tCharList[c].fxAdvance = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos)) / (float)res.fWidth;

        // get page
        // get channel
        fs >> tmp >> tmp; // page=0    chnl=0
    }

    // get number of kernings
    fs >> tmp >> tmp; // kernings count=96
    startpos = tmp.find(L"=") + 1;
    m_iNumKernings = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

    // initialize the kernings list
    m_tKerningsList = new tFontKerning[m_iNumKernings];

    for (int k = 0; k < m_iNumKernings; ++k)
    {
        // get first character
        fs >> tmp >> tmp; // kerning first=87
        startpos = tmp.find(L"=") + 1;
        m_tKerningsList[k].iFirstID = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

        // get second character
        fs >> tmp; // second=45
        startpos = tmp.find(L"=") + 1;
        m_tKerningsList[k].iSecondID = std::stoi(tmp.substr(startpos, tmp.size() - startpos));

        // get amount
        fs >> tmp; // amount=-1
        startpos = tmp.find(L"=") + 1;
        int t = (float)std::stoi(tmp.substr(startpos, tmp.size() - startpos));
        m_tKerningsList[k].fxAmount = (float)t / (float)res.fWidth;
    }
}

void CFont::Render(wstring text, Vec3 vPos, Vec3 vScale, Vec2 vPadding, Vec4 vColor)
{
    if (nullptr == m_pMtrl || nullptr == m_pMtrl->GetShader())
        return;

    m_pMtrl->UpdateData();

    CDevice::GetInst()->UpdateTable();

    CMDLIST->IASetVertexBuffers(0, 1, &m_tTextVtxView[0]);

    // Update
    int numCharacters = 0;

    float topLeftScreenX = (vPos.x * 2.0f) - 1.0f;
    float topLeftScreenY = ((1.0f - vPos.y) * 2.0f) - 1.0f;

    float x = topLeftScreenX;
    float y = topLeftScreenY;

    float horrizontalPadding = (m_fLeftPadding + m_fRightPadding) * vPadding.x;
    float verticalPadding = (m_fTopPadding + m_fBottomPadding) * vPadding.y;

    // cast the gpu virtual address to a textvertex, so we can directly store our vertices there
    TextVTX* vert = (TextVTX*)m_TextVBGPUAddress[0];

    wchar_t lastChar = -1; // no last character to start with

    for (int i = 0; i < text.size(); ++i)
    {
        wchar_t c = text[i];

        tFontChar* fc = GetChar(c);

        // character not in font char set
        if (fc == nullptr)
            continue;

        // end of string
        if (c == L' ')
            break;

        // new line
        if (c == L'n')
        {
            x = topLeftScreenX;
            y -= (m_fLineHeight + verticalPadding) * vScale.y;
            continue;
        }

        // don't overflow the buffer. In your app if this is true, you can implement a resize of your text vertex buffer
        if (numCharacters >= m_iMaxNumTextCharacters)
            break;

        float kerning = 0.0f;
        if (i > 0)
            kerning = GetKerning(lastChar, c);

        vert[numCharacters] = TextVTX(vColor.x, vColor.y, vColor.z, vColor.w,
            fc->fU, fc->fV, fc->ftWidth, fc->ftHeight,
            x + ((fc->fxOffset + kerning) * vScale.x), y - (fc->fyOffset * vScale.y), fc->fWidth * vScale.x, fc->fHeight * vScale.y);

        numCharacters++;

        // remove horrizontal padding and advance to next char position
        x += (fc->fxAdvance - horrizontalPadding) * vScale.x;

        lastChar = c;
    }

    CMDLIST->DrawInstanced(4, m_iNumCharacters, 0, 0);
}

float CFont::GetKerning(wchar_t first, wchar_t second)
{
    for (int i = 0; i < m_iNumKernings; ++i)
    {
        if ((wchar_t)m_tKerningsList[i].iFirstID == first && (wchar_t)m_tKerningsList[i].iSecondID == second)
            return m_tKerningsList[i].fxAmount;
    }
    return 0.0f;
}

tFontChar* CFont::GetChar(wchar_t c)
{
    for (int i = 0; i < m_iNumCharacters; ++i)
    {
        if (c == (wchar_t)m_tCharList[i].iId)
            return &m_tCharList[i];
    }
    cout << "Font : GetChar return nullptr" << endl;
    return nullptr;
}

CFont::CFont() : CResource(RES_TYPE::FONT)
, m_iMaxNumTextCharacters(1024)
{

}

CFont::~CFont()
{

}