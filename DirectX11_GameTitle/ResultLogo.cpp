#include "main.h"
#include "renderer.h"
#include "ResultLogo.h"

ID3D11VertexShader* CResultLogo::m_VertexShader = NULL;
ID3D11PixelShader* CResultLogo::m_PixelShader = NULL;
ID3D11InputLayout* CResultLogo::m_VertexLayout = NULL;

#define START_POSX (SCREEN_WIDTH/4)

void CResultLogo::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(START_POSX, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH/2+ START_POSX, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(START_POSX, SCREEN_HEIGHT/2, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH/2+ START_POSX, SCREEN_HEIGHT/2, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//���_�o�b�t�@�̐���
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//�e�N�X�`���̓ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/result.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);


	CRenderer::CreateVertexShader(
		&m_VertexShader,
		&m_VertexLayout,
		"unlitTextureVS.cso");

	CRenderer::CreatePixelShader(
		&m_PixelShader,
		"unlitTexturePS.cso");

}

void CResultLogo::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void CResultLogo::Update()
{
	//�摜�𓮂���
}

void CResultLogo::Draw()
{
	//���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);//DirectX9�ł�FVF

	//�V�F�[�_�[�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);//���_�V�F�[�_�[�A�AUnity�̃}�e���A��
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);//�s�N�Z���V�F�[�_�[

	//�}�g���N�X�ݒ�
	CRenderer::SetWorldViewProjection2D();

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(
		0,
		1,
		&m_VertexBuffer,//�`��̒��_�ݒ�
		&stride,
		&offset
	);

	//�e�N�X�`���ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);//&m_Texture...���������ɓǂݍ���ł�������

	//�v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP//�|���S�����ǂ�����ĕ`����...TRIANGLESTRIP
	);

	//�|���S���`��
	ID3D11DeviceContext* deviceContext = CRenderer::GetDeviceContext();
	deviceContext->Draw(4, 0);//�����Ń|���S���`��i4...���_��, 0...���Ƃł͎g��Ȃ��j��̒��_�o�b�t�@�ɕ����̃f�[�^�����Ă��̈ꕔ���g���ꍇ�Ɏg��

}