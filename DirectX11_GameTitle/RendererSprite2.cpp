#include "main.h"
#include "renderer.h"
#include "RendererSprite2.h"
#include "Scene.h"
#include "manager.h"
#include "Texture.h"
#include "Camera.h"
#define vertex_cnt 4

#define MIRROR_NUM			(1)


// ���ɉf��f���̎擾�Ɏg�p����N���[���̒��̋��̎l���̍��W( �������W )
D3DXVECTOR4 MirrorScreenPosW[MIRROR_NUM][4];

// ���̃��[���h���W
D3DXVECTOR3 MirrorWorldPos[MIRROR_NUM][4] =
{
	{
		{ -4000.0f, 400.0f,  4500.0f },
		{  4000.0f, 400.0f,  4500.0f },
		{ -4000.0f, 400.0f, -4500.0f },
		{  4000.0f, 400.0f, -4500.0f },
	}
};

ID3D11Buffer*CRendererSprite2::m_VertexBuffer = NULL;

void CRendererSprite2::Load()
{

}

void CRendererSprite2::Unload()
{
	//�|�C���^�̉��
	m_VertexBuffer->Release();
}

void CRendererSprite2::Init()
{
	VERTEX_3D vertex[4];
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;


	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	CRenderer::CreateVertexShader(
		&m_VertexShader,
		&m_VertexLayout,
		"unlitTextureVS.cso");//postEffectVS,unlitTextureVS

	CRenderer::CreatePixelShader(
		&m_PixelShader,
		"unlitTexturePS.cso");//NoisePS,waterEffectPS

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale3D = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_volume = 1.0f;
	m_volume_on_flag = false;
	
}

void CRendererSprite2::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void CRendererSprite2::Update()
{
	if (m_volume_on_flag == true)
	{
		if (m_volume_flag == false)
		{
			m_volume -= 0.01f;
			if (m_volume <= 0.0f)
			{
				m_volume_flag = true;
			}
		}
		else
		{
			m_volume += 0.01f;
			if (m_volume >= 1.0f)
			{
				m_volume_flag = false;
			}
		}
	}
}

void CRendererSprite2::Draw()
{
	CScene* scene = CManager::GetScene();
	CTexture* p_Texture = scene->GetGameObject<CTexture>(1);
	CTextureDate* p_Texturedate = p_Texture->Texture_GetTexture(m_textureId);


	//���_�f�[�^�̏�������
	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0] = { D3DXVECTOR3(m_Position.x,m_Position.y,m_Position.z),D3DXVECTOR3(0.0f, 1.0f, 0.0f),D3DXVECTOR4(1.0f,1.0f,1.0f,m_volume),D3DXVECTOR2(0.0f,0.0f) };
	vertex[1] = { D3DXVECTOR3(m_Position.x + m_width,m_Position.y,m_Position.z),D3DXVECTOR3(0.0f, 1.0f, 0.0f),D3DXVECTOR4(1.0f,1.0f,1.0f,m_volume),D3DXVECTOR2(1.0f,0.0f) };
	vertex[2] = { D3DXVECTOR3(m_Position.x,m_Position.y + m_height,m_Position.z),D3DXVECTOR3(0.0f, 1.0f, 0.0f),D3DXVECTOR4(1.0f,1.0f,1.0f,m_volume),D3DXVECTOR2(0.0f,1.0f) };
	vertex[3] = { D3DXVECTOR3(m_Position.x + m_width,m_Position.y + m_height,m_Position.z),D3DXVECTOR3(0.0f, 1.0f, 0.0f),D3DXVECTOR4(1.0f,1.0f,1.0f,m_volume),D3DXVECTOR2(1.0f,1.0f) };

	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

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

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);


	//�e�N�X�`���ݒ�
	auto* m_TextureRenderTargetView = CRenderer::GetShaderResourceView2();
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_TextureRenderTargetView);//&m_Texture...���������ɓǂݍ���ł�������

	//�v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP//�|���S�����ǂ�����ĕ`����...TRIANGLESTRIP
	);

	//�|���S���`��
	ID3D11DeviceContext* deviceContext = CRenderer::GetDeviceContext();
	deviceContext->Draw(4, 0);//�����Ń|���S���`��i4...���_��, 0...���Ƃł͎g��Ȃ��j��̒��_�o�b�t�@�ɕ����̃f�[�^�����Ă��̈ꕔ���g���ꍇ�Ɏg��

}

void CRendererSprite2::SetSize(float width, float height)
{
	m_width = width;
	m_height = height;
}

void CRendererSprite2::SetVolume(bool volume_on_flag)
{
	if (volume_on_flag == true)
	{
		m_volume_on_flag = volume_on_flag;
	}
	else
	{
		m_volume = 1.0f;
	}
}