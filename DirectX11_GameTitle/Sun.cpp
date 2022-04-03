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


	//頂点バッファの生成
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
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
		"asset/texture/sun.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);


}

void CSun::Unload()
{
	//ポインタの解放
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
	//視錐台カリング
	CScene* scene = CManager::GetScene();
	CCamera* camera = scene->GetGameObject<CCamera>(0);

	if (!camera->CheckView(m_Position))
		return;

	//入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);//DirectX9でのFVF

	//シェーダー設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);//頂点シェーダー、、Unityのマテリアル
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);//ピクセルシェーダー

	D3DXMATRIX view = camera->GetViewMatrix();

	//ビューの逆行列
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);//逆行列
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	//ワールドマトリクス設定
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, m_Scale3D.x, m_Scale3D.y, m_Scale3D.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView* trans;
	CRenderer::SetWorldMatrix(&world);


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
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);


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