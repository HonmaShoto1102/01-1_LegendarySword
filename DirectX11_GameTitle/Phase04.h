#pragma once

#include "State.h"
#include "Sword.h"
#include "Player.h"

class CPhase04 :public CState<CSword>
{
private:
	float m_Vibration;
	float m_Counter;

	WATER time;
public:
	//Sword
	void Enter(CSword* obj);
	void Exit(CSword* obj);
	CState<CSword>* Update(CSword* obj);
	
};

class CPhaseD : public CState<CPlayer>
{
private:
	int m_AnimationCount;
	void BoolSwitch(CPlayer* player, int boolSwitch);
public:
	
	//Player
	void Enter(CPlayer* player);
	void Exit(CPlayer* player);
	CState<CPlayer>* Update(CPlayer* player);
};