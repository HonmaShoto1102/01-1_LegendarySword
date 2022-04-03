#include "Phase03.h"
#include "Phase04.h"
#include "input.h"

static bool g_Nextflag = false;
static bool g_AnimFlag = false;
//Sword
void CPhase03::Enter(CSword* obj)
{
	m_Counter = 0.0f;
	m_Vibration = 0.0f;
	obj->SetPhaseNum(2);
}

void CPhase03::Exit(CSword* obj)
{

}

CState<CSword>* CPhase03::Update(CSword* obj)
{
	if (g_Nextflag == true)
	{
		return new CPhase04();
	}

	if (g_AnimFlag == true)
	{
		m_Counter += 1.0f;
		m_Vibration = sinf(m_Counter)*0.025f;
		obj->SetPosition(D3DXVECTOR3(sin(m_Vibration), 0.0f, 0.0f));
	}
	return this;
}

//Player
void CPhaseC::Enter(CPlayer* player)
{
	g_Nextflag = false;
	g_AnimFlag = false;
	m_AnimationFlag = false;
	m_AnimationCount = 0;
	player->m_03_Walk_Idle = false;
	player->m_03_Idle_Arm = false;
	player->m_03_Arm_Back = false;
	player->SetRotation(D3DXVECTOR3(0.0f, 3.14159f, 0.0f));
	player->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -12.0f));
	player->SetEndPosition(D3DXVECTOR3(0.0f, 0.0f, -2.0f));
}

void CPhaseC::Exit(CPlayer* player)
{

}

CState<CPlayer>* CPhaseC::Update(CPlayer* player)
{
	if (g_Nextflag == true)
	{
		return new CPhaseD();
	}

	if (m_AnimationFlag == false)
	{
		BoolSwitch(player, 0);
		player->SetAddPosition(D3DXVECTOR3(0.0f, 0.0f, 0.015f));
	}

	if (player->GetPosition().z >= player->GetEndPosition().z - 0.005f)
	{
		player->SetAddPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_AnimationFlag = true;
	}

	if(m_AnimationFlag==true)
	m_AnimationCount++;


	if (m_AnimationCount >= 50 && m_AnimationCount < 200)
	{
		BoolSwitch(player, 1);
	}
	if (m_AnimationCount >= 200)
	{
		BoolSwitch(player, 2);
		player->SetAddPosition(D3DXVECTOR3(0.0f, 0.0f, -0.015f));
	}
	if (m_AnimationCount >= 230)
	{
		g_AnimFlag = true;
	}
	if (m_AnimationCount >= 500)
	{
		g_Nextflag = true;
	}

	return this;
}


void CPhaseC::BoolSwitch(CPlayer* player, int boolSwitch)
{

	switch (boolSwitch)
	{
	case 0://Walk ¨ Idle
		player->m_03_Walk_Idle = true;
		player->m_03_Idle_Arm = false;
		player->m_03_Arm_Back = false;
		break;
	case 1://Idle ¨ Arm
		player->m_03_Walk_Idle = false;
		player->m_03_Idle_Arm = true;
		player->m_03_Arm_Back = false;
		break;
	case 2://Arm¨Back
		player->m_03_Walk_Idle = false;
		player->m_03_Idle_Arm = false;
		player->m_03_Arm_Back = true;
		break;
	default:
		break;
	}
}