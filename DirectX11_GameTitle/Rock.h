#pragma once

#include "gameObject.h"
#include "model.h"

class CRock : public CGameObject
{
private:
	static CModel* m_Model;

	static ID3D11VertexShader* m_VertexShader;  // 頂点シェーダー
	static ID3D11PixelShader* m_PixelShader;	// ピクセルシェーダー
	static ID3D11InputLayout* m_VertexLayout;	// 頂点レイアウト

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPositions(D3DXVECTOR3 position);
	void SetRotation(D3DXVECTOR3 rotation);
	
};

