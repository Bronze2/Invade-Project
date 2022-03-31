#pragma once
#include "Resource.h"

struct tShaderParam
{
    wstring			strParamName;
    SHADER_PARAM	eType;
};

class CShader :
    public CResource
{
private:
    ComPtr<ID3DBlob> m_pVSBlob;

    ComPtr<ID3DBlob> m_pHSBlob;
    ComPtr<ID3DBlob> m_pDSBlob;
    ComPtr<ID3DBlob> m_pGSBlob;
    ComPtr<ID3DBlob> m_pPSBlob;
    ComPtr<ID3DBlob> m_pErrBlob;
    ComPtr<ID3DBlob> m_pCSBlob;
    ComPtr<ID3D12PipelineState> m_pPipeLineState;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC m_tPipeLine;

    ComPtr<ID3D12PipelineState> m_pPipeLineState_CS;
    D3D12_COMPUTE_PIPELINE_STATE_DESC m_tCSStateDesc;
    


    D3D_PRIMITIVE_TOPOLOGY				m_eTopology;
    vector<tShaderParam>				m_vecShaderPamam;

    RS_TYPE								m_eRSType;
    BLEND_TYPE							m_eBlendType;
    DEPTH_STENCIL_TYPE					m_eDSType;

    SHADER_POV m_ePov;

public:
    void Create(SHADER_POV _ePov,D3D_PRIMITIVE_TOPOLOGY _eTopology=D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    void CreateVertexShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);
 
    void CreatePixelShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);
    void CreateGeometryShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);
    void CreateComputeShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);
    void CreateHullShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);
    void CreateDomainShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);


    void SetBlendState(BLEND_TYPE _eType) { m_eBlendType = _eType; }
    void SetDepthStencilType(DEPTH_STENCIL_TYPE _eType) { m_eDSType = _eType; }
    void SetRasterizerType(RS_TYPE _eType) { m_eRSType = _eType; }

    void AddShaderParam(const tShaderParam& _param) { m_vecShaderPamam.push_back(_param); }
    void UpdateData();
    void UpdateData_CS();

    bool IsPossibleInstancing() { return m_pPipeLineState == nullptr ? false : true; }
    SHADER_POV GetShaderPov() { return m_ePov; }


    CShader();
    virtual ~CShader();
};

