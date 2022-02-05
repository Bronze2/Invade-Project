#ifndef _COMPUTE
#define _COMPUTE
#include "value.fx"
RWTexture2D<float4> g_rw_tex_0 : register(u0);
[numthreads(1024,1,1)]
void CS_TEST(int3 _iThreadIdx : SV_DispatchThreadID)
{
    if (g_int_0)
    {
        if(_iThreadIdx.y==512)
            g_rw_tex_0[_iThreadIdx.xy] = float4(0.f, 1.f, 0.f, 1.f);
        else
            g_rw_tex_0[_iThreadIdx.xy] = float4(0.f, 0.f, 0.f, 1.f);
    }
    else
    {
        if (_iThreadIdx.y == 512)
            g_rw_tex_0[_iThreadIdx.xy] = float4(0.f, 0.f, 1.f, 1.f);
        else
            g_rw_tex_0[_iThreadIdx.xy] = float4(0.f, 0.f, 0.f, 1.f);
    }
}

#endif 