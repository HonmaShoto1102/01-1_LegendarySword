#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Camera.h"
#include "WaterCamera.h"
#include "Field.h"

#include "model.h"
#include "Rock.h"
#include "Sword.h"
#include "Lake.h"
#include "Sun.h"
#include "Player.h"

#include "Polygon2D.h"
#include "skyDoom.h"
#include "audio.h"
#include "Game.h"
#include "Texture.h"
#include "Sprite.h"
#include "Result.h"

void CGame::Init()
{
	CSun::Load();
	CLake::Load();
	CSword::Load();
	CPlayer::Load();
	

	AddGameObject<CCamera>(0);
	AddGameObject<CWaterCamera>(0);
	AddGameObject<CField>(1)->SetPosition(D3DXVECTOR3(2.5f, 0.0f, 0.5f));
	AddGameObject<CPlayer>(1);
	AddGameObject<CRock>(1)->SetPosition(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	AddGameObject<CSkyDoom>(1);
	AddGameObject<CSun>(1);
	AddGameObject<CLake>(1)->SetPosition(D3DXVECTOR3(0.0f, -1.4f, 0.0f));
	AddGameObject<CSword>(1);

	AddGameObject<CTexture>(1)->Load();
	CSprite* sprite_title = AddGameObject<CSprite>(2);
	sprite_title->SetTextureID(0);
	sprite_title->SetSize(50.0f*10.0f, 9.0f*10.0f);
	sprite_title->SetPosition(D3DXVECTOR3(20.0f, 0.0f, 0.0f));

	CSprite* sprite_enter = AddGameObject<CSprite>(2);
	sprite_enter->SetTextureID(1);
	sprite_enter->SetSize(65.0f*5.0f, 9.0f*5.0f);
	sprite_enter->SetPosition(D3DXVECTOR3(575.0f, 20.0f, 0.0f));
	sprite_enter->SetVolume(true);

	
	//AddGameObject<CLake>(1);
	
	//CAudio *bgm = AddGameObject<CAudio>(2);
	//bgm->Load("asset\\audio\\bgm.wav");
	//bgm->SetVolume(0.1f);
	//bgm->Play(true);//true...ループ再生　なし...ワンショット再生
	
}

void CGame::Uninit()
{
	
	CScene::Uninit();//継承元クラスのメソッド呼び出し
	CSprite::Unload();
	CSword::Unload();
	CPlayer::Unload();
	CLake::Unload();
	CSun::Unload();

}
void CGame::Update()
{
	CScene::Update();	
	
	

}