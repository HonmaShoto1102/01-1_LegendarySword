#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "Rock.h"

CModel* CRock::m_Model;
ID3D11VertexShader* CRock::m_VertexShader = NULL;
ID3D11PixelShader* CRock::m_PixelShader = NULL;
ID3D11InputLayout* CRock::m_VertexLayout = NULL;

void CRock::Init()
{
	m_Model = new CModel();
	m_Model->Load("asset\\model\\stand.obj");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0, 1.57f, 0.0f);
	m_Scale3D = D3DXVECTOR3(1.3, 1.0, 1.3);

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
	CRenderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");//specularReflectionPS,vertexLightingPS,BloomPS
}

void CRock::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void CRock::Update()
{
}

void CRock::Draw()
{
	// 入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);	// DX9でいうFVF

	// シェーダ設定
	// Unityで言うマテリアル設定みたいなもの
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale3D.x, m_Scale3D.y, m_Scale3D.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);

	world = scale * rot * trans;
	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
}

void CRock::SetPositions(D3DXVECTOR3 position)
{
	m_Position += position;
}

void CRock::SetRotation(D3DXVECTOR3 rotation)
{
	m_Rotation = rotation;

}