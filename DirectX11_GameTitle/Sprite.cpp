#include "main.h"
#include "renderer.h"
#include "Sprite.h"
#include "Scene.h"
#include "manager.h"
#include "Texture.h"
#include "Camera.h"
#define vertex_cnt 4


ID3D11Buffer*CSprite::m_VertexBuffer = NULL;

void CSprite::Load()
{

}

void CSprite::Unload()
{
	//ポインタの解放
	m_VertexBuffer->Release();
}

void CSprite::Init()
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
		"unlitTextureVS.cso");

	CRenderer::CreatePixelShader(
		&m_PixelShader,
		"unlitTexturePS.cso");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale3D = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_volume = 1.0f;
	m_volume_on_flag = false;
	m_ON = true;
}

void CSprite::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void CSprite::Update()
{
	if (m_volume_on_flag==true)
	{
		if (m_volume_flag == false)
		{
			m_volume -= 0.01f;
			if (m_volume <= 0.25f)
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

void CSprite::Draw()
{
	CScene* scene = CManager::GetScene();
	CTexture* p_Texture = scene->GetGameObject<CTexture>(1);
	CTextureDate* p_Texturedate = p_Texture->Texture_GetTexture(textureId);


	//頂点データの書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//m_width = p_Texturedate->Get_m_Width();
	//m_height = p_Texturedate->Get_m_Height();

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

	//カメラのビューマトリクス取得
	//CScene* scene = CManager::GetScene();
	//CCamera* camera = scene->GetGameObject<CCamera>(0);
	//D3DXMATRIX view = camera->GetViewMatrix();

	//ビューの逆行列
	//D3DXMATRIX invView;
	//D3DXMatrixInverse(&invView, NULL, &view);//逆行列
	//invView._41 = 0.0f;
	//invView._42 = 0.0f;
	//invView._43 = 0.0f;

	//マトリクス設定
	CRenderer::SetWorldViewProjection2D();
	//D3DXMATRIX world, scale, trans;
	//D3DXMatrixScaling(&scale, m_Scale3D.x, m_Scale3D.y, m_Scale3D.z);
	////D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	//D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	//world = scale * invView*  trans;
	//CRenderer::SetWorldMatrix(&world);


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
	auto* texture = p_Texturedate->Get_m_Texture();
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);//&m_Texture...初期化時に読み込んでいたもの

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP//ポリゴンをどうやって描くか...TRIANGLESTRIP
	);

	//ポリゴン描画
	ID3D11DeviceContext* deviceContext = CRenderer::GetDeviceContext();
	deviceContext->Draw(4, 0);//ここでポリゴン描画（4...頂点数, 0...授業では使わない）一つの頂点バッファに複数のデータを入れてその一部を使う場合に使う

}

void CSprite::SetSize(float width, float height)
{
	m_width = width;
	m_height = height;
}

void CSprite::SetVolume(bool volume_on_flag)
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