#pragma once

#include "gameObject.h"
#include "StateSystem.h"
#include "model.h"
class CSword :public CGameObject//gameObject�̒��g�����������Ă���i�p���j
{

private:
	int m_Frame;

	static CModel* m_Model;
	//static Audio* m_ShotSE;
	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

	class CAudio* m_ShotSE;

	
	//�X�e�[�g
	CStateSystem<CSword>* m_StateSystem;

public:
	static void Load();
	static void Unload();


	void Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXQUATERNION m_Quaternion;
	D3DXQUATERNION m_quatA, m_quatD, m_quatS, m_quatW, m_quatInit;
};