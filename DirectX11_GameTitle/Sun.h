#pragma once

#include "gameObject.h"
#include "StateSystem.h"

class CSun :public CGameObject//gameObject�̒��g�����������Ă���i�p���j
{
private:
	
	static ID3D11Buffer*				m_VertexBuffer;
	static ID3D11ShaderResourceView*	m_Texture;

	
	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;

	//�X�e�[�g
	CStateSystem<CSun>* m_StateSystem;
public:

	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();
};