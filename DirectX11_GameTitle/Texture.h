#pragma once

#include "gameObject.h"
#include <unordered_map>
#include <d3d11.h>
#include <string>

class CTextureDate
{
private:
	ID3D11ShaderResourceView* m_Texture;
	UINT m_Width;
	UINT m_Height;

public:
	ID3D11ShaderResourceView* Get_m_Texture() { return m_Texture; }
	UINT Get_m_Width() { return m_Width; }
	UINT Get_m_Height() { return m_Height; }

	CTextureDate() = delete;
	CTextureDate(ID3D11ShaderResourceView* _Texture, UINT _Width, UINT _Height) {
		m_Texture = _Texture;
		m_Height = _Height;
		m_Width = _Width;
	}
};

class CTexture :public CGameObject
{
private:
	std::unordered_map<int, CTextureDate*>m_Texture;

	
public:

	void Load();
	void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();
	
	CTextureDate* Texture_GetTexture(int textureId);
};
