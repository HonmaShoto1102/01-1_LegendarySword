#pragma once

#include "ImGUImanager.h"
#include "gameObject.h"

class CManager
{
private:
	static class CScene* m_pScene;
	static class CImGUI* m_pGUI;
	static class CSword* m_pSword;

	static class CRendererSprite* m_RendererSprite;
	static class CRendererSprite2* m_RendererSprite2;
	static class CRendererSprite3* m_RendererSprite3;
	static class CRendererSprite4* m_RendererSprite4;


public:
	//--メソッド、メンバ関数--
	static void Init();//初期化
	static void Uninit();//終了
	static void Update();//更新
	static void Draw();//描画
	

	static class CScene* GetScene() { return m_pScene; }

	template <typename T>
		static void SetScene()
	{
			if (m_pScene)
			{
				m_pScene->Uninit();
				delete m_pScene;
			}

			T* scene = new T();
			m_pScene = scene;
			scene->Init();
	}

};