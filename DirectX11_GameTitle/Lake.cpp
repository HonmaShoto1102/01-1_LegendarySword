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

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/Water.jpg",	//第二引数 : 読み込みたいテクスチャのファイル名
		NULL,
		NULL,
		&m_Texture,
		NULL);

	//エラーを検知（Debug時のみ）
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
	//初期化
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
			//波の面(グリッド作成)
			m_WaveVertex[i * m_WaveMax + j].Position = D3DXVECTOR3((float)((j - m_WaveMax / 2) * m_WavePitch), 0.0f, ((float)((i - m_WaveMax / 2) * m_WavePitch) * -1));
			m_WaveVertex[i + m_WaveMax + j].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_WaveVertex[i * m_WaveMax + j].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_WaveVertex[i * m_WaveMax + j].TexCoord = D3DXVECTOR2(j * (1.0f / m_WaveMax), i * (1.0f / m_WaveMax));
			m_WaveData[i][j].amplitude = m_WaveAmplitude; //振幅
			m_WaveData[i][j].time = 0.0f;
		}
	}
	{

		//頂点バッファ生成
		D3D11_BUFFER_DESC bd{};			//頂点バッファの設定
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * m_WaveMax * m_WaveMax; //頂点バッファのサイズ
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//ここで頂点バッファにする（頂点バッファフラグを建てる）
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd{};	//頂点バッファの初期値の設定
		sd.pSysMem = m_WaveVertex;		//最初に入れておく頂点情報

		//ここで頂点バッファを作成
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
			//波の面(グリッド作成)
			m_WaveVertex[i * m_WaveMax + j].Position = D3DXVECTOR3((float)((j - m_WaveMax / 2) * m_WavePitch), 0.0f, ((float)((i - m_WaveMax / 2) * m_WavePitch) * -1));
			m_WaveVertex[i + m_WaveMax + j].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_WaveVertex[i * m_WaveMax + j].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_WaveVertex[i * m_WaveMax + j].TexCoord = D3DXVECTOR2(j * (1.0f / m_WaveMax), i * (1.0f / m_WaveMax));

			m_WaveData[i][j].amplitude = m_WaveAmplitude; //振幅
			m_WaveData[i][j].time = 0.0f;




			//振動原点から距離を求める
			dx = m_WaveVertex[i * m_WaveMax + j].Position.x - m_WaveVertex[m_WaveMax / 2 * m_WaveMax + m_WaveMax / 2].Position.x;
			dz = m_WaveVertex[i * m_WaveMax + j].Position.z - m_WaveVertex[m_WaveMax / 2 * m_WaveMax + m_WaveMax / 2].Position.z;
			length = sqrtf(dx * dx + dz * dz);

			//距離を元に振動の式から変位を求める
			m_WaveVertex[i * m_WaveMax + j].Position.y = m_WaveAmplitude * sinf(2.0f * (float)m_PI * ((length / m_WaveLength) - (m_WaveTime / m_WAVE_CYCLE))) * 0.1f;
		}
	}

	m_WaveTime += 0.06f;

	//屈折ベクトルからUVを計算する
	for (int i = 0; i < m_WaveMax * m_WaveMax; i++)
	{


		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


		//ポリゴン設定
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

	//入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout); //directX9の時のFVF

	//シェーダ設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale3D.x, m_Scale3D.y, m_Scale3D.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	CRenderer::SetWorldMatrix(&world);

	//頂点バッファの設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//インデックバッファの設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//テクスチャ設定
	//CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	//テクスチャ設定
	auto* m_TextureRenderTargetView = CRenderer::GetShaderResourceView2();
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);//&m_Texture...初期化時に読み込んでいたもの


	//マテリアル情報の設定
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);


	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//ID3D11RasterizerState *rs = NULL;

	//Renderer::GetDeviceContext()->RSGetState(&rs);

	//D3D11_RASTERIZER_DESC rasterizerDesc{};
	//rs->GetDesc(&rasterizerDesc);

	//rasterizerDesc.CullMode = D3D11_CULL_NONE;


	//ポリゴン描画
	CRenderer::GetDeviceContext()->DrawIndexed(m_WaveNum * m_WaveNum * 6, 0, 0);//頂点数で設定

	//rasterizerDesc.CullMode = D3D11_CULL_BACK;
}

void CLake::Uninit()
{

	m_IndexBuffer->Release();
	m_VertexBuffer->Release();
}
