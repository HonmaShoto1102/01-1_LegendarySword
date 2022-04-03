#pragma once

#include "State.h"
#include "Sword.h"
#include "Sun.h"
#include "Player.h"

class CPhase02 :public CState<CSword>
{
private:

public:
	//Sword
	void Enter(CSword* obj);
	void Exit(CSword* obj);
	CState<CSword>* Update(CSword* obj);
	
};

class CPhaseB :public CState<CPlayer>
{
private:

public:
	
	//Player
	void Enter(CPlayer* player);
	void Exit(CPlayer* player);
	CState<CPlayer>* Update(CPlayer* player);

};

class CPhaseRaise02 :public CState<CSun>
{
private:
	float m_Move;
	bool m_Start = false;

public:
	//Sun
	void Enter(CSun* sun);
	void Exit(CSun* sun);
	CState<CSun>* Update(CSun* sun);
	
};