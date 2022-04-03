#include "main.h"
#include "renderer.h"
#include "Sun.h"
#include "Scene.h"
#include "manager.h"
#include "Camera.h"
#include "Phase01.h"

ID3D11Buffer*CSun::m_VertexBuffer = NULL;
ID3D11ShaderResourceView* CSun::m_Texture = NULL;

void CSun::Load()
{
	VERTEX_3D vertex[4];
	float size = 1.5f;

	vertex[0].Position = D3DXVECTOR3(-size, size, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(size, size, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-size, -size, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(size, -size, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//���_�o�b�t�@�̐���
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
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
		"asset/texture/sun.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);


}

void CSun::Unload()
{
	//�|�C���^�̉��
	m_VertexBuffer->Release();
	m_Texture->Release();
}

void CSun::Init()
{

	CRenderer::CreateVertexShader(
		&m_VertexShader,
		&m_VertexLayout,
		"unlitTextureVS.cso");//unlitTextureVS,BloomVS

	CRenderer::CreatePixelShader(
		&m_PixelShader,
		"unlitTexturePS.cso");//unlitTexturePS,BloomPS

	
	m_Position = (D3DXVECTOR3(sinf(-1.7f)*55.0f, cosf(0.0f)*30.0f, 75.0f));
	m_Scale3D = D3DXVECTOR3(8.0f, 8.0f, 8.0f);

	m_StateSystem = new CStateSystem<CSun>();
	m_StateSystem->Initialize(new CPhaseRaise01(), this);
}

void CSun::Uninit()
{
	
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void CSun::Update()
{
	m_StateSystem->Update(this);
}

void CSun::Draw()
{
	//������J�����O
	CScene* scene = CManager::GetScene();
	CCamera* camera = scene->GetGameObject<CCamera>(0);

	if (!camera->CheckView(m_Position))
		return;

	//���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);//DirectX9�ł�FVF

	//�V�F�[�_�[�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);//���_�V�F�[�_�[�A�AUnity�̃}�e���A��
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);//�s�N�Z���V�F�[�_�[

	D3DXMATRIX view = camera->GetViewMatrix();

	//�r���[�̋t�s��
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);//�t�s��
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	//���[���h�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, m_Scale3D.x, m_Scale3D.y, m_Scale3D.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView* trans;
	CRenderer::SetWorldMatrix(&world);


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
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);


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