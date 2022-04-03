#pragma once

#include "gameObject.h"
#include "StateSystem.h"

struct Corrugation
{
	float amplitude;	//�U��
	float time;			//����
};


class CLake : public CGameObject
{

private:

	//�g�ݒ�l
	const int	m_WavePitch = 10;					//�g�̊Ԋu
	const int	m_WaveNum = 10;
	const int	m_WaveMax = m_WaveNum + 1;			//�g�̑���
	const float m_WaveAmplitude = 1.5f;				//�g�̐U��
	const float m_WaveLength = 50.0f;				//�g�̔g��
	const float m_WAVE_CYCLE = 10.0f;				//�g�̎���
	const float m_PI = 3.141592f;					//�Œ�l
	
	float m_WaveTime = 0.0f;						//�g�̃J�E���g�A�b�v

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;


	static ID3D11ShaderResourceView* m_Texture;
	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

	Corrugation** m_WaveData;
	VERTEX_3D* m_WaveVertex;

	unsigned int* m_WaveIndices;					//���_��

	
public:
	
	static void Load();
	static void Unload();

	void Init();
	void Update();
	void Draw();
	void Uninit();
	
	bool m_On = false;
};