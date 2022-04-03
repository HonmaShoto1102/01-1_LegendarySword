#include "main.h"
#include "renderer.h"
#include "Field.h"

#define Field_X 21
#define Field_Z 21

#define floor -5.0f
#define rood -1.0f
#define ornament 0.75f

ID3D11VertexShader* CField::m_VertexShader = NULL;
ID3D11PixelShader* CField::m_PixelShader = NULL;
ID3D11InputLayout* CField::m_VertexLayout = NULL;
float num = 1;

float g_FieldHeight[Field_X][Field_Z]
{
	{floor,	floor,	floor,	floor,	6.0f,	5.0f,	6.0f,	3.5f,		4.5f,	5.0f,			3.4f,			4.0f,	4.3f,		4.4f,	5.0f,	4.4f,	4.7f,	5.2f,	6.0f,	6.5f,	6.3f},
	{floor,	floor,	floor,	floor,	4.0f,	4.0f,	3.0f,	2.6f,		3.2f,	3.0f,			2.5f,			2.2f,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	5.0f,	floor,	floor,	floor,	floor,		4.4f,	floor,			floor,			floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	5.0f,	floor,	floor,	floor,	floor,	5.5f,		floor,	floor,			floor,			floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	7.0f,	6.0f,	floor,	3.6f,	floor,	floor,		floor,	floor,			floor,			floor,	floor,		floor,	4.7f,	6.3f,	5.3f,	5.4f,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	floor,			floor,			floor,	floor,		floor,	floor,	floor,	floor,	floor,	7.4f,	8.8f,	9.4f},
	{7.0f,	floor,	floor,	floor,	floor,	floor,	floor,	ornament,	floor,	floor,			floor,			floor,	ornament,	floor,	floor,	floor,	5.0f,	5.8f,	floor,	floor,	floor},
	{floor,	8.0f,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	floor,			floor,			floor,	floor,		floor,	floor,	5.5f,	4.2f,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	ornament,	floor,	floor,			floor,			floor,	ornament,	floor,	floor,	floor,	floor,	6.7f,	floor,	7.5f,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	rood - 0.5f,	rood - 0.5f,	floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	ornament,	floor,	rood,			rood,			floor,	ornament,	floor,	floor,	floor,	floor,	floor,	floor,	floor,	8.6f},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	rood - 0.05f,	rood - 0.05f,	floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	rood - 0.1f,	rood - 0.1f,	floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	rood - 0.15f,	rood - 0.15f,	floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	rood,			rood,			floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	rood,			rood,			floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	rood,			rood,			floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	rood,			rood,			floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	rood,			rood,			floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	rood,			rood,			floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},
	{floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor,		floor,	rood,			rood,			floor,	floor,		floor,	floor,	floor,	floor,	floor,	floor,	floor,	floor},

};


void CField::Init()
{
	//頂点バッファの生成
	{
		for (int x = 0; x < Field_X; x++)
		{
			for (int z = 0; z < Field_Z; z++)
			{
				float y = g_FieldHeight[z][x];
				m_Vertex[x][z].Position = D3DXVECTOR3((x-10)*5.0f,y, (z - 10)*-5.0f);
				m_Vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//法線ベクトル
				m_Vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[x][z].TexCoord = D3DXVECTOR2(x*0.5f, z*0.5f);
			}
		}

		//法線ベクトル算出
		for (int x = 1; x < Field_X - 1; x++)
		{
			for (int z = 1; z < Field_Z-1; z++)
			{
				D3DXVECTOR3 vx, vz, vn;

				vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;
				vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;

				D3DXVec3Cross(&vn, &vz, &vx);//外積
				D3DXVec3Normalize(&vn, &vn);//正規化（長さ１にする）
				m_Vertex[x][z].Normal = vn;
			}
		}


		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * (Field_X)*(Field_Z);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = m_Vertex;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	//インデックスバッファ生成
	{
		unsigned int index[((Field_X + 1) * 2) * (Field_X - 1) - 2];

		int i = 0;
		for (int x = 0; x < Field_X-1; x++)
		{
			for (int z = 0; z < Field_Z; z++)
			{
				index[i] = x * Field_Z + z;
				i++;

				index[i] = (x + 1)*Field_Z + z;
				i++;
			}

			if (x == (Field_X-2))
				break;

			//縮退ポリゴン用インデックス
			index[i] = (x + 1)*Field_X + (Field_X-1);
			i++;

			index[i] = (x + 1)*Field_X;
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int)*(((Field_X + 1) * 2)*Field_X - 3);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);

	}

	//テクスチャの読み込み
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/rock.jpg",
		NULL,
		NULL,
		&m_FurTexture[0],
		NULL);

	assert(m_FurTexture[0]);

	
	CRenderer::CreateVertexShader(
		&m_VertexShader,
		&m_VertexLayout,
		"unlitTextureVS.cso");//unlitTextureVS

	CRenderer::CreatePixelShader(
		&m_PixelShader,
		"unlitTexturePS.cso");//unlitTexturePS

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale3D = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

}

void CField::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_FurTexture[0]->Release();
	
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void CField::Update()
{
	
}

void CField::Draw()
{
	//入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);//DirectX9でのFVF

	//シェーダー設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);//頂点シェーダー、、Unityのマテリアル
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);//ピクセルシェーダー

	//ワールドマトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale3D.x, m_Scale3D.y, m_Scale3D.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot*trans;
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

	//インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);


	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_FurTexture[0]);//&m_Texture...初期化時に読み込んでいたもの



	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP//ポリゴンをどうやって描くか...TRIANGLESTRIP
	);

	//ポリゴン描画
	ID3D11DeviceContext* deviceContext = CRenderer::GetDeviceContext();
	deviceContext->DrawIndexed(((Field_X + 1) * 2)*Field_Z - 3, 0, 0);

}

float CField::GetHight(D3DXVECTOR3 Position)
{
	int x, z;

	//ブロック番号算出←プレイヤーの周り算出
	x = Position.x / 5.0f + 10.0f;		//defineとconstにする
	z = Position.z / -5.0f + 10.0f;	//defineとconstにする

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	//プレイヤーの周りの４頂点
	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	D3DXVECTOR3 v12, v1p, c;

	v12 = pos2 - pos1;
	v1p = Position - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	D3DXVECTOR3 n;

	if (c.y > 0.0f)
	{
		//左上ポリゴン
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else
	{
		//右下ポリゴン
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}

	//高さ取得
	py = -((Position.x - pos1.x)*n.x + (Position.z - pos1.z)*n.z) / n.y + pos1.y;

	return py;

}