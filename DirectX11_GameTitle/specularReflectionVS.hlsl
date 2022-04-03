
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp; //行列計算用
	wvp = mul(World, View);
	wvp = mul(wvp, Projection); //頂点変換行列作成
	Out.Position = mul(In.Position, wvp); //頂点を変換して出力
	
	float4 worldNormal, normal; //法線をワールド行列で回転
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal); //変換した法線を正規化
	Out.Normal = worldNormal; //ワールド変換した法線を出力


	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse;

	//ワールド変換した頂点座標を出力
	Out.WorldPosition = mul(In.Position, World);

}

