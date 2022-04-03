#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Scene.h"
#include "model.h"
#include "Sword.h"
#include "Camera.h"
#include "Field.h"
#include "audio.h" 
#include "Phase01.h"

CModel* CSword::m_Model;
//Audio* Player::m_ShotSE;
ID3D11VertexShader* CSword::m_VertexShader = NULL;
ID3D11PixelShader* CSword::m_PixelShader = NULL;
ID3D11InputLayout* CSword::m_VertexLayout = NULL;

void CSword::Load()
{
	m_Model = new CModel();
	m_Model->Load("asset\\model\\sword\\Sword.obj");//\\or//
	
	
	/*m_ShotSE = new Audio();
	m_ShotSE->Load("asset\\audio\\bgm.wav");*/

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "specularReflectionVS.cso");//specularReflectionVS,vertexLightingVS,BloomVS
	CRenderer::CreatePixelShader(&m_PixelShader, "specularReflectionPS.cso");//specularReflectionPS,vertexLightingPS,BloomPS

}
void CSword::Unload()
{
	m_Model->Unload();
	delete m_Model;
}
void CSword::Init()
{
	m_Frame = 0;
	m_Position = D3DXVECTOR3(0.0f, 6.4f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale3D = D3DXVECTOR3(4.5f, 4.5f, 4.5f);

	m_StateSystem = new CStateSystem<CSword>();
	m_StateSystem->Initialize(new CPhase01(), this);
}

void CSword::Uninit()
{
}

void CSword::Update()
{

	//ステートの更新
	m_StateSystem->Update(this);
}

void CSword::Draw()
{
	//視錐台カリング
	CScene* scene = CManager::GetScene();
	CCamera* camera = scene->GetGameObject<CCamera>(0);

	if (!camera->CheckView(m_Position))
		return;

	//入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale3D.x, m_Scale3D.y, m_Scale3D.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot*trans;

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();
	
}