#include "main.h"
#include "renderer.h"
#include "Texture.h"




struct Texture_tag
{
	int textureID;
	std::string Filepath;
	
};

const Texture_tag TexturePath[]
{
	{0,"asset/texture/title.png"},
	{1,"asset/texture/enter.png"},
	{2,"asset/texture/result.png"},
	//{3,"asset/texture/ball.png"},

};

void CTexture::Init() {

}

void CTexture::Update() {

}

void CTexture::Draw() {

}

void CTexture::Uninit() {

}

void CTexture::Load()
{
	for (const Texture_tag &file : TexturePath)
	{
		ID3D11ShaderResourceView* p_Texture = NULL;
		
		D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(), file.Filepath.c_str(), NULL, NULL, &p_Texture, NULL);
		assert(p_Texture);

		//リソースの詳細を取得
		ID3D11Texture2D* tex = nullptr;
		p_Texture->GetResource(reinterpret_cast<ID3D11Resource**>(&tex));

		//解像度を取得する
		D3D11_TEXTURE2D_DESC desc = {};
		tex->GetDesc(&desc);

		CTextureDate* date = new CTextureDate(p_Texture, desc.Width, desc.Height);



		m_Texture[file.textureID] = date;
	}
	

}

CTextureDate* CTexture::Texture_GetTexture(int textureId)
{
	if (m_Texture.count(textureId) == 0)
	{
		assert(textureId);
	}
	return m_Texture[textureId];
}