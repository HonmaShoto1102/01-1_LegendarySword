
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

//適当なSeed値をもとに疑似乱数を計算する
float Rand(float2 coord)
{
	//frac(x)=xの小数部を返す 乱数は0.0 <= rand < 1.0の範囲
	return frac(sin(dot(coord.xy, float2(12.9898f, 78.233f))) * 43758.5453f);
}

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	//テクスチャデータ取得
	float4 Tex = g_Texture.Sample(g_SamplerState, In.TexCoord);
	//テクスチャ座標とC言語からのパラメータ値をSeedにして乱数を作成
	float noise = clamp(0.2f, 0.5f, Rand(In.TexCoord * Parameter.y));
	//乱数値を輝度値としてテクスチャのデータへ加算する
	Tex.rgb += noise;
	//頂点（ピクセル）色と合成
	outDiffuse.rgb = saturate(Tex.rgb) * In.Diffuse;
	outDiffuse.a = Tex.a * In.Diffuse.a; //αは別処理
	//この後にoutDiffuseをグレースケール化して終了
	outDiffuse = 0.299*outDiffuse.r + 0.587*outDiffuse.g + 0.114*outDiffuse.b;

}
