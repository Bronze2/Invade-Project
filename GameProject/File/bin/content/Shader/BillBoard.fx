#ifndef _BILLBOARD
#define _BILLBOARD
#include "value.fx"

struct tBillBoard
{
    float3 vWorldPos;
    float3 vWorldDir;
    float2 vPadding;
};


struct VTX_BILLBOARD_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint iID : SV_InstanceID;
};


struct VTX_BILLBOARD_OUT
{
    float4 vViewPos : POSITION;
    float2 vUV : TEXCOORD;
    float iInstID : FOG;
};
StructuredBuffer<tBillBoard> tData : register(t11);


VTX_BILLBOARD_OUT VS_BillBoard(VTX_BILLBOARD_IN _in)
{
    VTX_BILLBOARD_OUT output = (VTX_BILLBOARD_OUT) 0.f;
    float3 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    vWorldPos += tData[_in.iID].vWorldPos;
    output.vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    output.vUV = _in.vUV;
    output.iInstID = _in.iID;
    
    return output;
}

struct GS_BILLBOARD_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_BillBoard(point VTX_BILLBOARD_OUT _in[1], inout TriangleStream<GS_BILLBOARD_OUT> OutputStream)
{
    GS_BILLBOARD_OUT output[4] =
    {
        (GS_BILLBOARD_OUT) 0.f, (GS_BILLBOARD_OUT) 0.f, (GS_BILLBOARD_OUT) 0.f, (GS_BILLBOARD_OUT) 0.f
    };
    
    uint iInstID = (uint) _in[0].iInstID;
    float fCurScalex = ((g_float_1 - g_float_0)) * 0.5f;
    float fCurScaley = ((g_float_3 - g_float_2)) * 0.5f;
    output[0].vPosition = _in[0].vViewPos + float4(-fCurScalex, fCurScaley, 0.f, 0.f);
    output[1].vPosition = _in[0].vViewPos + float4(fCurScalex, fCurScaley, 0.f, 0.f);
    output[2].vPosition = _in[0].vViewPos + float4(fCurScalex, -fCurScaley, 0.f, 0.f);
    output[3].vPosition = _in[0].vViewPos + float4(-fCurScalex, -fCurScaley, 0.f, 0.f);
    
    output[0].vPosition = mul(output[0].vPosition, g_matProj);
    output[1].vPosition = mul(output[1].vPosition, g_matProj);
    output[2].vPosition = mul(output[2].vPosition, g_matProj);
    output[3].vPosition = mul(output[3].vPosition, g_matProj);
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    
    output[0].iInstID = iInstID;
    output[1].iInstID = iInstID;
    output[2].iInstID = iInstID;
    output[3].iInstID = iInstID;
    
    OutputStream.Append(output[0]);
    OutputStream.Append(output[1]);
    OutputStream.Append(output[2]);
    OutputStream.RestartStrip();
    OutputStream.Append(output[0]);
    OutputStream.Append(output[2]);
    OutputStream.Append(output[3]);
    OutputStream.RestartStrip();
    
}


float4 PS_BillBoard(GS_BILLBOARD_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    return vColor;
}


#endif
