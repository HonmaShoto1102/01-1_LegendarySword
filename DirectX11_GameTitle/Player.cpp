#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "Scene.h"
#include "AnimationModel.h"
#include "Player.h"
#include "Camera.h"
#include "Field.h"
#include "audio.h" 
#include "Phase01.h"


CAnimationModel* CPlayer::m_AniModel;
//Audio* Player::m_ShotSE;
ID3D11VertexShader* CPlayer::m_VertexShader = NULL;
ID3D11PixelShader* CPlayer::m_PixelShader = NULL;
ID3D11InputLayout* CPlayer::m_VertexLayout = NULL;

void CPlayer::Load()
{

	m_AniModel = new CAnimationModel();
	m_AniModel->Load("asset\\model\\animation\\Akai_Idle.fbx");
	m_AniModel->LoadAnimation("asset\\model\\animation\\Akai_Idle.fbx", "Idle");
	m_AniModel->LoadAnimation("asset\\model\\animation\\Crouched Walking.fbx","Walk");
	m_AniModel->LoadAnimation("asset\\model\\animation\\Disarmed.fbx", "Arm");
	m_AniModel->LoadAnimation("asset\\model\\animation\\Walk Backwards.fbx", "Back");
	m_AniModel->LoadAnimation("asset\\model\\animation\\Running Turn 180.fbx", "Turn");
	m_AniModel->LoadAnimation("asset\\model\\animation\\Fast Run.fbx", "Run");


	/*m_ShotSE = new Audio();
	m_ShotSE->Load("asset\\audio\\bgm.wav");*/

	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	CRenderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

}
void CPlayer::Unload()
{
	/*m_ShotSE->Uninit();
	delete m_ShotSE;*/
	m_AniModel->UnLoad();
	delete m_AniModel;
	/*m_Model->Unload();
	delete m_Model;*/
}
void CPlayer::Init()
{
	m_FrameIdle = 0;
	m_FrameWalk = 0;
	m_FrameBack = 0;
	m_FrameArm = 0;

	m_Position = D3DXVECTOR3(0.0f, 0.0f, -12.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 3.14159f, 0.0f);
	m_Scale3D = D3DXVECTOR3(0.02f, 0.02f, 0.02f);

	CScene* scene = CManager::GetScene();
	m_ShotSE = scene->AddGameObject<CAudio>(2);
	m_ShotSE->Load("asset\\audio\\shot.wav");
	m_ShotSE -> SetVolume(0.1f);

	m_StateSystem = new CStateSystem<CPlayer>();
	m_StateSystem->Initialize(new CPhaseA(), this);
}

void CPlayer::Uninit()
{
}

void CPlayer::Update()
{
	
	
	//アニメーションブレンド
	if (m_03_Walk_Idle == true)
	{
		m_AniModel->UpdateAnimBlend("Walk", "Idle", m_BlendWalk, m_FrameWalk);
		m_FrameWalk++;
		m_BlendWalk += AnimationLerp(0.0f, 10.0f, 0.000115f);
	}
	else {
		m_BlendWalk = 0.0f;
	}
	if (m_03_Idle_Arm == true)
	{
		m_AniModel->UpdateAnimBlend("Idle", "Arm", m_BlendIdle, m_FrameIdle);
		m_FrameIdle++;
		m_BlendIdle += AnimationLerp(0.0f, 10.0f, 0.003f);
	}
	else {
		m_BlendIdle = 0.0f;
	}
	if (m_03_Arm_Back == true)
	{
		m_AniModel->UpdateAnimBlend("Arm", "Back", m_BlendArm, m_FrameArm);
		m_FrameArm++;
		m_BlendArm += AnimationLerp(0.0f, 10.0f, 0.8f);
	}
	else {
		m_BlendArm = 0.0f;
	}
	if (m_04_Back_Turn == true)
	{
		m_AniModel->UpdateAnimBlend("Back", "Turn", m_BlendBack, m_FrameBack);
		m_FrameBack++;
		m_BlendBack += AnimationLerp(0.0f, 10.0f, 0.2f);
	}
	else {
		m_BlendBack = 0.0f;
	}
	//アニメーション
	if (m_04_Run == true)
	{
		m_AniModel->Update("Run",  m_FrameTurn);
		m_FrameTurn++;
	}


	if (m_BlendWalk > 1.0f)
		m_BlendWalk = 1.0f;
	if (m_BlendIdle > 1.0f)
		m_BlendIdle = 1.0f;
	if (m_BlendArm > 1.0f)
		m_BlendArm = 1.0f;
	if (m_BlendBack > 1.0f)
		m_BlendBack = 1.0f;


	if (CInput::GetKeyTrigger(VK_SPACE))
	{
		m_ShotSE->Play();//true...ループ再生　なし...ワンショット再生
	}
	

	//メッシュフィールド高さ取得
	CField* meshField = CManager::GetScene()->GetGameObject<CField>(1);

	m_Position.y = meshField->GetHight(m_Position)/*-1.0f*/;//-1.0f;←浮いている場合//地面の高さから計算←ジャンプの場合

	m_StateSystem->Update(this);
}

void CPlayer::Draw()
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

	/*m_Model->Draw();*/
	m_AniModel->Draw();
}