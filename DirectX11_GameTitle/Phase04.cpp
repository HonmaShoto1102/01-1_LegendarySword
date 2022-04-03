#include "Phase04.h"
#include "Phase01.h"
#include "input.h"
static bool g_Nextflag = false;
//Sword
void CPhase04::Enter(CSword* obj)
{
	obj->SetPhaseNum(3);
	m_Counter = 0.0f;
	m_Vibration = 0.0f;

	time = WATER{};
	time.NoiseProjection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void CPhase04::Exit(CSword* obj)
{

}

CState<CSword>* CPhase04::Update(CSword* obj)
{
	if (g_Nextflag == true)
	{
		g_Nextflag = true;
		return new CPhase01();
	}

	m_Counter += 1.0f;
	m_Vibration = sinf(m_Counter)*0.05f;
	obj->SetPosition(D3DXVECTOR3(sin(m_Vibration), m_Counter*0.01f, 0.0f));

	time.NoiseProjection += D3DXVECTOR3(2.0f, 0.0f, 0.0f);
	CRenderer::SetWater(time);

	return this;
}

//Player
void CPhaseD::Enter(CPlayer* player)
{
	m_AnimationCount = 0;
	g_Nextflag = false;
	player->m_04_Back_Turn = false;
	player->m_04_Run = false;
	player->SetAddPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

void CPhaseD::Exit(CPlayer* player)
{
	player->m_04_Run = false;
	player->SetAddPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

CState<CPlayer>* CPhaseD::Update(CPlayer* player)
{
	
	if (g_Nextflag==true)
	{
		return new CPhaseA();
	}

	m_AnimationCount++;

	if (m_AnimationCount < 50)
	{
		BoolSwitch(player, 0);
	}
	if (m_AnimationCount >= 50)
	{
		BoolSwitch(player, 1);
		
		player->SetAddPosition(D3DXVECTOR3(0.0f, 0.0f, -0.1f));
		player->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else if (player->GetPosition().z <= -12.0f)
	{
		player->SetAddPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (m_AnimationCount >= 800)
	{
		g_Nextflag = true;
	}

	return this;
}

void CPhaseD::BoolSwitch(CPlayer* player, int boolSwitch)
{

	switch (boolSwitch)
	{
	case 0://Back ¨ Turn
		player->m_04_Back_Turn = true;
		player->m_04_Run = false;
		break;
	case 1://Run
		player->m_04_Back_Turn = false;
		player->m_04_Run = true;
		break;
	default:
		break;
	}
}