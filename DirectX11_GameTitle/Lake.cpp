#include "main.h"
#include "renderer.h"
#include "Lake.h"
#include "manager.h"
#include "Lake.h"

ID3D11ShaderResourceView* CLake::m_Texture = NULL;
ID3D11VertexShader* CLake::m_VertexShader = NULL;
ID3D11PixelShader* CLake::m_PixelShader = NULL;
ID3D11InputLayout* CLake::m_VertexLayout = NULL;


void CLake::Load()
{

	//�e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/Water.jpg",	//������ : �ǂݍ��݂����e�N�X�`���̃t�@�C����
		NULL,
		NULL,
		&m_Texture,
		NULL);

	//�G���[�����m�iDebug���̂݁j
	assert(m_Texture);

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,"BloomVS.cso");//BloomVS,unlitTextureVS

	CRenderer::CreatePixelShader(&m_PixelShader, "BloomPS.cso");//BloomPS,unlitTexturePS

}

void CLake::Unload()
{

	m_Texture->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void CLake::Init()
{
	//������
	m_WaveData = new Corrugation*[m_WaveMax];

	for (int i = 0; i < m_WaveMax; i++) {
		m_WaveData[i] = new Corrugation[m_WaveMax];
	}

	m_WaveVertex = new VERTEX_3D[m_WaveMax * m_WaveMax];

	m_WaveIndices = new unsigned int[m_WaveNum * m_WaveNum * 6];



	for (int i = 0; i < m_WaveMax; i++)
	{
		for (int j = 0; j < m_WaveMax; j++)
		{
			//�g�̖�(�O���b�h�쐬)
			m_WaveVertex[i * m_WaveMax + j].Position = D3DXVECTOR3((float)((j - m_WaveMax / 2) * m_WavePitch), 0.0f, ((float)((i - m_WaveMax / 2) * m_WavePitch) * -1));
			m_WaveVertex[i + m_WaveMax + j].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_WaveVertex[i * m_WaveMax + j].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_WaveVertex[i * m_WaveMax + j].TexCoord = D3DXVECTOR2(j * (1.0f / m_WaveMax), i * (1.0f / m_WaveMax));
			m_WaveData[i][j].amplitude = m_WaveAmplitude; //�U��
			m_WaveData[i][j].time = 0.0f;
		}
	}
	{

		//���_�o�b�t�@����
		D3D11_BUFFER_DESC bd{};			//���_�o�b�t�@�̐ݒ�
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * m_WaveMax * m_WaveMax; //���_�o�b�t�@�̃T�C�Y
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//�����Œ��_�o�b�t�@�ɂ���i���_�o�b�t�@�t���O�����Ă�j
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd{};	//���_�o�b�t�@�̏����l�̐ݒ�
		sd.pSysMem = m_WaveVertex;		//�ŏ��ɓ���Ă������_���

		//�����Œ��_�o�b�t�@���쐬
		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}



	unsigned short index = 0;

	for (int i = 0; i < m_WaveNum; i++)
	{
		for (int j = 0; j < m_WaveNum; j++)
		{
			m_WaveIndices[index + 0] = j + m_WaveMax * i;
			m_WaveIndices[index + 1] = j + 1 + m_WaveMax * i;
			m_WaveIndices[index + 2] = j + m_WaveMax * (i + 1);
			m_WaveIndices[index + 3] = j + 1 + m_WaveMax * i;
			m_WaveIndices[index + 4] = j + 1 + m_WaveMax * (i + 1);
			m_WaveIndices[index + 5] = j + m_WaveMax * (i + 1);
			index += 6;
		}
	}

	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * m_WaveNum * m_WaveNum * 6;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_WaveIndices;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	m_Position = D3DXVECTOR3(0.0f, 0, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale3D = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

}

void CLake::Update()
{

	float		length, dx, dz;

	for (int i = 0; i < m_WaveMax; i++)
	{
		for (int j = 0; j < m_WaveMax; j++)
		{
			//�g�̖�(�O���b�h�쐬)
			m_WaveVertex[i * m_WaveMax + j].Position = D3DXVECTOR3((float)((j - m_WaveMax / 2) * m_WavePitch), 0.0f, ((float)((i - m_WaveMax / 2) * m_WavePitch) * -1));
			m_WaveVertex[i + m_WaveMax + j].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_WaveVertex[i * m_WaveMax + j].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_WaveVertex[i * m_WaveMax + j].TexCoord = D3DXVECTOR2(j * (1.0f / m_WaveMax), i * (1.0f / m_WaveMax));

			m_WaveData[i][j].amplitude = m_WaveAmplitude; //�U��
			m_WaveData[i][j].time = 0.0f;




			//�U�����_���狗�������߂�
			dx = m_WaveVertex[i * m_WaveMax + j].Position.x - m_WaveVertex[m_WaveMax / 2 * m_WaveMax + m_WaveMax / 2].Position.x;
			dz = m_WaveVertex[i * m_WaveMax + j].Position.z - m_WaveVertex[m_WaveMax / 2 * m_WaveMax + m_WaveMax / 2].Position.z;
			length = sqrtf(dx * dx + dz * dz);

			//���������ɐU���̎�����ψʂ����߂�
			m_WaveVertex[i * m_WaveMax + j].Position.y = m_WaveAmplitude * sinf(2.0f * (float)m_PI * ((length / m_WaveLength) - (m_WaveTime / m_WAVE_CYCLE))) * 0.1f;
		}
	}

	m_WaveTime += 0.06f;

	//���܃x�N�g������UV���v�Z����
	for (int i = 0; i < m_WaveMax * m_WaveMax; i++)
	{


		//���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


		//�|���S���ݒ�
		for (int i = 0; i < m_WaveMax; i++) {
			for (int j = 0; j < m_WaveMax; j++) {

				vertex[i * m_WaveMax + j].Position = m_WaveVertex[i * m_WaveMax + j].Position;
				vertex[i + m_WaveMax + j].Normal = m_WaveVertex[i * m_WaveMax + j].Normal;
				vertex[i * m_WaveMax + j].Diffuse = m_WaveVertex[i * m_WaveMax + j].Diffuse;
				vertex[i * m_WaveMax + j].TexCoord = m_WaveVertex[i * m_WaveMax + j].TexCoord;
			}
		}


		CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	}

}

void CLake::Draw()
{

	//���̓��C�A�E�g�ݒ�
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout); //directX9�̎���FVF

	//�V�F�[�_�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���b�N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale3D.x, m_Scale3D.y, m_Scale3D.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	CRenderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�̐ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�C���f�b�N�o�b�t�@�̐ݒ�
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//�e�N�X�`���ݒ�
	//CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	//�e�N�X�`���ݒ�
	auto* m_TextureRenderTargetView = CRenderer::GetShaderResourceView2();
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);//&m_Texture...���������ɓǂݍ���ł�������


	//�}�e���A�����̐ݒ�
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);


	//�v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//ID3D11RasterizerState *rs = NULL;

	//Renderer::GetDeviceContext()->RSGetState(&rs);

	//D3D11_RASTERIZER_DESC rasterizerDesc{};
	//rs->GetDesc(&rasterizerDesc);

	//rasterizerDesc.CullMode = D3D11_CULL_NONE;


	//�|���S���`��
	CRenderer::GetDeviceContext()->DrawIndexed(m_WaveNum * m_WaveNum * 6, 0, 0);//���_���Őݒ�

	//rasterizerDesc.CullMode = D3D11_CULL_BACK;
}

void CLake::Uninit()
{

	m_IndexBuffer->Release();
	m_VertexBuffer->Release();
}
