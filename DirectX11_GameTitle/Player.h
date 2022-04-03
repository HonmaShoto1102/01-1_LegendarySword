#pragma once

#include "gameObject.h"
#include "StateSystem.h"

class CPlayer :public CGameObject//gameObjectの中身を引っ張ってくる（継承）
{

private:
	int m_FrameIdle;
	int m_FrameWalk;
	int m_FrameArm;
	int m_FrameBack;
	int m_FrameTurn;
	int m_FrameRun;

	static class CAnimationModel* m_AniModel;

	//static Audio* m_ShotSE;
	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

	class CAudio* m_ShotSE;

	D3DXQUATERNION m_Quaternion;
	D3DXQUATERNION m_quatA, m_quatD, m_quatS, m_quatW;

	//ステート
	CStateSystem<CPlayer>* m_StateSystem;

	float AnimationLerp(float start, float end, float t)
	{
		return (1 - t) * start + t * end;
	}

public:
	bool m_03_Walk_Idle = false;
	bool m_03_Idle_Arm = false;
	bool m_03_Arm_Back = false;
	bool m_04_Back_Turn = false;
	bool m_04_Run = false;

	float m_BlendWalk;
	float m_BlendIdle;
	float m_BlendArm;
	float m_BlendBack;
	float m_BlendTurn;

	static void Load();
	static void Unload();


	void Init();
	void Uninit();
	void Update();
	void Draw();

};