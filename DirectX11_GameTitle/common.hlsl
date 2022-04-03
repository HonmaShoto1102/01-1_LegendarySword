


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

// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float4 Position                 : POSITION0;		// ���W( ���[�J����� )
	float4 NoiseProjection			: POSITION1;		// ���ɉf��f���̎��_�ł̎ˉe���W
	float3 Normal                   : NORMAL0;			// �@��( ���[�J����� )
	float4 Diffuse                  : COLOR0;			// �f�B�t���[�Y�J���[
	float4 Specular                 : COLOR1;			// �X�y�L�����J���[
	float4 TexCoords0               : TEXCOORD0;		// �e�N�X�`�����W
};

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 ProjectionPosition       : POSITION;	// ���W( �ˉe��� )
	float4 NoiseProjection			: TEXCOORD0;	// ���ɉf��f���̎��_�ł̎ˉe���W
	float3 ViewPosition             : TEXCOORD1;	// ���W( �r���[��� )
	float3 ViewNormal               : TEXCOORD2;	// �@��( �r���[��� )
	float4 Diffuse                  : COLOR0;		// �f�B�t���[�Y�J���[
};

struct PS_IN
{
	float4 Position		: SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;

};

// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 ProjectionPosition       : SV_POSITION;	// ���W( �ˉe��� )
	float4 NoiseProjection			: TEXCOORD0;	// ���ɉf��f���̎��_�ł̎ˉe���W
	float3 ViewPosition             : TEXCOORD1;	// ���W( �r���[��� )
	float3 ViewNormal               : TEXCOORD2;	// �@��( �r���[��� )
	float4 Diffuse                  : COLOR0;		// �f�B�t���[�Y�J���[
};

// �s�N�Z���V�F�[�_�[�̏o��
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

cbuffer CameraBuffer : register(b5)	//�o�b�t�@�̂T�ԂƂ���
{
	float4 CameraPosition;	//�J�������W���󂯂Ƃ�ϐ�
}

cbuffer ParameterBuffer : register(b6)
{
	float4 Parameter; //C���ꑤ����float4�f�[�^���󂯎����ꕨ
}

