#ifndef _TRAIL
#define _TRAIL
#include "value.fx"
#include "func.fx"

struct VTX_TRAIL_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR;
};

struct VTX_TRAIL_OUT
{
    float4 vPos : SV_POSITION;
    float2 vUV : TEXCOORD;
    float4 vColor : COLOR;
};

// ==========================
// Trail Shader
// Blend State : AlphaBlend
// DepthStencil State : No_DepthWrite (±Ì¿Ã≈◊Ω∫∆Æ 0, ±Ì¿Ã ±‚∑œ x)
// MRT : SwapchainMRT
//
// g_float_0 : Max Width
// g_float_1 : Min Width
// g_vec4_0 : color
// g_tex_0 : Trail Texture
// ==========================


VTX_TRAIL_OUT VS_Trail(VTX_TRAIL_IN _in)
{
    VTX_TRAIL_OUT output = (VTX_TRAIL_OUT)0.f;

    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vColor = _in.vColor;
    //output.iInstID = _in.iID;
    return output;
}

float4 PS_Trail(VTX_TRAIL_OUT _in) : SV_Target
{
    float4 vResult;
    if (tex_0)
        vResult = g_tex_0.Sample(g_sam_0, _in.vUV) * _in.vColor;
    else
        vResult = _in.vColor;
    return vResult;
}

#endif