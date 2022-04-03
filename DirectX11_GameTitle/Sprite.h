#pragma once

#include "gameObject.h"

class CSprite :public CGameObject
{
private:
	static ID3D11Buffer*				m_VertexBuffer;

	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;

	UINT m_width;
	UINT m_height;

	bool m_ON = true;
	int textureId;
	float m_volume=1.0f;
	bool m_volume_flag=false;
	bool m_volume_on_flag = false;
public:

	static void Load();
	static void Unload();

	void SetTextureID(int set_texture) {
		textureId = set_texture;
	}
	void SetSize(float width, float height);
	void SetVolume(bool volume_on_flag);

	void Draw();

	void Init();
	void Uninit();
	void Update();

	void Draw2d(const char* filename, float pos_x, float pos_y);
	void Draw3d(const char* filename,float pos_x,float pos_y,D3DXVECTOR3 Scale);
	
};