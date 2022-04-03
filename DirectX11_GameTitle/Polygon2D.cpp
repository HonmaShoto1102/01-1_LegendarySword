#include "main.h"
#include "renderer.h"
#include "Polygon2D.h"

ID3D11VertexShader* CPolygon2D::m_VertexShader = NULL;
ID3D11PixelShader* CPolygon2D::m_PixelShader = NULL;
ID3D11InputLayout* CPolygon2D::m_VertexLayout = NULL;

void CPolygon2D::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f,1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, 200.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(200.0f, 200.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファの生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャの読み込み
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/block.png",
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

void CPolygon2D::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void CPolygon2D::Update()
{
	//画像を動かす
}

void CPolygon2D::Draw()
{
	//入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);//DirectX9でのFVF

	//シェーダー設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);//頂点シェーダー、、Unityのマテリアル
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);//ピクセルシェーダー

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

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);//&m_Texture...初期化時に読み込んでいたもの

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP//ポリゴンをどうやって描くか...TRIANGLESTRIP
	);

	//ポリゴン描画
	ID3D11DeviceContext* deviceContext = CRenderer::GetDeviceContext();
	deviceContext->Draw(4, 0);//ここでポリゴン描画（4...頂点数, 0...授業では使わない）一つの頂点バッファに複数のデータを入れてその一部を使う場合に使う

}