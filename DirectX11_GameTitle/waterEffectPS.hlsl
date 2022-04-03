
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
Texture2D		g_Texture2 : register(t1);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	float2 samplePoint = In.TexCoord;
	float4 Tex = g_Texture.Sample(g_SamplerState, samplePoint);
	float vignette = length(float2(0.5, 0.5) - In.TexCoord);
	vignette = clamp(vignette - 0.2, 0, 1);
	outDiffuse.rgb -= vignette;
	
}