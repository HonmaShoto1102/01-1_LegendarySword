#pragma once

#include "gameObject.h"
#include "model.h"

class CRock : public CGameObject
{
private:
	static CModel* m_Model;

	static ID3D11VertexShader* m_VertexShader;  // ���_�V�F�[�_�[
	static ID3D11PixelShader* m_PixelShader;	// �s�N�Z���V�F�[�_�[
	static ID3D11InputLayout* m_VertexLayout;	// ���_���C�A�E�g

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPositions(D3DXVECTOR3 position);
	void SetRotation(D3DXVECTOR3 rotation);
	
};

