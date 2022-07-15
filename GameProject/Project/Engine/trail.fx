#ifndef _TRAIL
#define _TRAIL
#include "value.fx"
#include "func.fx"

//struct tTrail
//{
//    float3 vWorldDir;
//    float3 vWorldPos;
//    float fCurTime;
//    float fLifeTime;
//    int iAlive;
//};

//struct tTrailShared
//{
//    int iAddCount;
//    int iCurCount;
//    float fRange;
//};

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

//struct GS_TRAIL_OUT
//{
//    float4 vPosition : SV_Position;
//    float2 vUV : TEXCOORD;
//};

//StructuredBuffer<tTrail> tData: register(t13);
//RWStructuredBuffer<tTrail> tRWData : register(u0);
//RWStructuredBuffer<tTrailShared> tRWSharedData : register(u1);

// ==========================
// Trail Shader
// Blend State : AlphaBlend
// DepthStencil State : No_DepthWrite (깊이테스트 0, 깊이 기록 x)
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
    float3 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    output.vUV = _in.vUV;
    //output.iInstID = _in.iID;
    return output;
}

//[maxvertexcount(6)]
//void GS_Trail(point VTX_TRAIL_OUT _in[1], inout TriangleStream<GS_TRAIL_OUT> OutputStream)
//{
//    // 0 -- 1
//    // | in |
//    // 3 -- 2
//
//    GS_TRAIL_OUT output[4] =
//    {
//        (GS_TRAIL_OUT)0.f, (GS_TRAIL_OUT)0.f, (GS_TRAIL_OUT)0.f, (GS_TRAIL_OUT)0.f
//    };
//
//    uint iInstID = (uint) _in[0].iInstID;
//    if (0 == tData[iInstID].iAlive)
//        return;
//    float fRatio = tData[iInstID].fCurTime / tData[iInstID].fLifeTime;
//    float fCurScale = ((g_float_1 - g_float_0) * fRatio + g_float_0) / 2.f;
//
//    output[0].vPosition = _in[0].vViewPos + float4(-fCurScale, fCurScale, 0.f, 0.f);
//    output[1].vPosition = _in[0].vViewPos + float4(fCurScale, fCurScale, 0.f, 0.f);
//    output[2].vPosition = _in[0].vViewPos + float4(fCurScale, -fCurScale, 0.f, 0.f);
//    output[3].vPosition = _in[0].vViewPos + float4(-fCurScale, -fCurScale, 0.f, 0.f);
//
//    output[0].vPosition = mul(output[0].vPosition, g_matProj);
//    output[1].vPosition = mul(output[1].vPosition, g_matProj);
//    output[2].vPosition = mul(output[2].vPosition, g_matProj);
//    output[3].vPosition = mul(output[3].vPosition, g_matProj);
//
//    output[0].vUV = float2(0.f, 0.f);
//    output[1].vUV = float2(1.f, 0.f);
//    output[2].vUV = float2(1.f, 1.f);
//    output[3].vUV = float2(0.f, 1.f);
//
//    output[0].iInstID = iInstID;
//    output[1].iInstID = iInstID;
//    output[2].iInstID = iInstID;
//    output[3].iInstID = iInstID;
//
//    OutputStream.Append(output[0]);
//    OutputStream.Append(output[1]);
//    OutputStream.Append(output[2]);
//    OutputStream.RestartStrip();
//    OutputStream.Append(output[0]);
//    OutputStream.Append(output[2]);
//    OutputStream.Append(output[3]);
//    OutputStream.RestartStrip();
//}

float4 PS_Trail(VTX_TRAIL_OUT _in) : SV_Target
{
    float4 vColor;
    if (tex_0)
        vColor = g_tex_0.Sample(g_sam_0, _in.vUV) * g_vec4_0;
    else
        vColor = g_vec4_0;
    return vColor;
}

// ===============
// Trail Update

// g_int_0 : Particle Max Count

// g_float_0 : CurTime
// g_float_1 : EmitTime

// g_float_2 : speed

// g_vec4_0 : WorldPos
// ===============

//
//[numthreads(1024, 1, 1)]
//void CS_TrailUpdate(int3 _iThreadIdx : SV_DispatchThreadID)
//{
//    if (_iThreadIdx.x >= g_int_0)
//        return;
//
//    tRWSharedData[0].iAddCount = g_int_1;
//    if (0 == tRWData[_iThreadIdx.x].iAlive)
//    {
//        int iOrigin = tRWSharedData[0].iAddCount;
//        int iExChange = 0;
//        while (0 < iOrigin)
//        {
//            int iInputValue = iOrigin - 1;
//            // InterlockedExchange(tRWSharedData[0].iAddCount, iInputValue, iExChange);
//            InterlockedCompareExchange(tRWSharedData[0].iAddCount, iOrigin, iInputValue, iExChange); //개수만큼만 생성
//            if (iExChange == iOrigin)
//            {
//                tRWData[_iThreadIdx.x].iAlive = 1;
//                break;
//            }
//            iOrigin = iInputValue;
//        }
//        if (1 == tRWData[_iThreadIdx.x].iAlive)
//        {
//            float2 vUV = float2(((float)_iThreadIdx.x / (float)g_int_0) + g_fAccTime, g_fAccTime);
//            vUV.y += sin(vUV.x * 2 * 3.141592);
//            if (vUV.x > 0)
//                vUV.x = frac(vUV.x);
//            else
//                vUV.x = ceil(abs(vUV.x)) - abs(vUV.x);
//            if (vUV.y > 0)
//                vUV.y = frac(vUV.y);
//            else
//                vUV.y = ceil(abs(vUV.y)) - abs(vUV.y);
//            vUV = vUV * g_vec2_0;
//
//            float3 vNoise =
//            {
//                gaussian5x5Sample(vUV + int2(0, -100), g_tex_0)
//                , gaussian5x5Sample(vUV + int2(0, 0), g_tex_0),
//                gaussian5x5Sample(vUV + int2(0, 100), g_tex_0)
//            };
//            float g_PI = 3.141592f;
//            float3 vDir = (float4) 0.f;
//            vDir = (vNoise - 0.5f) * 2.f;
//            tRWData[_iThreadIdx.x].vWorldDir = normalize(vDir);
//            tRWData[_iThreadIdx.x].vWorldPos = (vNoise.xyz - 0.5f) * 50;
//            tRWData[_iThreadIdx.x].fLifeTime = ((g_float_1 - g_float_0) * vNoise.x) + g_float_0;
//            tRWData[_iThreadIdx.x].fCurTime = 0.f;
//        }
//    }
//    else
//    {
//        tRWData[_iThreadIdx.x].fCurTime += g_fDT;
//        float Time = tRWData[_iThreadIdx.x].fCurTime;
//        if (tRWData[_iThreadIdx.x].fLifeTime < tRWData[_iThreadIdx.x].fCurTime)
//        {
//            tRWData[_iThreadIdx.x].iAlive = 0;
//            return;
//        }
//        //  g_vec2_3;
//        float g_PI = 3.141592f;
//        float amp = 0.5f;
//        float fRatio = tRWData[_iThreadIdx.x].fCurTime / tRWData[_iThreadIdx.x].fLifeTime;
//        float fSpeed = g_float_2;
//
//        tRWData[_iThreadIdx.x].vWorldPos.x -= g_vec4_0.z * fSpeed * g_fDT;
//        tRWData[_iThreadIdx.x].vWorldPos.y = sin(Time * 8 * g_PI) * 5.f * g_vec2_3.x;
//        tRWData[_iThreadIdx.x].vWorldPos.z += g_vec4_0.x * fSpeed * g_fDT;
//
//
//        tRWSharedData[0].iCurCount = 0;
//        InterlockedAdd(tRWSharedData[0].iCurCount, 1);
//    }
//}


#endif