#pragma once

#include "gameObject.h"

class CField:public CGameObject//gameObjectの中身を引っ張ってくる（継承）
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11Buffer*				m_IndexBuffer = NULL;
	//ID3D11ShaderResourceView*	m_Texture = NULL;
	ID3D11ShaderResourceView* m_FurTexture[1];//Furテクスチャ

	VERTEX_3D m_Vertex[21][21];

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
public:
	//Field();
	//~Field();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	float GetHight(D3DXVECTOR3 Position);

};