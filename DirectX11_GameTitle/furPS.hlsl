#include "common.hlsl"
Texture2D g_Texture : register(t0);
Texture2D g_TextureFur : register(t1); //ファーテクスチャ
SamplerState g_SamplerState : register(s0);
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//テクスチャを取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	//頂点の明るさを乗算して合成し出力
	outDiffuse *= In.Diffuse;
	//αはファーシェーダー用テクスチャのα値を乗算してから出力
	outDiffuse.a *= g_TextureFur.Sample(g_SamplerState, In.TexCoord).a;
}