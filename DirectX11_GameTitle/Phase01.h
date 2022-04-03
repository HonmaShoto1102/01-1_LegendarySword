#pragma once
#include "State.h"
#include "Sword.h"
#include "Player.h"
#include "Sun.h"

class CPhase01 :public CState<CSword>
{
private:
	
	bool m_PositionPoint = false;

public:
	
	//Sword
	void Enter(CSword* obj);
	void Exit(CSword* obj);
	CState<CSword>* Update(CSword* obj);

	
};

class CPhaseA :public CState<CPlayer>
{
private:
	

public:
	
	//Player
	void Enter(CPlayer* player);
	void Exit(CPlayer* player);
	CState<CPlayer>* Update(CPlayer* player);
};

class CPhaseRaise01 :public CState<CSun>
{
private:
	
public:
	//Sun
	void Enter(CSun* sun);
	void Exit(CSun* sun);
	CState<CSun>* Update(CSun* sun);

};