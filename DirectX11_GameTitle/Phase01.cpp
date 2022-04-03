#include "Phase01.h"
#include "Phase02.h"
#include "input.h"

static bool g_Nextflag = false;
//Sword
void CPhase01::Enter(CSword* obj)
{
	g_Nextflag = false;
	obj->SetPhaseNum(0);
	obj->SetPosition(D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	obj->SetRotation(D3DXVECTOR3(1.571f, 0.0f, 0.0f));
	//�Ō�̍��W�ݒ�
	obj->SetEndPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//���Ԓn�_�̍��W�ݒ�
	obj->SetSavePosition(D3DXVECTOR3(0.0f, 0.0f, 2.25f));
	//�Ō�̌����ݒ�
	obj->SetEndRotation(D3DXVECTOR3(1.571f, 0.0f, 0.0f));
	m_PositionPoint = false;
	
}

void CPhase01::Exit(CSword* obj)
{
	
}

CState<CSword>* CPhase01::Update(CSword* obj)
{
	if (m_PositionPoint == false)
	{

		float rate = 0.003f;
		D3DXVECTOR3 pos;
		//���`��Ԃō��W�����킹��
		obj->RateLerp(&pos, &obj->GetPosition(), &obj->GetSavePosition(), rate);
		obj->SetPosition(pos);

		//���̍�����true
		if (obj->GetPosition().y <= 2.275f)
		{
			m_PositionPoint = true;
		}

		//��]���x�ݒ�
		obj->SetAddRotation(D3DXVECTOR3(0.0f, 0.0f, 0.075f));

	}
	else
	{
		float rate = 1.0f;
		D3DXVECTOR3 rot;
		//��]�ƈړ����~�߂�
		obj->SetAddRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//���`��ԂŐ��ʂɍ��킹��
		obj->RateLerp(&rot, &obj->GetRotation(), &obj->GetEndRotation(), rate);
		obj->SetRotation(rot);

		float posRate = 0.0075f;
		D3DXVECTOR3 pos;
		//���`��Ԃō��W�����킹��
		obj->RateLerp(&pos, &obj->GetPosition(), &obj->GetEndPosition(), posRate);
		obj->SetPosition(pos);

		if (obj->GetRotation().z <= 0.01f&&obj->GetRotation().z >= -0.01f)
		{
			obj->SetAddPosition(D3DXVECTOR3(0.0f, -0.01f, 0.0f));
			if (obj->GetPosition().y <= 0.0f)
			{
				g_Nextflag = true;
				obj->SetAddPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				return new CPhase02();
			}
		}
	}

	return this;
}

//Player
void CPhaseA::Enter(CPlayer* player)
{
	player->m_03_Walk_Idle = false;
	player->m_03_Idle_Arm = false;
	player->m_03_Arm_Back = false;
	player->m_04_Back_Turn = false;
	player->m_04_Run = false;
	player->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -12.0f));
}

void CPhaseA::Exit(CPlayer* player)
{
	
}

CState<CPlayer>* CPhaseA::Update(CPlayer* player)
{
	if (g_Nextflag ==true)//m_NextStep==true
	{
		return new CPhaseB();
	}
	return this;
}

//Sun
void CPhaseRaise01::Enter(CSun* sun)
{
	
}

void CPhaseRaise01::Exit(CSun* sun)
{

}

CState<CSun>* CPhaseRaise01::Update(CSun* sun)
{
	if (g_Nextflag == true)
	{
		return new CPhaseRaise02();
	}
	return this;
}