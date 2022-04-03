


cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}




struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	float3 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}




struct LIGHT
{
	bool Enable;
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}

struct WATER
{
	float3 NoiseProjection;
	bool dummy;
};

cbuffer WaterBuffer : register(b5)
{
	WATER Water;
}


struct VS_IN
{
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;

};

// 頂点シェーダーの入力
struct VS_INPUT
{
	float4 Position                 : POSITION0;		// 座標( ローカル空間 )
	float4 NoiseProjection			: POSITION1;		// 鏡に映る映像の視点での射影座標
	float3 Normal                   : NORMAL0;			// 法線( ローカル空間 )
	float4 Diffuse                  : COLOR0;			// ディフューズカラー
	float4 Specular                 : COLOR1;			// スペキュラカラー
	float4 TexCoords0               : TEXCOORD0;		// テクスチャ座標
};

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 ProjectionPosition       : POSITION;	// 座標( 射影空間 )
	float4 NoiseProjection			: TEXCOORD0;	// 鏡に映る映像の視点での射影座標
	float3 ViewPosition             : TEXCOORD1;	// 座標( ビュー空間 )
	float3 ViewNormal               : TEXCOORD2;	// 法線( ビュー空間 )
	float4 Diffuse                  : COLOR0;		// ディフューズカラー
};

struct PS_IN
{
	float4 Position		: SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;

};

// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 ProjectionPosition       : SV_POSITION;	// 座標( 射影空間 )
	float4 NoiseProjection			: TEXCOORD0;	// 鏡に映る映像の視点での射影座標
	float3 ViewPosition             : TEXCOORD1;	// 座標( ビュー空間 )
	float3 ViewNormal               : TEXCOORD2;	// 法線( ビュー空間 )
	float4 Diffuse                  : COLOR0;		// ディフューズカラー
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0         : COLOR0;
};

struct PS_IN_PARTICLE
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION0;
	float2 TexCoord : TEXCOORD0;
};

struct VS_IN_PARTICLE
{
	float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
	uint InstanceID : SV_InstanceID;
};

cbuffer CameraBuffer : register(b5)	//バッファの５番とする
{
	float4 CameraPosition;	//カメラ座標を受けとる変数
}

cbuffer ParameterBuffer : register(b6)
{
	float4 Parameter; //C言語側からfloat4データを受け取る入れ物
}

