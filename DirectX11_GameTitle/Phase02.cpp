#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Phase01.h"
#include "Phase02.h"
#include "Phase03.h"
#include "input.h"

static bool g_Nextflag = false;
//Sword
void CPhase02::Enter(CSword* obj)
{
	obj->SetPhaseNum(1);
	g_Nextflag = false;

	/*bool On = true;
	CPhaseRaise02 sunRaise;
	sunRaise.SetValue(On);*/
}

void CPhase02::Exit(CSword* obj)
{
}

CState<CSword>* CPhase02::Update(CSword* obj)
{
	
	obj->SetAddPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	if(g_Nextflag == true)
	{
		return new CPhase03();
	}
	return this;
}

//Player
void CPhaseB::Enter(CPlayer* player)
{
}

void CPhaseB::Exit(CPlayer* player)
{

}

CState<CPlayer>* CPhaseB::Update(CPlayer* player)
{
	
	if (g_Nextflag ==true)
	{
		return new CPhaseC();
	}
	return this;
}

//Sun
void CPhaseRaise02::Enter(CSun* sun)
{
	sun->SetScale3D(D3DXVECTOR3(8.5f, 8.5f, 8.5f));
	m_Move = -1.75f;
	m_Start = true;
	sun->SetPosition(D3DXVECTOR3(sinf(m_Move)*55.0f, cosf(m_Move)*30.0f, 75.0f));
}

void CPhaseRaise02::Exit(CSun* sun)
{
	m_Start = false;
}

CState<CSun>* CPhaseRaise02::Update(CSun* sun)
{
	if (g_Nextflag == true)
	{
		return new CPhaseRaise01();
	}

	if(sun->GetPosition().y<= -18.0f)
		g_Nextflag = true;

	if (m_Start == true)
	{
		m_Move += 0.005f;
		sun->SetPosition(D3DXVECTOR3(sinf(m_Move)*55.0f, cosf(m_Move)*30.0f, 75.0f));
	}

	return this;
}