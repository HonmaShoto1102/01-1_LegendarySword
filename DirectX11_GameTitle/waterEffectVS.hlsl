
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{

	//�P���_���̃f�[�^���󂯎��A�������A�o�͂���
	//���_�ϊ������@���̏����͕K���K�v
	matrix wvp;								//�s��ϐ����쐬
	wvp = mul(World, View);					//wvp = ���[���h�s�� * �J�����s��
	wvp = mul(wvp, Projection);				//wvp = wvp * �v���W�F�N�V�����s��
	Out.Position = mul(In.Position, wvp);	//�ϐ����ʂ��o�͂���

	//�@�������[���h�ϊ����ďo��
	float4 worldNormal, normal;
	normal = float4 (In.Normal.xyz, 0.0);	//normal.w�͂O�ɂ���i���s�ړ��͂��Ȃ��j
	worldNormal = mul(normal, World);		//���[���h�ϊ�
	worldNormal = normalize(worldNormal);	//��]��̖@���𐳋K������
	Out.Normal = worldNormal;				//��]��̖@���o��



	//�󂯎�������̒��_�f�B�t���[�Y�����̂܂܏o��
	Out.Diffuse = In.Diffuse;

	//���_�̃e�N�X�`�����W���o��
	Out.TexCoord = In.TexCoord;

	Out.WorldPosition = mul(In.Position, World);//���[���h�ϊ��������_���W���o��


}
