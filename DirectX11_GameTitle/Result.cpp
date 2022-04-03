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
	CScene::Uninit();//�p���N���X�̃��\�b�h�Ăяo��
}

void CResult::Update()
{
	CScene::Update();
	if (CInput::GetKeyTrigger(VK_RETURN))
	{
		CManager::SetScene<CGame>();
	}

}