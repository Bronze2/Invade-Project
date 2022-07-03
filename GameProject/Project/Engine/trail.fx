//#ifndef _TRAIL
//#define _TRAIL
//#include "value.fx"
//
//struct tTrail
//{
//	float3 vWorldPos;
//    int iAlive;
//};
//
//struct VTX_TRAIL_IN
//{
//    float3 vPos : POSITION;
//    float2 vUV : TEXCOORD;
//    uint iID : SV_InstanceID;
//};
//
//struct VTX_TRAIL_OUT
//{
//    float4 vViewPos : POSITION;
//    float2 vUV : TEXCOORD;
//    float iInstID : FOG;
//};
//
//// ==========================
//// Trail Shader
//// Blend State : AlphaBlend
//// DepthStencil State : No_DepthWrite (±Ì¿Ã≈◊Ω∫∆Æ 0, ±Ì¿Ã ±‚∑œ x)
//// MRT : SwapchainMRT
////
//// g_float_0 : Start Scale
//// g_float_1 : End Scale
////
//// g_tex_0 : Trail Texture
//// ==========================
//
//StructuredBuffer<tTrail> tData : register(t13);
//
//VTX_TRAIL_OUT VS_Trail(VTX_TRAIL_IN _in)
//{
//    VTX_TRAIL_OUT output = (VTX_TRAIL_OUT)0.f;
//    float3 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
//    vWorldPos += tData[_in.iID].vWorldPos;
//    output.vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
//    output.vUV = _in.vUV;
//    output.iInstID = _in.iID;
//    return output;
//}
//
//struct GS_TRAIL_OUT
//{
//    float4 vPosition : SV_Position;
//    float2 vUV : TEXCOORD;
//    uint iInstID : SV_InstanceID;
//};
//
//[maxvertexcount(6)]
//void GS_Trail(point VTX_TRAIL_OUT _in[1], inout TriangleStream<GS_TRAIL_OUT> OutputStream)
//{
//    GS_TRAIL_OUT output[4] =
//    {
//        (GS_TRAIL_OUT)0.f, (GS_TRAIL_OUT)0.f, (GS_TRAIL_OUT)0.f, (GS_TRAIL_OUT)0.f
//    };
//    uint iInstID = (uint) _in[0].iInstID;
//    if (0 == tData[iInstID].iAlive)
//        return;
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
//
//float4 PS_Trail(GS_TRAIL_OUT _in) : SV_Target
//{
//    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
//    return vColor;
//}
//
//#endif