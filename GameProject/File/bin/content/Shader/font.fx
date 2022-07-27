#ifndef _FONT
#define _FONT
#include "value.fx"
#include "func.fx"

struct VTX_FONT_INPUT
{
	float4 vPos : POSITION;
	float4 vUV : TEXCOORD;
	float4 vColor : COLOR;
};

struct VTX_FONT_OUTPUT
{
	float4 vPos : SV_Position;
	float2 vUV : TEXCOORD;
	float4 vColor : COLOR;
};

VTX_FONT_OUTPUT VS_FONT(VTX_FONT_INPUT _input, uint vertexID : SV_VertexID)
{
	VTX_FONT_OUTPUT output = (VTX_FONT_OUTPUT)0.f;

	float2 uv = float2(vertexID & 1, (vertexID >> 1) & 1);

	output.vPos = float4(_input.vPos.x + (_input.vPos.z * uv.x), _input.vPos.y - (_input.vPos.w * uv.y), 0, 1);

	output.vColor = _input.vColor;

	output.vUV = float2(_input.vUV.x + (_input.vUV.z * uv.x), _input.vUV.y + (_input.vUV.w * uv.y));

	return output;
}

float4 PS_FONT(VTX_FONT_OUTPUT _input) : SV_Target
{
	float4 vColor = (float4)0.f;
	vColor = (_input.vColor.rgb, _input.vColor.a * g_tex_0.Sample(g_sam_0, _input.vUV).a) * _input.vColor;
	return vColor;
}

#endif