#pragma once

#include "State.h"
#include "Sword.h"
#include "Player.h"

class CPhase03 :public CState<CSword>
{
private:
	float m_Vibration;
	float m_Counter;
public:
	//Sword
	void Enter(CSword* obj);
	void Exit(CSword* obj);
	CState<CSword>* Update(CSword* obj);

};


class CPhaseC :public CState<CPlayer>
{
private:
	int m_AnimationCount;
	bool m_AnimationFlag;
	
	void BoolSwitch(CPlayer* player, int boolSwitch);
public:
	
	//Player
	void Enter(CPlayer* player);
	void Exit(CPlayer* player);
	CState<CPlayer>* Update(CPlayer* player);

};
