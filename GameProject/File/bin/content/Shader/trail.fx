#ifndef _TRAIL
#define _TRAIL
#include "value.fx"

struct VTX_TRAIL_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_TRAIL_OUT
{
    float4 vViewPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct GS_TRAIL_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// ==========================
// Trail Shader
// Blend State : AlphaBlend
// DepthStencil State : No_DepthWrite (±Ì¿Ã≈◊Ω∫∆Æ 0, ±Ì¿Ã ±‚∑œ x)
// MRT : SwapchainMRT
//
// g_float_0 : width
// g_float_1 : height
// g_vec4_0 : color
// g_tex_0 : Trail Texture
// ==========================


VTX_TRAIL_OUT VS_Trail(VTX_TRAIL_IN _in)
{
    VTX_TRAIL_OUT output = (VTX_TRAIL_OUT)0.f;
    float3 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    output.vUV = _in.vUV;
    return output;
}

[maxvertexcount(6)]
void GS_Trail(point VTX_TRAIL_OUT _in[1], inout TriangleStream<GS_TRAIL_OUT> OutputStream)
{
    // 0 -- 1
    // | in |
    // 3 -- 2

    GS_TRAIL_OUT output[4] =
    {
        (GS_TRAIL_OUT)0.f, (GS_TRAIL_OUT)0.f, (GS_TRAIL_OUT)0.f, (GS_TRAIL_OUT)0.f
    };

    float fScaleX = g_float_0 * 0.5f;
    float fScaleY = g_float_1 * 0.5f;

    output[0].vPosition = _in[0].vViewPos + float4(-fScaleX, fScaleY, 0.f, 0.f);
    output[1].vPosition = _in[0].vViewPos + float4(fScaleX, fScaleY, 0.f, 0.f);
    output[2].vPosition = _in[0].vViewPos + float4(fScaleX, -fScaleY, 0.f, 0.f);
    output[3].vPosition = _in[0].vViewPos + float4(-fScaleX, -fScaleY, 0.f, 0.f);

    output[0].vPosition = mul(output[0].vPosition, g_matProj);
    output[1].vPosition = mul(output[1].vPosition, g_matProj);
    output[2].vPosition = mul(output[2].vPosition, g_matProj);
    output[3].vPosition = mul(output[3].vPosition, g_matProj);

    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);

    OutputStream.Append(output[0]);
    OutputStream.Append(output[1]);
    OutputStream.Append(output[2]);
    OutputStream.RestartStrip();
    OutputStream.Append(output[0]);
    OutputStream.Append(output[2]);
    OutputStream.Append(output[3]);
    OutputStream.RestartStrip();
}

float4 PS_Trail(GS_TRAIL_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV) * g_vec4_0;
    return vColor;
}

#endif