#include "main.h"
#include "renderer.h"
#include "RendererSprite.h"
#include "Scene.h"
#include "manager.h"
#include "Texture.h"
#include "Camera.h"
#define vertex_cnt 4


ID3D11Buffer*CRendererSprite::m_VertexBuffer = NULL;

void CRendererSprite::Load()
{

}

void CRendererSprite::Unload()
{
	//ポインタの解放
	m_VertexBuffer->Release();
}

void CRendererSprite::Init()
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
		"postEffectVS.cso");//postEffectVS,unlitTextureVS

	CRenderer::CreatePixelShader(
		&m_PixelShader,
		"postEffectPS.cso");//NoisePS,postEffectPS,unlitTexturePS

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale3D = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_volume = 1.0f;
	m_volume_on_flag = false;
	m_ON = true;
}

void CRendererSprite::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void CRendererSprite::Update()
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

void CRendererSprite::Draw()
{
	CScene* scene = CManager::GetScene();
	CTexture* p_Texture = scene->GetGameObject<CTexture>(1);
	CTextureDate* p_Texturedate = p_Texture->Texture_GetTexture(textureId);


	//頂点データの書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0] = { D3DXVECTOR3(m_Position.x,m_Position.y,m_Position.z),D3DXVECTOR3(0.0f, 1.0f, 0.0f),D3DXVECTOR4(1.0f,1.0f,1.0f,m_volume),D3DXVECTOR2(0.0f,0.0f) };
	vertex[1] = { D3DXVECTOR3(m_Position.x + m_width,m_Position.y,m_Position.z),D3DXVECTOR3(0.0f, 1.0f, 0.0f),D3DXVECTOR4(1.0f,1.0f,1.0f,m_volume),D3DXVECTOR2(1.0f,0.0f) };
	vertex[2] = { D3DXVECTOR3(m_Position.x,m_Position.y + m_height,m_Position.z),D3DXVECTOR3(0.0f, 1.0f, 0.0f),D3DXVECTOR4(1.0f,1.0f,1.0f,m_volume),D3DXVECTOR2(0.0f,1.0f) };
	vertex[3] = { D3DXVECTOR3(m_Position.x + m_width,m_Position.y + m_height,m_Position.z),D3DXVECTOR3(0.0f, 1.0f, 0.0f),D3DXVECTOR4(1.0f,1.0f,1.0f,m_volume),D3DXVECTOR2(1.0f,1.0f) };

	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);//DirectX9でのFVF

	//シェーダー設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);//頂点シェーダー、、Unityのマテリアル
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);//ピクセルシェーダー

	//マトリクス設定
	CRenderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(
		0,
		1,
		&m_VertexBuffer,//描画の頂点設定
		&stride,
		&offset
	);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);


	//テクスチャ設定
	auto* m_TextureRenderTargetView = CRenderer::GetShaderResourceView();
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_TextureRenderTargetView);//&m_Texture...初期化時に読み込んでいたもの

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP//ポリゴンをどうやって描くか...TRIANGLESTRIP
	);

	//ポリゴン描画
	ID3D11DeviceContext* deviceContext = CRenderer::GetDeviceContext();
	deviceContext->Draw(4, 0);//ここでポリゴン描画（4...頂点数, 0...授業では使わない）一つの頂点バッファに複数のデータを入れてその一部を使う場合に使う

}

void CRendererSprite::SetSize(float width, float height)
{
	m_width = width;
	m_height = height;
}

void CRendererSprite::SetVolume(bool volume_on_flag)
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