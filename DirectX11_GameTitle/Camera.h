#pragma once

#include "gameObject.h"

class CCamera:public CGameObject
{
private:
	
	D3DXVECTOR3 m_Target;
	D3DXVECTOR3 m_Rotation;
	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjectionMatrix;

public:
	
	void Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
	bool CheckView(D3DXVECTOR3 Position);

};