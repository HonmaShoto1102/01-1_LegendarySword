#pragma once

#include "main.h"

class CGameObject
{

	//private:	GameObject�̊O������̓A�N�Z�X�ł��Ȃ��Ȃ�
	//public:	�����o�ϐ��͑��̐l���g���Ă��܂���������Ȃ�
protected://	��{private�Ɠ����A�p�����class����̓A�N�Z�X�ł���
	bool m_Destroy = false;

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_EndRotation;
	D3DXVECTOR3 m_EndPosition;
	D3DXVECTOR3 m_SavePosition;
	D3DXVECTOR2 m_Scale2D;
	D3DXVECTOR3 m_Scale3D;
	int m_PhaseNum;

#define Field_limit (100.0f)
#define Rand_max (9999.0f)

public:
	//virtual��t���Ȃ���GameObject��update�������Ăяo�����

	CGameObject() {}//�R���X�g���N�^
	virtual ~CGameObject() {}//�f�X�g���N�^

	virtual void Init() = 0;//�������z�֐�
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR2 GetScale2D() { return m_Scale2D; }
	D3DXVECTOR3 GetScale3D() { return m_Scale3D; }
	D3DXVECTOR3& GetEndRotation() { return m_EndRotation; }
	D3DXVECTOR3& GetEndPosition() { return m_EndPosition; }
	D3DXVECTOR3& GetSavePosition() { return m_SavePosition; }


	void SetPosition(D3DXVECTOR3 Position) { m_Position = Position; }
	void SetRotation(D3DXVECTOR3 Rotation) { m_Rotation = Rotation; }
	void SetAddRotation(D3DXVECTOR3 AddRotation) { m_Rotation += AddRotation; }
	void SetAddPosition(D3DXVECTOR3 AddPosition) { m_Position += AddPosition; }
	void SetScale2D(D3DXVECTOR2 Scale) { m_Scale2D = Scale; }
	void SetScale3D(D3DXVECTOR3 Scale) { m_Scale3D = Scale; }
	void SetEndPosition(D3DXVECTOR3 EndPosition) { m_EndPosition = EndPosition; }
	void SetSavePosition(D3DXVECTOR3 SavePosition) { m_SavePosition = SavePosition; }
	void SetEndRotation(D3DXVECTOR3 EndRotation) { m_EndRotation = EndRotation; }
	void SetDestroy() { m_Destroy = true; }

	D3DXVECTOR3 GetObbx()
	{
		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale, m_Scale3D.x, m_Scale3D.y, m_Scale3D.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		world = scale * rot;

		D3DXVECTOR3 vx;
		vx.x = world._11;
		vx.y = world._12;
		vx.z = world._13;

		return vx * 0.5f;
	}

	D3DXVECTOR3 GetObbz()
	{
		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale, m_Scale3D.x, m_Scale3D.y, m_Scale3D.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 vz;
		vz.x = world._31;
		vz.y = world._32;
		vz.z = world._33;

		return vz * 0.5f;
	}

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	//
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	//t�̒l�������I�Ɏg���Ȑ��I�Ȑ��`���
	D3DXVECTOR3* RateLerp(D3DXVECTOR3* out, D3DXVECTOR3* start, D3DXVECTOR3* end, float t) {
		out->x = start->x + t * (end->x - start->x);
		out->y = start->y + t * (end->y - start->y);
		out->z = start->z + t * (end->z - start->z);
		return out;
	}

	float FloatLerp(float start, float end, float t)
	{
		return (1 - t) * start + t * end;
	}

	int GetPhaseNum() { return m_PhaseNum; }
	void SetPhaseNum(int Num) { m_PhaseNum = Num; }

};