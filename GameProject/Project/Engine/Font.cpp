#include "pch.h"
#include "Font.h"
#include "RenderMgr.h"
#include "ResMgr.h"
#include "Texture.h"
#include "Device.h"

void CFont::Load(const wstring& _strFullPath)
{
    LoadFont(_strFullPath.c_str());
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

void CFont::CreateVB()
{
    // 정점버퍼 만들기

    /*
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

        D3D12_RANGE readRange{ 0, 0 };
        m_pTextVB[i]->Map(0, &readRange, reinterpret_cast<void**>(&m_TextVBGPUAddress[i]));
    }

    for (int i = 0; i < 3; ++i) {
        m_tTextVtxView[i].BufferLocation = m_pTextVB[i]->GetGPUVirtualAddress();
        m_tTextVtxView[i].StrideInBytes = sizeof(TextVTX);
        m_tTextVtxView[i].SizeInBytes = m_iMaxNumTextCharacters * sizeof(TextVTX);
    }
    */

    /*
    D3D12_RESOURCE_DESC fontTextureDesc;
    int fontImageBytesPerRow;
    BYTE* fontImageData;
    int fontImageSize = LoadImageDataFromFile(&fontImageData, fontTextureDesc, m_fontImage.c_str(), fontImageBytesPerRow);

    // create the font texture resource
    DEVICE->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &fontTextureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_pTextBuffer));
   
    m_pTextBuffer->SetName(L"Font Texture Buffer Resource Heap");

    ComPtr<ID3D12Resource> fontTextureBufferUploadHeap;
    UINT64 fontTextureUploadBufferSize;
    DEVICE->GetCopyableFootprints(&fontTextureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &fontTextureUploadBufferSize);

    // create an upload heap to copy the texture to the gpu
    DEVICE->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE, // no flags
        &CD3DX12_RESOURCE_DESC::Buffer(fontTextureUploadBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&fontTextureBufferUploadHeap));
   
    fontTextureBufferUploadHeap->SetName(L"Font Texture Buffer Upload Resource Heap");

    // store font image in upload heap
    D3D12_SUBRESOURCE_DATA fontTextureData = {};
    fontTextureData.pData = &fontImageData[0]; // pointer to our image data
    fontTextureData.RowPitch = fontImageBytesPerRow; // size of all our triangle vertex data
    fontTextureData.SlicePitch = fontImageBytesPerRow * fontTextureDesc.Height; // also the size of our triangle vertex data

    // Now we copy the upload buffer contents to the default heap
    UpdateSubresources(CMDLIST_RES.Get(), m_pTextBuffer.Get(), fontTextureBufferUploadHeap.Get(), 0, 0, 1, &fontTextureData);


    // transition the texture default heap to a pixel shader resource (we will be sampling from this heap in the pixel shader to get the color of pixels)
    CMDLIST->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pTextBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

    // create an srv for the font
    D3D12_SHADER_RESOURCE_VIEW_DESC fontsrvDesc = {};
    fontsrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    fontsrvDesc.Format = fontTextureDesc.Format;
    fontsrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    fontsrvDesc.Texture2D.MipLevels = 1;

    // we need to get the next descriptor location in the descriptor heap to store this srv
    srvHandleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    srvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DEVICE->Get->GetGPUDescriptorHandleForHeapStart(), 1, srvHandleSize);

    CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(mainDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 1, srvHandleSize);
    DEVICE->CreateShaderResourceView(m_pTextBuffer.Get(), &fontsrvDesc, srvHandle);

    // create text vertex buffer committed resources

    for (int i = 0; i < 2; ++i)
    {
        // create upload heap. We will fill this with data for our text
        ID3D12Resource* vBufferUploadHeap;
        DEVICE->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
            D3D12_HEAP_FLAG_NONE, // no flags
            &CD3DX12_RESOURCE_DESC::Buffer(m_iMaxNumTextCharacters * sizeof(TextVTX)), // resource description for a buffer
            D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
            nullptr,
            IID_PPV_ARGS(&textVertexBuffer[i]));
        
        textVertexBuffer[i]->SetName(L"Text Vertex Buffer Upload Resource Heap");

        CD3DX12_RANGE readRange(0, 0);    // We do not intend to read from this resource on the CPU. (so end is less than or equal to begin)

        // map the resource heap to get a gpu virtual address to the beginning of the heap
        textVertexBuffer[i]->Map(0, &readRange, reinterpret_cast<void**>(&m_TextVBGPUAddress[i]));
    }
    */

    ID3D12Resource* vBufferUploadHeap;
    CD3DX12_HEAP_PROPERTIES value = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC value2 = CD3DX12_RESOURCE_DESC::Buffer(m_iMaxNumTextCharacters * sizeof(TextVTX));
    DEVICE->CreateCommittedResource(
        &value, // upload heap
        D3D12_HEAP_FLAG_NONE, // no flags
        &value2, // resource description for a buffer
        D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
        nullptr,
        IID_PPV_ARGS(&textVertexBuffer));

   // textVertexBuffer->SetName(L"Text Vertex Buffer Upload Resource Heap");

    D3D12_RANGE readRange{ 0, 0 };
    textVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_TextVBGPUAddress));

    m_tTextVtxView.BufferLocation = textVertexBuffer->GetGPUVirtualAddress();
    m_tTextVtxView.StrideInBytes = sizeof(TextVTX);
    m_tTextVtxView.SizeInBytes = m_iMaxNumTextCharacters * sizeof(TextVTX);
}

void CFont::CreateSrv()
{
    //// Load the image from file
    //D3D12_RESOURCE_DESC fontTextureDesc;
    //int fontImageBytesPerRow;
    //BYTE* fontImageData;
    //int fontImageSize = LoadImageDataFromFile(&fontImageData, fontTextureDesc, arialFont.fontImage.c_str(), fontImageBytesPerRow);

    //// create the font texture resource
    //DEVICE->CreateCommittedResource(
    //    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
    //    D3D12_HEAP_FLAG_NONE,
    //    &fontTextureDesc,
    //    D3D12_RESOURCE_STATE_COPY_DEST,
    //    nullptr,
    //    IID_PPV_ARGS(&textureBuffer));
   

    //textureBuffer->SetName(L"Font Texture Buffer Resource Heap");

    //ID3D12Resource* fontTextureBufferUploadHeap;
    //UINT64 fontTextureUploadBufferSize;
    //DEVICE->GetCopyableFootprints(&fontTextureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &fontTextureUploadBufferSize);

    //// create an upload heap to copy the texture to the gpu
    //DEVICE->CreateCommittedResource(
    //    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
    //    D3D12_HEAP_FLAG_NONE, // no flags
    //    &CD3DX12_RESOURCE_DESC::Buffer(fontTextureUploadBufferSize),
    //    D3D12_RESOURCE_STATE_GENERIC_READ,
    //    nullptr,
    //    IID_PPV_ARGS(&fontTextureBufferUploadHeap));

    //fontTextureBufferUploadHeap->SetName(L"Font Texture Buffer Upload Resource Heap");

    //// store font image in upload heap
    //D3D12_SUBRESOURCE_DATA fontTextureData = {};
    //fontTextureData.pData = &fontImageData[0]; // pointer to our image data
    //fontTextureData.RowPitch = fontImageBytesPerRow; // size of all our triangle vertex data
    //fontTextureData.SlicePitch = fontImageBytesPerRow * fontTextureDesc.Height; // also the size of our triangle vertex data

    //// Now we copy the upload buffer contents to the default heap
    //UpdateSubresources(CMDLIST, arialFont.textureBuffer, fontTextureBufferUploadHeap, 0, 0, 1, &fontTextureData);

    //// transition the texture default heap to a pixel shader resource (we will be sampling from this heap in the pixel shader to get the color of pixels)
    //CMDLIST->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(arialFont.textureBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

    //// create an srv for the font
    //D3D12_SHADER_RESOURCE_VIEW_DESC fontsrvDesc = {};
    //fontsrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    //fontsrvDesc.Format = fontTextureDesc.Format;
    //fontsrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    //fontsrvDesc.Texture2D.MipLevels = 1;

    //// we need to get the next descriptor location in the descriptor heap to store this srv
    //srvHandleSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    //arialFont.srvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mainDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), 1, srvHandleSize);

    //CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(mainDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 1, srvHandleSize);
    //DEVICE->CreateShaderResourceView(arialFont.textureBuffer, &fontsrvDesc, srvHandle);
}

void CFont::Render(wstring text, Vec2 vPos, Vec2 vScale, Vec2 vPadding, Vec4 vColor)
{
    if (nullptr == m_pMtrl || nullptr == m_pMtrl->GetShader())
        return;

    m_pMtrl->UpdateData();

    CDevice::GetInst()->UpdateTable();

    CMDLIST->IASetVertexBuffers(0, 1, &m_tTextVtxView);

    //CMDLIST->SetGraphicsRootDescriptorTable(1, m_pTex->GetSRV()->GetGPUDescriptorHandleForHeapStart());

    // Update
    int numCharacters = 0;

    float topLeftScreenX = (vPos.x * 2.0f) - 1.0f;
    float topLeftScreenY = ((1.0f - vPos.y) * 2.0f) - 1.0f;

    float x = topLeftScreenX;
    float y = topLeftScreenY;

    float horrizontalPadding = (m_fLeftPadding + m_fRightPadding) * vPadding.x;
    float verticalPadding = (m_fTopPadding + m_fBottomPadding) * vPadding.y;

    // cast the gpu virtual address to a textvertex, so we can directly store our vertices there
    TextVTX* vert = (TextVTX*)m_TextVBGPUAddress;

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

    //for (int i = 0; i < m_iNumCharacters; ++i) {
    //    cout << vert[i].vPos.x << ", " << vert[i].vPos.y << ", " << vert[i].vPos.z << ", " << vert[i].vPos.w << endl;
    //}

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