#pragma once

#include "gameObject.h"
#include "StateSystem.h"

struct Corrugation
{
	float amplitude;	//U•
	float time;			//ŠÔ
};


class CLake : public CGameObject
{

private:

	//”gİ’è’l
	const int	m_WavePitch = 10;					//”g‚ÌŠÔŠu
	const int	m_WaveNum = 10;
	const int	m_WaveMax = m_WaveNum + 1;			//”g‚Ì‘”
	const float m_WaveAmplitude = 1.5f;				//”g‚ÌU•
	const float m_WaveLength = 50.0f;				//”g‚Ì”g’·
	const float m_WAVE_CYCLE = 10.0f;				//”g‚ÌüŠú
	const float m_PI = 3.141592f;					//ŒÅ’è’l
	
	float m_WaveTime = 0.0f;						//”g‚ÌƒJƒEƒ“ƒgƒAƒbƒv

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;


	static ID3D11ShaderResourceView* m_Texture;
	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

	Corrugation** m_WaveData;
	VERTEX_3D* m_WaveVertex;

	unsigned int* m_WaveIndices;					//’¸“_”

	
public:
	
	static void Load();
	static void Unload();

	void Init();
	void Update();
	void Draw();
	void Uninit();
	
	bool m_On = false;
};