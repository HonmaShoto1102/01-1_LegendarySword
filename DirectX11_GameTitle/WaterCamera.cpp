#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "model.h"
#include "WaterCamera.h"
#include "Scene.h"


void CWaterCamera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 1.6f, -10.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


void CWaterCamera::Uninit()
{

}

void CWaterCamera::Update()
{

}

void CWaterCamera::Draw()
{

	//ビューマトリクス設定

	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

	CRenderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);//ニアとファー

	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

bool CWaterCamera::CheckView(D3DXVECTOR3 Position)
{
	D3DXMATRIX vp, invvp;

	vp = m_ViewMatrix * m_ProjectionMatrix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vpos[4];
	D3DXVECTOR3 wpos[4];

	vpos[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vpos[1] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vpos[2] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vpos[3] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	D3DXVECTOR3 v, v1, v2, v3, v4, n, n1;

	v = Position - m_Position;

	v1 = wpos[0] - m_Position;
	v2 = wpos[2] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);

	v3 = wpos[1] - m_Position;
	v4 = wpos[3] - m_Position;
	D3DXVec3Cross(&n1, &v3, &v4);


	if (D3DXVec3Dot(&n, &v) < 0.0f)
		return false;

	if (D3DXVec3Dot(&n1, &v) > 0.0f)
		return false;

	//ニアクリップ

	return true;
}