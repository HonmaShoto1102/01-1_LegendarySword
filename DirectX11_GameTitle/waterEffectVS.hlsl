
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{

	//１頂点分のデータを受け取り、処理し、出力する
	//頂点変換処理　この処理は必ず必要
	matrix wvp;								//行列変数を作成
	wvp = mul(World, View);					//wvp = ワールド行列 * カメラ行列
	wvp = mul(wvp, Projection);				//wvp = wvp * プロジェクション行列
	Out.Position = mul(In.Position, wvp);	//変数結果を出力する

	//法線をワールド変換して出力
	float4 worldNormal, normal;
	normal = float4 (In.Normal.xyz, 0.0);	//normal.wは０にする（平行移動はしない）
	worldNormal = mul(normal, World);		//ワールド変換
	worldNormal = normalize(worldNormal);	//回転後の法線を正規化する
	Out.Normal = worldNormal;				//回転後の法線出力



	//受け取ったこの頂点ディフューズをそのまま出力
	Out.Diffuse = In.Diffuse;

	//頂点のテクスチャ座標を出力
	Out.TexCoord = In.TexCoord;

	Out.WorldPosition = mul(In.Position, World);//ワールド変換した頂点座標を出力


}
