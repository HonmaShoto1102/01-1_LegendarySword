#include "common.hlsl"
void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;//���[���h�r���[�v���W�F�N�V�����s��
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0); //�@�����R�s�[�i�����O�ɂ���j
	worldNormal = mul(normal, World);//�@�������[���h�ϊ����Ă���
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;
	//Parameter.x�ɕ`��񐔂����͂���Ă���B
	//���_��@���x�N�g��*�`���*0.02�����ړ������ă��f�����g�傷��
	In.Position += normal * Parameter.x * 0.02; //0.02�͓K���ȃX�P�[���l
	Out.Position = mul(In.Position, wvp);//WVP�s�����Z����
	//�����v�Z ���ʂ̃����o�[�g���n�[�t�����o�[�g��OK
	float light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);
	Out.Diffuse = light;
	Out.Diffuse.a = In.Diffuse.a;
	//�e�N�X�`�����W
	Out.TexCoord = In.TexCoord;
}