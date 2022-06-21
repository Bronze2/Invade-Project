#ifndef _TRAIL
#define _TRAIL
#include "value.fx"

struct tTrail
{
	float3 wWorldPos;
};

struct VTX_IN
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float2 vUV : TEXCOORD;
    uint iID : SV_InstanceID;
};

struct VTX_OUT
{
    float4 vViewPos : POSITION;
    float2 vUV : TEXCOORD;
    float iInstID : FOG;
};

// ==========================
// Trail Shader
// Blend State : AlphaBlend
// DepthStencil State : No_DepthWrite (±Ì¿Ã≈◊Ω∫∆Æ 0, ±Ì¿Ã ±‚∑œ x)
// MRT : SwapchainMRT
//
// g_tex_0 : Trail Texture
// ==========================




#endif