#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "Result.h"
#include "ResultLogo.h"
#include "Game.h"
#include "input.h"

void CResult::Init()
{
	AddGameObject<CResultLogo>(2);
}

void CResult::Uninit()
{
	CScene::Uninit();//継承クラスのメソッド呼び出し
}

void CResult::Update()
{
	CScene::Update();
	if (CInput::GetKeyTrigger(VK_RETURN))
	{
		CManager::SetScene<CGame>();
	}

}