#pragma once

#include "gameObject.h"
#include "StateSystem.h"

class CSun :public CGameObject//gameObjectの中身を引っ張ってくる（継承）
{
private:
	
	static ID3D11Buffer*				m_VertexBuffer;
	static ID3D11ShaderResourceView*	m_Texture;

	
	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;

	//ステート
	CStateSystem<CSun>* m_StateSystem;
public:

	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();
};