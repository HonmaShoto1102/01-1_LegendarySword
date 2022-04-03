#include "common.hlsl"


Texture2D		g_Texture : register(t0);
Texture2D		g_Texture2 : register(t1);
SamplerState	g_SamplerState : register(s0);

float WhiteNoise(float2 coord) {
	return frac(sin(dot(coord, float2(8.7819, 3.255))) * 437.645);
}

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	float2 samplePoint = In.TexCoord;
	float4 Tex = g_Texture.Sample(g_SamplerState, samplePoint);
	float noise = WhiteNoise(In.TexCoord * Water.NoiseProjection.x) - 0.5;
	outDiffuse.rgb += float3(noise, noise, noise);
	
}