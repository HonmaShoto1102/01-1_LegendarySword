#pragma once

#include "gameObject.h"

class CResultLogo :public CGameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

public:
	//Polygon2D();
	//~Polygon2D();

	void Init();
	void Uninit();
	void Update();
	void Draw();
};