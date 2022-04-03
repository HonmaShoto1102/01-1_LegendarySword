#pragma once



#include "main.h"

struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};



struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];
};



struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;
};

struct WATER
{
	D3DXVECTOR3 NoiseProjection;
	BOOL		Dummy;
};


class CRenderer//�`��ׂ̈̕ϐ�
{
private:

	//static�͂������cpp�̕��ł��錾���Ȃ��Ă͂����Ȃ�
	static D3D_FEATURE_LEVEL       m_FeatureLevel;//�v���p�e�B�A�����o�ϐ�

	static ID3D11Device*           m_Device;
	//static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*			m_WaterBuffer;
	static ID3D11Buffer*			m_ParameterBuffer;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	
	//�����_�����O�e�N�X�`��1
	static ID3D11Texture2D* m_RendereTexture;
	static ID3D11RenderTargetView* m_TextureRenderTargetView;
	static ID3D11ShaderResourceView* m_TextureShaderResourceView;


	//�����_�����O�e�N�X�`��2
	static ID3D11Texture2D* m_RendereTexture2;
	static ID3D11RenderTargetView* m_TextureRenderTargetView2;
	static ID3D11ShaderResourceView* m_TextureShaderResourceView2;

	//�����_�����O�e�N�X�`��3
	static ID3D11Texture2D* m_RendereTexture3;
	static ID3D11RenderTargetView* m_TextureRenderTargetView3;
	static ID3D11ShaderResourceView* m_TextureShaderResourceView3;

	//�����_�����O�e�N�X�`��4
	static ID3D11Texture2D* m_RendereTexture4;
	static ID3D11RenderTargetView* m_TextureRenderTargetView4;
	static ID3D11ShaderResourceView* m_TextureShaderResourceView4;

	//�t�@�[�V�F�[�_�[�p
	static ID3D11DeviceContext*    m_ImmediateContext;
	static ID3D11RenderTargetView* m_PPRenderTargetView;
	static ID3D11DepthStencilView* m_ShadowDepthStencilView;
	static ID3D11RenderTargetView* m_GeometryRenderTargetView[2];


public:
	static void Init();
	static void Uninit();
	static void Begin();

	static void End();

	static void RendererBegin();
	static void RendererBegin2();
	static void RendererBegin3();
	static void RendererBegin4();

	static void SetDepthEnable(bool Enable);//Set�ݒ肵����
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetWater(WATER Water);
	static void SetParameter(D3DXVECTOR4 Parameter);


	static ID3D11Device* GetDevice( void ){ return m_Device; }//Get�擾������
	//static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }

	static IDXGISwapChain* GetSwapChain(void){ return m_SwapChain; }
	static ID3D11RenderTargetView* GetRenderTargetView(void) { return m_RenderTargetView; }

	//�����_�����O�e�N�X�`��
	static ID3D11ShaderResourceView* GetShaderResourceView() { return m_TextureShaderResourceView; }
	static ID3D11ShaderResourceView* GetShaderResourceView2() { return m_TextureShaderResourceView2; }
	static ID3D11ShaderResourceView* GetShaderResourceView3() { return m_TextureShaderResourceView3; }
	static ID3D11ShaderResourceView* GetShaderResourceView4() { return m_TextureShaderResourceView4; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	//�t�@�[�V�F�[�_�[�p
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_ImmediateContext; }
	//�����_�����O�^�[�Q�b�g���e�N�X�`���֐؂�ւ���֐�
	static void BeginPP()
	{
		//�����_�\�^�[�Q�b�g��Z�o�b�t�@�̃Z�b�g
		m_ImmediateContext->OMSetRenderTargets(1, &m_PPRenderTargetView,
			m_DepthStencilView);
		//�킩��₷���悤�Ƀ����_�\�^�[�Q�b�g��΂ŃN���A���Ă���
		float ClearColor[4] = { 0.0f, 0.5f, 0.0f, 1.0f };
		m_ImmediateContext->ClearRenderTargetView(m_PPRenderTargetView,
			ClearColor);
		//Z�o�b�t�@�̃N���A
		m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView,
			D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	static void BeginDepth()//�V�K�֐��ǉ�
	{
		//�V���h�E�o�b�t�@��[�x�o�b�t�@�ɐݒ肵�A���e��1�œh��Ԃ����Ă���
		m_ImmediateContext->OMSetRenderTargets(0, NULL, m_ShadowDepthStencilView);
		m_ImmediateContext->ClearDepthStencilView(m_ShadowDepthStencilView,
			D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	static void BeginGeometry()
	{
		//�Q�̃e�N�X�`���������_�����O�^�[�Q�b�g�O��1�Ԃɐݒ�
		m_ImmediateContext->OMSetRenderTargets(2, m_GeometryRenderTargetView,
			m_DepthStencilView);
		//�f�v�X�o�b�t�@�ƃe�N�X�`�����Q�Ƃ��N���A
		float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_ImmediateContext->ClearRenderTargetView(m_GeometryRenderTargetView[0],
			ClearColor);
		m_ImmediateContext->ClearRenderTargetView(m_GeometryRenderTargetView[1],
			ClearColor);
		m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH,
			1.0f, 0);
	}
};
