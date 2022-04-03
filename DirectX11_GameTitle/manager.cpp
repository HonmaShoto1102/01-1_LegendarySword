#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "RendererSprite.h"
#include "RendererSprite2.h"
#include "RendererSprite3.h"
#include "RendererSprite4.h"
#include "input.h"
#include "Scene.h"
#include "audio.h"
#include "Game.h"
#include "Camera.h"
#include "WaterCamera.h"
#include "Sword.h"

CSword* CManager::m_pSword = NULL;
CScene* CManager::m_pScene = NULL;
CImGUI* CManager::m_pGUI = NULL;
CRendererSprite* CManager::m_RendererSprite = NULL;
CRendererSprite2* CManager::m_RendererSprite2 = NULL;
CRendererSprite3* CManager::m_RendererSprite3 = NULL;
CRendererSprite4* CManager::m_RendererSprite4 = NULL;

void CManager::Init()
{
	CRenderer::Init();
	CInput::Init();
	CAudio::InitMaster();

	m_pScene = new CGame();
	m_pScene->Init();

	m_RendererSprite = new CRendererSprite();
	m_RendererSprite->Init();
	m_RendererSprite->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_RendererSprite2 = new CRendererSprite2();
	m_RendererSprite2->Init();
	m_RendererSprite2->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_RendererSprite3 = new CRendererSprite3();
	m_RendererSprite3->Init();
	m_RendererSprite3->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_RendererSprite4 = new CRendererSprite4();
	m_RendererSprite4->Init();
	m_RendererSprite4->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	m_pSword = new CSword();
	m_pSword->Init();
	
#ifdef _DEBUG
	m_pGUI = new CImGUI();
	m_pGUI->Init();
#endif
}


void CManager::Uninit()
{
	m_pSword->Uninit();
	delete m_pSword;
	m_pScene->Uninit();
	delete m_pScene;
	m_RendererSprite4->Uninit();
	delete m_RendererSprite4;
	m_RendererSprite3->Uninit();
	delete m_RendererSprite3; 
	m_RendererSprite2->Uninit();
	delete m_RendererSprite2;
	m_RendererSprite->Uninit();
	delete m_RendererSprite;

#ifdef _DEBUG
	m_pGUI->Uninit();
	delete m_pGUI;
#endif

	CAudio::UninitMaster();
	CInput::Uninit();
	CRenderer::Uninit();	
}

void CManager::Update()
{
	CInput::Update();//ˆê”ÔÅ‰

	m_pSword->Update();
	m_pScene->Update();
	m_RendererSprite->Update();
	m_RendererSprite2->Update();
	m_RendererSprite3->Update();
	m_RendererSprite4->Update();

}

void CManager::Draw()
{
	
	int m_Phase = m_pSword->GetPhaseNum();

	switch (m_Phase)
	{
	case 0:
		CRenderer::RendererBegin();
		m_pScene->GetGameObject<CCamera>(0)->Draw();
		break;
	case 1:
		CRenderer::RendererBegin2();
		m_pScene->GetGameObject<CCamera>(0)->Draw();
		break;
	case 2:
		CRenderer::RendererBegin3();
		m_pScene->GetGameObject<CCamera>(0)->Draw();
		break;
	case 3:
		CRenderer::RendererBegin4();
		m_pScene->GetGameObject<CCamera>(0)->Draw();
		break;
	default:
		break;
	}
	
	m_pScene->Draw_Object();
	CRenderer::Begin();

	switch (m_Phase)
	{
	case 0:
		m_RendererSprite->Draw();
		break;
	case 1:
		m_RendererSprite2->Draw();
		break;
	case 2:
		m_RendererSprite3->Draw();
		break;
	case 3:
		m_RendererSprite4->Draw();
		break;
	default:
		break;
	}

	m_pScene->Draw_UI();

#ifdef _DEBUG
	m_pGUI->Draw();
#endif


	CRenderer::End();
}
