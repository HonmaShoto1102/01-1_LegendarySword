

#include "renderer.h"
#include <io.h>


D3D_FEATURE_LEVEL       CRenderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           CRenderer::m_Device = NULL;
//ID3D11DeviceContext*    CRenderer::m_DeviceContext = NULL;
IDXGISwapChain*         CRenderer::m_SwapChain = NULL;
ID3D11RenderTargetView* CRenderer::m_RenderTargetView = NULL;
ID3D11DepthStencilView* CRenderer::m_DepthStencilView = NULL;

ID3D11Buffer*			CRenderer::m_WorldBuffer = NULL;
ID3D11Buffer*			CRenderer::m_ViewBuffer = NULL;
ID3D11Buffer*			CRenderer::m_ProjectionBuffer = NULL;
ID3D11Buffer*			CRenderer::m_MaterialBuffer = NULL;
ID3D11Buffer*			CRenderer::m_LightBuffer = NULL;
ID3D11Buffer*			CRenderer::m_WaterBuffer = NULL;
ID3D11Buffer*			CRenderer::m_ParameterBuffer = NULL;

ID3D11DepthStencilState* CRenderer::m_DepthStateEnable = NULL;
ID3D11DepthStencilState* CRenderer::m_DepthStateDisable = NULL;

//レンダリングテクスチャ1
ID3D11RenderTargetView* CRenderer::m_TextureRenderTargetView = NULL;
ID3D11ShaderResourceView* CRenderer::m_TextureShaderResourceView = NULL;
ID3D11Texture2D* CRenderer::m_RendereTexture = NULL;

//レンダリングテクスチャ2
ID3D11RenderTargetView* CRenderer::m_TextureRenderTargetView2 = NULL;
ID3D11ShaderResourceView* CRenderer::m_TextureShaderResourceView2 = NULL;
ID3D11Texture2D* CRenderer::m_RendereTexture2 = NULL;

//レンダリングテクスチャ3
ID3D11RenderTargetView* CRenderer::m_TextureRenderTargetView3 = NULL;
ID3D11ShaderResourceView* CRenderer::m_TextureShaderResourceView3 = NULL;
ID3D11Texture2D* CRenderer::m_RendereTexture3 = NULL;

//レンダリングテクスチャ4
ID3D11RenderTargetView* CRenderer::m_TextureRenderTargetView4 = NULL;
ID3D11ShaderResourceView* CRenderer::m_TextureShaderResourceView4 = NULL;
ID3D11Texture2D* CRenderer::m_RendereTexture4 = NULL;

//ファーシェーダー用
ID3D11DeviceContext*    CRenderer::m_ImmediateContext = NULL;
ID3D11RenderTargetView* CRenderer::m_PPRenderTargetView = NULL;
ID3D11DepthStencilView* CRenderer::m_ShadowDepthStencilView = NULL;
ID3D11RenderTargetView* CRenderer::m_GeometryRenderTargetView[2] = { NULL };

void CRenderer::Init()
{
	HRESULT hr = S_OK;




	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};//swapChainはフロントバッファ、バッグバッファの切り替え
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();
	swapChainDesc.SampleDesc.Count = 1;//16or1
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_SwapChain,
		&m_Device,
		&m_FeatureLevel,
		&m_ImmediateContext);






	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget = NULL;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	m_Device->CreateRenderTargetView(renderTarget, NULL, &m_RenderTargetView);
	renderTarget->Release();


	// デプスステンシルバッファ作成		深度バッファとも言われる
	ID3D11Texture2D* depthStencile = NULL;
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Flags = 0;
	m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
	depthStencile->Release();


	m_ImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);//NULL(),or,m_DepthStencilView





	// ビューポート設定		画面の何処に描画するのか
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)SCREEN_WIDTH;
	viewport.Height = (FLOAT)SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports(1, &viewport);



	// ラスタライザステート設定		描画の設定、塗りつぶすのか
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	m_Device->CreateRasterizerState(&rasterizerDesc, &rs);

	m_ImmediateContext->RSSetState(rs);




	// ブレンドステート設定		αブレンドの設定
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = TRUE;//半透明オブジェクトには使えない
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;//D3D11_BLEND_SRC_ALPHA,D3D11_BLEND_SRC_ALPHA,D3D11_BLEND_INV_DEST_COLOR,
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//D3D11_BLEND_INV_SRC_ALPHA,D3D11_BLEND_ONE,D3D11_BLEND_ONE,D3D11_BLEND_DEST_COLOR
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_Device->CreateBlendState(&blendDesc, &blendState);
	m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);



	// デプスステンシルステート設定		Zバッファ、深度バッファ
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//深度無効ステート

	m_ImmediateContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);




	// サンプラーステート設定		テクスチャをどう張り付けるのか
	D3D11_SAMPLER_DESC samplerDesc{};
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MaxAnisotropy = 16;

	ID3D11SamplerState* samplerState = NULL;
	m_Device->CreateSamplerState(&samplerDesc, &samplerState);

	m_ImmediateContext->PSSetSamplers(0, 1, &samplerState);



	// 定数バッファ生成		シェーダーに
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_WorldBuffer);
	m_ImmediateContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ViewBuffer);
	m_ImmediateContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ProjectionBuffer);
	m_ImmediateContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_MaterialBuffer);
	m_ImmediateContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);


	bufferDesc.ByteWidth = sizeof(LIGHT);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_LightBuffer);
	m_ImmediateContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
	m_ImmediateContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);

	bufferDesc.ByteWidth = sizeof(WATER);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_WaterBuffer);
	m_ImmediateContext->VSSetConstantBuffers(5, 1, &m_WaterBuffer);
	m_ImmediateContext->PSSetConstantBuffers(5, 1, &m_WaterBuffer);

	bufferDesc.ByteWidth = sizeof(D3DXVECTOR4);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ParameterBuffer);
	m_ImmediateContext->VSSetConstantBuffers(6, 1, &m_ParameterBuffer);
	m_ImmediateContext->PSSetConstantBuffers(6, 1, &m_ParameterBuffer);



	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetLight(light);



	// マテリアル初期化
	MATERIAL material{};
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);
	{//1
		//レンダリングテクスチャ初期化
		D3D11_TEXTURE2D_DESC td;//テクスチャ作成用デスクリプタ構造体変数
		ZeroMemory(&td, sizeof(td)); //構造体を初期化
		td.Width = swapChainDesc.BufferDesc.Width;
		td.Height = swapChainDesc.BufferDesc.Height;
		td.MipLevels = 1;//ミップマップの数
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//ピクセル
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;
		//使用法のフラグを設定
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を生成
		m_Device->CreateTexture2D(&td, NULL, &m_RendereTexture);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		m_Device->CreateRenderTargetView(m_RendereTexture, &rtvd, &m_TextureRenderTargetView);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(m_RendereTexture, &srvd, &m_TextureShaderResourceView);

	}

	{//2
		//レンダリングテクスチャ初期化
		D3D11_TEXTURE2D_DESC td2;//テクスチャ作成用デスクリプタ構造体変数
		ZeroMemory(&td2, sizeof(td2)); //構造体を初期化
		td2.Width = swapChainDesc.BufferDesc.Width;
		td2.Height = swapChainDesc.BufferDesc.Height;
		td2.MipLevels = 1;//ミップマップの数
		td2.ArraySize = 1;
		td2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//ピクセル
		td2.SampleDesc = swapChainDesc.SampleDesc;
		td2.Usage = D3D11_USAGE_DEFAULT;
		//使用法のフラグを設定
		td2.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td2.CPUAccessFlags = 0;
		td2.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を生成
		m_Device->CreateTexture2D(&td2, NULL, &m_RendereTexture2);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd2;
		ZeroMemory(&rtvd2, sizeof(rtvd2));
		rtvd2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd2.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		m_Device->CreateRenderTargetView(m_RendereTexture2, &rtvd2, &m_TextureRenderTargetView2);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd2;
		ZeroMemory(&srvd2, sizeof(srvd2));
		srvd2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		srvd2.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(m_RendereTexture2, &srvd2, &m_TextureShaderResourceView2);
	}

	{//3
		//レンダリングテクスチャ初期化
		D3D11_TEXTURE2D_DESC td3;//テクスチャ作成用デスクリプタ構造体変数
		ZeroMemory(&td3, sizeof(td3)); //構造体を初期化
		td3.Width = swapChainDesc.BufferDesc.Width;
		td3.Height = swapChainDesc.BufferDesc.Height;
		td3.MipLevels = 1;//ミップマップの数
		td3.ArraySize = 1;
		td3.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//ピクセル
		td3.SampleDesc = swapChainDesc.SampleDesc;
		td3.Usage = D3D11_USAGE_DEFAULT;
		//使用法のフラグを設定
		td3.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td3.CPUAccessFlags = 0;
		td3.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を生成
		m_Device->CreateTexture2D(&td3, NULL, &m_RendereTexture3);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd3;
		ZeroMemory(&rtvd3, sizeof(rtvd3));
		rtvd3.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd3.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		m_Device->CreateRenderTargetView(m_RendereTexture3, &rtvd3, &m_TextureRenderTargetView3);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd3;
		ZeroMemory(&srvd3, sizeof(srvd3));
		srvd3.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd3.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		srvd3.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(m_RendereTexture3, &srvd3, &m_TextureShaderResourceView3);
	}

	{//4
		//レンダリングテクスチャ初期化
		D3D11_TEXTURE2D_DESC td4;//テクスチャ作成用デスクリプタ構造体変数
		ZeroMemory(&td4, sizeof(td4)); //構造体を初期化
		td4.Width = swapChainDesc.BufferDesc.Width;
		td4.Height = swapChainDesc.BufferDesc.Height;
		td4.MipLevels = 1;//ミップマップの数
		td4.ArraySize = 1;
		td4.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//ピクセル
		td4.SampleDesc = swapChainDesc.SampleDesc;
		td4.Usage = D3D11_USAGE_DEFAULT;
		//使用法のフラグを設定
		td4.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td4.CPUAccessFlags = 0;
		td4.MiscFlags = 0;
		//構造体の設定に従ってテクスチャ領域を生成
		m_Device->CreateTexture2D(&td4, NULL, &m_RendereTexture4);

		//レンダーターゲットビュー作成
		D3D11_RENDER_TARGET_VIEW_DESC rtvd4;
		ZeroMemory(&rtvd4, sizeof(rtvd4));
		rtvd4.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd4.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		m_Device->CreateRenderTargetView(m_RendereTexture4, &rtvd4, &m_TextureRenderTargetView4);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd4;
		ZeroMemory(&srvd4, sizeof(srvd4));
		srvd4.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd4.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		srvd4.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(m_RendereTexture4, &srvd4, &m_TextureShaderResourceView4);
	}

}



void CRenderer::Uninit()
{

	m_WorldBuffer->Release();//必要なくなったメソッドはRelease()を
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();


	m_ImmediateContext->ClearState();
	m_RenderTargetView->Release();
	m_SwapChain->Release();
	m_ImmediateContext->Release();
	m_Device->Release();

}




void CRenderer::Begin()//描画の開始
{
	// バックバッファクリア
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//R,G,B,A
	m_ImmediateContext->ClearRenderTargetView( m_RenderTargetView, clearColor );
	m_ImmediateContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//デフォルトのバックバッファと深度バッファへ復帰させておく
	m_ImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
}



void CRenderer::End()//描画の終わり
{
	m_SwapChain->Present( 1, 0 );
}


void CRenderer::RendererBegin()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//R,G,B,A
	m_ImmediateContext->ClearRenderTargetView(m_TextureRenderTargetView, clearColor);
	m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_ImmediateContext->OMSetRenderTargets(1, &m_TextureRenderTargetView, m_DepthStencilView);
}


void CRenderer::RendererBegin2()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//R,G,B,A
	m_ImmediateContext->ClearRenderTargetView(m_TextureRenderTargetView2, clearColor);
	m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_ImmediateContext->OMSetRenderTargets(1, &m_TextureRenderTargetView2, m_DepthStencilView);
}

void CRenderer::RendererBegin3()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//R,G,B,A
	m_ImmediateContext->ClearRenderTargetView(m_TextureRenderTargetView3, clearColor);
	m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_ImmediateContext->OMSetRenderTargets(1, &m_TextureRenderTargetView3, m_DepthStencilView);
}

void CRenderer::RendererBegin4()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//R,G,B,A
	m_ImmediateContext->ClearRenderTargetView(m_TextureRenderTargetView4, clearColor);
	m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_ImmediateContext->OMSetRenderTargets(1, &m_TextureRenderTargetView4, m_DepthStencilView);
}


void CRenderer::SetDepthEnable( bool Enable )
{
	if( Enable )
		m_ImmediateContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );
	else
		m_ImmediateContext->OMSetDepthStencilState( m_DepthStateDisable, NULL );

}

void CRenderer::SetWorldViewProjection2D()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	m_ImmediateContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	m_ImmediateContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&projection, &projection);
	m_ImmediateContext->UpdateSubresource( m_ProjectionBuffer, 0, NULL, &projection, 0, 0 );

}


void CRenderer::SetWorldMatrix( D3DXMATRIX* WorldMatrix )
{
	D3DXMATRIX world;
	D3DXMatrixTranspose(&world, WorldMatrix);
	m_ImmediateContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
}

void CRenderer::SetViewMatrix( D3DXMATRIX* ViewMatrix )
{
	D3DXMATRIX view;
	D3DXMatrixTranspose(&view, ViewMatrix);
	m_ImmediateContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
}

void CRenderer::SetProjectionMatrix( D3DXMATRIX* ProjectionMatrix )
{
	D3DXMATRIX projection;
	D3DXMatrixTranspose(&projection, ProjectionMatrix);
	m_ImmediateContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}



void CRenderer::SetMaterial( MATERIAL Material )
{
	m_ImmediateContext->UpdateSubresource( m_MaterialBuffer, 0, NULL, &Material, 0, 0 );
}

void CRenderer::SetLight( LIGHT Light )
{
	m_ImmediateContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
}

void CRenderer::SetWater(WATER Water)
{
	m_ImmediateContext->UpdateSubresource(m_WaterBuffer, 0, NULL, &Water, 0, 0);
}

void CRenderer::SetParameter(D3DXVECTOR4 Parameter)
{
	m_ImmediateContext->UpdateSubresource(m_ParameterBuffer, 0, NULL, &Parameter, 0, 0);
}

void CRenderer::CreateVertexShader( ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName )
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}

void CRenderer::CreatePixelShader( ID3D11PixelShader** PixelShader, const char* FileName )
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}


