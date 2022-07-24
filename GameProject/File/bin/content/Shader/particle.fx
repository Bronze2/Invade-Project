#ifndef _PARTICLE
#define _PARTICLE
#include "value.fx"
#include "func.fx"


struct tParticle
{
    float3 vWorldPos;
    float3 vWorldDir;
    float m_fCurTime;
    float m_fLifeTime;
    
    int iAlive;
    int arrPadding[3];
};


struct tParticleShared
{
    int iAddCount;
    int iCurCount;
    float fRange;
    int arrPadding;
};

StructuredBuffer<tParticle> tData: register(t10);
RWStructuredBuffer<tParticle> tRWData : register(u0);
RWStructuredBuffer<tParticleShared> tRWSharedData : register(u1);

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


// ===============
// Particle Shader
// Blend State : AlphaBlend
// DepthStencil State : No_DepthWrite (���� �׽�Ʈ 0, ���� ��� X)
// MRT : SwapChainMRT

// g_vec4_0 : Start Color
// g_Vec4_1 : End Color

// g_float_0 : Start Scale
// g_float_1 : End Scale

// g_tex_0 : Particle Texture
// ===============

VTX_OUT VS_Particle(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    float3 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    vWorldPos += tData[_in.iID].vWorldPos;
    output.vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    output.vUV = _in.vUV;
    output.iInstID = _in.iID;
    return output;
}
struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_Particle(point VTX_OUT _in[1], inout TriangleStream<GS_OUT> OutputStream)
{
    GS_OUT output[4] =
    {
        (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f
    };
    uint iInstID = (uint) _in[0].iInstID;
    if(0==tData[iInstID].iAlive)
        return;
    float fRatio = tData[iInstID].m_fCurTime / tData[iInstID].m_fLifeTime;
    float fCurScale = ((g_float_1 - g_float_0) * fRatio + g_float_0) / 2.f;
    
    float3 updown = float3(0, 1, 0);
    
    output[0].vPosition = _in[0].vViewPos + float4(-fCurScale, fCurScale, 0.f, 0.f);
    output[1].vPosition = _in[0].vViewPos + float4(fCurScale, fCurScale, 0.f, 0.f);
    output[2].vPosition = _in[0].vViewPos + float4(fCurScale, -fCurScale, 0.f, 0.f);
    output[3].vPosition = _in[0].vViewPos + float4(-fCurScale, -fCurScale, 0.f, 0.f);

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



float4 PS_Particle(GS_OUT _in) : SV_Target
{
    float fRatio = tData[_in.iInstID].m_fCurTime / tData[_in.iInstID].m_fLifeTime;
    float4 vCurColor = (g_vec4_1 - g_vec4_0) * fRatio + g_vec4_0;
    return vCurColor * g_tex_0.Sample(g_sam_0, _in.vUV);
}
// ===============
// Particle Update
// g_tex_0 : Noise Texture
// g_vec2_0 : NoiseTexture Resolution

// g_int_0 : Particle Max Count
// g_int_1 : AddCount

// g_float_0 : Min LifeTime
// g_float_1 : Max LifeTime

// g_float_2 : Min Speed
// g_float_3 : Max Speed
// ===============
[numthreads(1024,1,1)]
void CS_ParticleUpdate(int _iThreadIdx : SV_DispatchThreadID)
{
    if(_iThreadIdx.x>=g_int_0)
        return;
    tRWSharedData[0].iAddCount = g_int_1;
    if (0 == tRWData[_iThreadIdx.x].iAlive)
    {
        int iOrigin = tRWSharedData[0].iAddCount;
        int iExChange = 0;
        while (0 < iOrigin)
        {
            int iInputValue = iOrigin - 1;
           // InterlockedExchange(tRWSharedData[0].iAddCount, iInputValue, iExChange);
            InterlockedCompareExchange(tRWSharedData[0].iAddCount,iOrigin ,iInputValue, iExChange); //������ŭ�� ����
            if (iExChange == iOrigin)
            {
                tRWData[_iThreadIdx.x].iAlive = 1;
                break;
            }
            iOrigin = iInputValue;
        }
        if (1 == tRWData[_iThreadIdx.x].iAlive)
        {
            float2 vUV = float2(((float) _iThreadIdx.x / (float) g_int_0) + g_fAccTime, g_fAccTime);
            vUV.y += sin(vUV.x * 2 * 3.141592);
            if(vUV.x>0)
                vUV.x = frac(vUV.x);
            else
                vUV.x = ceil(abs(vUV.x)) - abs(vUV.x);
            if(vUV.y>0)
                vUV.y = frac(vUV.y);
            else
                vUV.y = ceil(abs(vUV.y)) - abs(vUV.y);
            vUV = vUV * g_vec2_0;
            
            float3 vNoise =
            {
                gaussian5x5Sample(vUV + int2(0, -100), g_tex_0)
                , gaussian5x5Sample(vUV + int2(0, 0), g_tex_0),
                gaussian5x5Sample(vUV + int2(0, 100), g_tex_0)
            };
            float g_PI = 3.141592f;
            float3 vDir = (float4) 0.f;
            vDir = (vNoise - 0.5f) * 2.f;
            tRWData[_iThreadIdx.x].vWorldDir = normalize(vDir);
            tRWData[_iThreadIdx.x].vWorldPos = sin(0.f);
            tRWData[_iThreadIdx.x].m_fLifeTime = ((g_float_1 - g_float_0) * vNoise.x) + g_float_0;
            tRWData[_iThreadIdx.x].m_fCurTime = 0.f;
        }
    }
    else
    {
        tRWData[_iThreadIdx.x].m_fCurTime += g_fDT;
        float Time = tRWData[_iThreadIdx.x].m_fCurTime;
        if (tRWData[_iThreadIdx.x].m_fLifeTime < tRWData[_iThreadIdx.x].m_fCurTime)
        {
            tRWData[_iThreadIdx.x].iAlive = 0;
            return;
        }
      //  g_vec2_3;
        float g_PI = 3.141592f;
        float amp = 0.5f;
        float fRatio = tRWData[_iThreadIdx.x].m_fCurTime / tRWData[_iThreadIdx.x].m_fLifeTime;
        float fSpeed = (g_float_3 - g_float_2) * fRatio + g_float_2;
       
            tRWData[_iThreadIdx.x].vWorldPos.x -= g_vec4_0.z * fSpeed * g_fDT;
            tRWData[_iThreadIdx.x].vWorldPos.y = sin(Time * 8 * g_PI) * 5.f *g_vec2_3.x;
            tRWData[_iThreadIdx.x].vWorldPos.z -= g_vec4_0.x * fSpeed * g_fDT;
 
        
      
        tRWSharedData[0].iCurCount = 0;
        InterlockedAdd(tRWSharedData[0].iCurCount, 1);
    }
}
[numthreads(1024, 1, 1)]
void CS_ParticleUpdate2(int3 _iThreadIdx : SV_DispatchThreadID)
{
    if (_iThreadIdx.x >= g_int_0)
        return;
    
    tRWSharedData[0].iAddCount = g_int_1;
        
    // Dead ��ƼŬ �츮��
    if (0 == tRWData[_iThreadIdx.x].iAlive)
    {
        int iOrigin = tRWSharedData[0].iAddCount;
        int iExchange = 0;
        while (0 < iOrigin)
        {
            int iInputValue = iOrigin - 1;
            InterlockedExchange(tRWSharedData[0].iAddCount, iInputValue, iExchange);
      
            if (iExchange == iOrigin)
            {
                tRWData[_iThreadIdx.x].iAlive = 1;
                break;
            }
          
            iOrigin = iInputValue;
        }
                
        if (1 == tRWData[_iThreadIdx.x].iAlive)
        {
            // ���� ���� ��ġ, ����           
            float2 vUV = float2(((float) _iThreadIdx.x / (float) g_int_0) + g_fAccTime, g_fAccTime);
            vUV.y += sin(vUV.x * 2 * 3.141592);
            
            if (vUV.x > 0)
                vUV.x = frac(vUV.x);
            else
                vUV.x = ceil(abs(vUV.x)) - abs(vUV.x);
            
            if (vUV.y > 0)
                vUV.y = frac(vUV.y);
            else
                vUV.y = ceil(abs(vUV.y)) - abs(vUV.y);
            
            vUV = vUV * g_vec2_0;
            
            float3 vNoise =
            {
                gaussian5x5Sample(vUV + int2(0, -100), g_tex_0)
                , gaussian5x5Sample(vUV + int2(0, 0), g_tex_0)
                , gaussian5x5Sample(vUV + int2(0, 100), g_tex_0)
            };
            
            
            float3 vDir = (float4) 0.f;
            
            vDir = (vNoise - 0.5f) * 2.f;
            
            //vDir = float3(0.f, 1.f, 0.f);
            //vDir.x = (vNoise.x - 0.5f) * 2.f * sin(3.141592 / 8.f);
            //vDir.z = (vNoise.z - 0.5f) * 2.f * sin(3.141592 / 8.f);
                        
            tRWData[_iThreadIdx.x].vWorldDir = normalize(vDir); //normalize((vNoise.xyz - 0.5f) * 2.f);
            tRWData[_iThreadIdx.x].vWorldPos = (vNoise.xyz - 0.5f) * 50;
            tRWData[_iThreadIdx.x].m_fLifeTime = ((g_float_1 - g_float_0) * vNoise.x) + g_float_0;
            tRWData[_iThreadIdx.x].m_fCurTime = 0.f;
        }
    }
    else // Particle Update �ϱ�
    {
        tRWData[_iThreadIdx.x].m_fCurTime += g_fDT;
        if (tRWData[_iThreadIdx.x].m_fLifeTime < tRWData[_iThreadIdx.x].m_fCurTime)
        {
            tRWData[_iThreadIdx.x].iAlive = 0;
            return;
        }
        
        float fRatio = tRWData[_iThreadIdx.x].m_fCurTime / tRWData[_iThreadIdx.x].m_fLifeTime;
        float fSpeed = (g_float_3 - g_float_2) * fRatio + g_float_2;
        tRWData[_iThreadIdx.x].vWorldPos += tRWData[_iThreadIdx.x].vWorldDir * fSpeed * g_fDT;
        
        // ���� ��ƼŬ ���� Ȯ��
        tRWSharedData[0].iCurCount = 0;
        InterlockedAdd(tRWSharedData[0].iCurCount, 1);
    }
}

[numthreads(1024, 1, 1)]
void CS_ParticleUpdate3(int3 _iThreadIdx : SV_DispatchThreadID)
{
    if (_iThreadIdx.x >= g_int_0)
        return;
    
    tRWSharedData[0].iAddCount = g_int_1;
        
    // Dead ��ƼŬ �츮��
    if (0 == tRWData[_iThreadIdx.x].iAlive)
    {
        int iOrigin = tRWSharedData[0].iAddCount;
        int iExchange = 0;
        while (0 < iOrigin)
        {
            int iInputValue = iOrigin - 1;
            InterlockedExchange(tRWSharedData[0].iAddCount, iInputValue, iExchange);
      
            if (iExchange == iOrigin)
            {
                tRWData[_iThreadIdx.x].iAlive = 1;
                break;
            }
          
            iOrigin = iInputValue;
        }
                
        if (1 == tRWData[_iThreadIdx.x].iAlive)
        {
            // ���� ���� ��ġ, ����           
            float2 vUV = float2(((float) _iThreadIdx.x / (float) g_int_0) + g_fAccTime, g_fAccTime);
            vUV.y += sin(vUV.x * 2 * 3.141592);
            
            if (vUV.x > 0)
                vUV.x = frac(vUV.x);
            else
                vUV.x = ceil(abs(vUV.x)) - abs(vUV.x);
            
            if (vUV.y > 0)
                vUV.y = frac(vUV.y);
            else
                vUV.y = ceil(abs(vUV.y)) - abs(vUV.y);
            
            vUV = vUV * g_vec2_0;
            
            float3 vNoise =
            {
                gaussian5x5Sample(vUV + int2(0, -100), g_tex_0)
                , gaussian5x5Sample(vUV + int2(0, 0), g_tex_0)
                , gaussian5x5Sample(vUV + int2(0, 100), g_tex_0)
            };
            
            
            float3 vDir = (float4) 0.f;
            
            vDir = (vNoise - 0.5f) * 2.f;
            
            //vDir = float3(0.f, 1.f, 0.f);
            //vDir.x = (vNoise.x - 0.5f) * 2.f * sin(3.141592 / 8.f);
            //vDir.z = (vNoise.z - 0.5f) * 2.f * sin(3.141592 / 8.f);
                        
            tRWData[_iThreadIdx.x].vWorldDir = normalize(vDir); //normalize((vNoise.xyz - 0.5f) * 2.f);
            tRWData[_iThreadIdx.x].vWorldPos = (vNoise.xyz - 0.5f) * 50;
            tRWData[_iThreadIdx.x].m_fLifeTime = ((g_float_1 - g_float_0) * vNoise.x) + g_float_0;
            tRWData[_iThreadIdx.x].m_fCurTime = 0.f;
        }
    }
    else // Particle Update �ϱ�
    {
        tRWData[_iThreadIdx.x].m_fCurTime += g_fDT;
        if (tRWData[_iThreadIdx.x].m_fLifeTime < tRWData[_iThreadIdx.x].m_fCurTime)
        {
            tRWData[_iThreadIdx.x].iAlive = 0;
            return;
        }
        
        float fRatio = tRWData[_iThreadIdx.x].m_fCurTime / tRWData[_iThreadIdx.x].m_fLifeTime;
        float fSpeed = (g_float_3 - g_float_2) * fRatio + g_float_2;
        tRWData[_iThreadIdx.x].vWorldPos.y -= fSpeed * g_fDT;
        
        // ���� ��ƼŬ ���� Ȯ��
        tRWSharedData[0].iCurCount = 0;
        InterlockedAdd(tRWSharedData[0].iCurCount, 1);
    }
}
[numthreads(1024, 1, 1)]
void CS_ParticleUpdate4(int3 _iThreadIdx : SV_DispatchThreadID)
{
    if (_iThreadIdx.x >= g_int_0)
        return;
    
    tRWSharedData[0].iAddCount = g_int_1;
        
    // Dead ��ƼŬ �츮��
    if (0 == tRWData[_iThreadIdx.x].iAlive)
    {
        int iOrigin = tRWSharedData[0].iAddCount;
        int iExchange = 0;
        while (0 < iOrigin)
        {
            int iInputValue = iOrigin - 1;
            InterlockedExchange(tRWSharedData[0].iAddCount, iInputValue, iExchange);
      
            if (iExchange == iOrigin)
            {
                tRWData[_iThreadIdx.x].iAlive = 1;
                break;
            }
          
            iOrigin = iInputValue;
        }
                
        if (1 == tRWData[_iThreadIdx.x].iAlive)
        {
            // ���� ���� ��ġ, ����           
            float2 vUV = float2(((float) _iThreadIdx.x / (float) g_int_0) + g_fAccTime, g_fAccTime);
            vUV.y += sin(vUV.x * 2 * 3.141592);
            
            if (vUV.x > 0)
                vUV.x = frac(vUV.x);
            else
                vUV.x = ceil(abs(vUV.x)) - abs(vUV.x);
            
            if (vUV.y > 0)
                vUV.y = frac(vUV.y);
            else
                vUV.y = ceil(abs(vUV.y)) - abs(vUV.y);
            
            vUV = vUV * g_vec2_0;
            
            float3 vNoise =
            {
                gaussian5x5Sample(vUV + int2(0, -100), g_tex_0)
                , gaussian5x5Sample(vUV + int2(0, 0), g_tex_0)
                , gaussian5x5Sample(vUV + int2(0, 100), g_tex_0)
            };
            
            
            float3 vDir = (float4) 0.f;
            
            vDir = (vNoise - 0.5f) * 2.f;
            
            //vDir = float3(0.f, 1.f, 0.f);
            //vDir.x = (vNoise.x - 0.5f) * 2.f * sin(3.141592 / 8.f);
            //vDir.z = (vNoise.z - 0.5f) * 2.f * sin(3.141592 / 8.f);
                        
            tRWData[_iThreadIdx.x].vWorldDir = normalize(vDir); //normalize((vNoise.xyz - 0.5f) * 2.f);
            tRWData[_iThreadIdx.x].vWorldPos = (vNoise.xyz - 0.5f) * 50;
            tRWData[_iThreadIdx.x].m_fLifeTime = ((g_float_1 - g_float_0) * vNoise.x) + g_float_0;
            tRWData[_iThreadIdx.x].m_fCurTime = 0.f;
        }
    }
    else // Particle Update �ϱ�
    {
        tRWData[_iThreadIdx.x].m_fCurTime += g_fDT;
        if (tRWData[_iThreadIdx.x].m_fLifeTime < tRWData[_iThreadIdx.x].m_fCurTime)
        {
            tRWData[_iThreadIdx.x].iAlive = 0;
            return;
        }
        
        float fRatio = tRWData[_iThreadIdx.x].m_fCurTime / tRWData[_iThreadIdx.x].m_fLifeTime;
        float fSpeed = (g_float_3 - g_float_2) * fRatio + g_float_2;
        tRWData[_iThreadIdx.x].vWorldPos.y += fSpeed * g_fDT;
        
        // ���� ��ƼŬ ���� Ȯ��
        tRWSharedData[0].iCurCount = 0;
        InterlockedAdd(tRWSharedData[0].iCurCount, 1);
    }
}

[numthreads(1024, 1, 1)]
void CS_ParticleUpdate5(int3 _iThreadIdx : SV_DispatchThreadID)
{
    if (_iThreadIdx.x >= g_int_0)
        return;
    
    tRWSharedData[0].iAddCount = g_int_1;
        
    // Dead ��ƼŬ �츮��
    if (0 == tRWData[_iThreadIdx.x].iAlive)
    {
        int iOrigin = tRWSharedData[0].iAddCount;
        int iExchange = 0;
        while (0 < iOrigin)
        {
            int iInputValue = iOrigin - 1;
            InterlockedExchange(tRWSharedData[0].iAddCount, iInputValue, iExchange);
      
            if (iExchange == iOrigin)
            {
                tRWData[_iThreadIdx.x].iAlive = 1;
                break;
            }
          
            iOrigin = iInputValue;
        }
                
        if (1 == tRWData[_iThreadIdx.x].iAlive)
        {
            // ���� ���� ��ġ, ����           
            float2 vUV = float2(((float) _iThreadIdx.x / (float) g_int_0) + g_fAccTime, g_fAccTime);
            vUV.y += sin(vUV.x * 2 * 3.141592);
            
            if (vUV.x > 0)
                vUV.x = frac(vUV.x);
            else
                vUV.x = ceil(abs(vUV.x)) - abs(vUV.x);
            
            if (vUV.y > 0)
                vUV.y = frac(vUV.y);
            else
                vUV.y = ceil(abs(vUV.y)) - abs(vUV.y);
            
            vUV = vUV * g_vec2_0;
            
            float3 vNoise =
            {
                gaussian5x5Sample(vUV + int2(0, -100), g_tex_0)
                , gaussian5x5Sample(vUV + int2(0, 0), g_tex_0)
                , gaussian5x5Sample(vUV + int2(0, 100), g_tex_0)
            };
            
            
            float3 vDir = (float4) 0.f;
            
            vDir = (vNoise - 0.5f) * 2.f;
            
            //vDir = float3(0.f, 1.f, 0.f);
            //vDir.x = (vNoise.x - 0.5f) * 2.f * sin(3.141592 / 8.f);
            //vDir.z = (vNoise.z - 0.5f) * 2.f * sin(3.141592 / 8.f);
                        
            tRWData[_iThreadIdx.x].vWorldDir = normalize(vDir); //normalize((vNoise.xyz - 0.5f) * 2.f);
            tRWData[_iThreadIdx.x].vWorldPos = (vNoise.xyz - 0.5f) * 50;
            tRWData[_iThreadIdx.x].m_fLifeTime = ((g_float_1 - g_float_0) * vNoise.x) + g_float_0;
            tRWData[_iThreadIdx.x].m_fCurTime = 0.f;
        }
    }
    else // Particle Update �ϱ�
    {
        tRWData[_iThreadIdx.x].m_fCurTime += g_fDT;
        if (tRWData[_iThreadIdx.x].m_fLifeTime < tRWData[_iThreadIdx.x].m_fCurTime)
        {
            tRWData[_iThreadIdx.x].iAlive = 0;
            return;
        }
        
        float fRatio = tRWData[_iThreadIdx.x].m_fCurTime / tRWData[_iThreadIdx.x].m_fLifeTime;
        float fSpeed = (g_float_3 - g_float_2) * fRatio + g_float_2;
        tRWData[_iThreadIdx.x].vWorldPos.x+= tRWData[_iThreadIdx.x].vWorldDir.x * fSpeed * g_fDT;
        tRWData[_iThreadIdx.x].vWorldPos.z += tRWData[_iThreadIdx.x].vWorldDir.z * fSpeed * g_fDT;
        // ���� ��ƼŬ ���� Ȯ��
        tRWSharedData[0].iCurCount = 0;
        InterlockedAdd(tRWSharedData[0].iCurCount, 1);
    }
}

[numthreads(1024, 1, 1)]
void CS_ParticleUpdate6(int3 _iThreadIdx : SV_DispatchThreadID)
{
    if (_iThreadIdx.x >= g_int_0)
        return;

    tRWSharedData[0].iAddCount = g_int_1;

    // Dead ��ƼŬ �츮��
    if (0 == tRWData[_iThreadIdx.x].iAlive)
    {
        int iOrigin = tRWSharedData[0].iAddCount;
        int iExchange = 0;
        while (0 < iOrigin)
        {
            int iInputValue = iOrigin - 1;
            InterlockedExchange(tRWSharedData[0].iAddCount, iInputValue, iExchange);

            if (iExchange == iOrigin)
            {
                tRWData[_iThreadIdx.x].iAlive = 1;
                break;
            }

            iOrigin = iInputValue;
        }

        if (1 == tRWData[_iThreadIdx.x].iAlive)
        {
            // ���� ���� ��ġ, ����           
            float2 vUV = float2(((float)_iThreadIdx.x / (float)g_int_0) + g_fAccTime, g_fAccTime);
            vUV.y += sin(vUV.x * 2 * 3.141592);

            if (vUV.x > 0)
                vUV.x = frac(vUV.x);
            else
                vUV.x = ceil(abs(vUV.x)) - abs(vUV.x);

            if (vUV.y > 0)
                vUV.y = frac(vUV.y);
            else
                vUV.y = ceil(abs(vUV.y)) - abs(vUV.y);

            vUV = vUV * g_vec2_0;

            float3 vNoise =
            {
                gaussian5x5Sample(vUV + int2(0, -100), g_tex_0)
                , gaussian5x5Sample(vUV + int2(0, 0), g_tex_0)
                , gaussian5x5Sample(vUV + int2(0, 100), g_tex_0)
            };


            float3 vDir = (float4) 0.f;

            vDir = (vNoise - 0.5f) * 2.f;

            //vDir = float3(0.f, 1.f, 0.f);
            //vDir.x = (vNoise.x - 0.5f) * 2.f * sin(3.141592 / 8.f);
            //vDir.z = (vNoise.z - 0.5f) * 2.f * sin(3.141592 / 8.f);

            tRWData[_iThreadIdx.x].vWorldDir = normalize(vDir); //normalize((vNoise.xyz - 0.5f) * 2.f);
            tRWData[_iThreadIdx.x].vWorldPos =0;
            tRWData[_iThreadIdx.x].m_fLifeTime = ((g_float_1 - g_float_0) * vNoise.x) + g_float_0;
            tRWData[_iThreadIdx.x].m_fCurTime = 0.f;
        }
    }
    else // Particle Update �ϱ�
    {
        tRWData[_iThreadIdx.x].m_fCurTime += g_fDT;
        if (tRWData[_iThreadIdx.x].m_fLifeTime < tRWData[_iThreadIdx.x].m_fCurTime)
        {
            tRWData[_iThreadIdx.x].iAlive = 0;
            return;
        }

        float fRatio = tRWData[_iThreadIdx.x].m_fCurTime / tRWData[_iThreadIdx.x].m_fLifeTime;
        float fSpeed = (g_float_3 - g_float_2) * fRatio + g_float_2;
        tRWData[_iThreadIdx.x].vWorldPos.y += fSpeed * g_fDT;

        // ���� ��ƼŬ ���� Ȯ��
        tRWSharedData[0].iCurCount = 0;
        InterlockedAdd(tRWSharedData[0].iCurCount, 1);
    }
}

#endif 