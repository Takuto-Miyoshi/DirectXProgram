﻿#include "DirectGraphics.h"

LPDIRECT3D9 g_Interface = nullptr;
IDirect3DDevice9* g_Device = nullptr;
LPDIRECT3DTEXTURE9 g_Textures[TextureID::TexIDMax];

LPCWSTR g_TextureNameList[] = {
	TEXT( "Sprite/Picasso.bmp" )
};

bool InitDirectGraphics(HWND window_handle) {
	g_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_Interface == nullptr) {
		return false; // 初期化失敗
	}

	// デバイスの情報を設定する構造体
	D3DPRESENT_PARAMETERS parameters;

	// データを0で初期化
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	parameters.BackBufferCount = 1;
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;
	parameters.Windowed = true;
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	HRESULT result = g_Interface->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window_handle,
		D3DCREATE_HARDWARE_VERTEXPROCESSING |
		D3DCREATE_MULTITHREADED,
		&parameters,
		&g_Device
	);

	if (FAILED(result)) {
		return false;
	}

	return true;

}

void ReleaseDirectGraphics() {
	if (g_Device != nullptr) {
		g_Device->Release();
		g_Device = nullptr;
	}

	if (g_Interface != nullptr) {
		g_Interface->Release();
		g_Interface = nullptr;
	}
}

void StartDrawing() {

	g_Device->Clear(
		0,
		nullptr,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB( 0, 0, 0 ),
		1.0f,
		0
	);

	g_Device->BeginScene();
}

void FinishDrawing() {

	g_Device->EndScene();

	g_Device->Present( nullptr, nullptr, nullptr, nullptr );
}

void DrawPorigon_Triangle() {
	CustomVertex vertices[] = {
		{ 300, 100, 0, 1, 0xff0000},
		{ 400, 200, 0, 1, 0xff0000},
		{ 200, 200, 0, 1, 0xff0000},
	};

	g_Device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		1,
		vertices,
		sizeof( CustomVertex )
	);
}

void DrawPorigon_Rect() {
	CustomVertex vertices[] = {
		{ 300, 300, 0, 1, 0x00ffff},
		{ 500, 300, 0, 1, 0x0088ff},
		{ 500, 400, 0, 1, 0x00ff88},
		{ 300, 400, 0, 1, 0x008888}
	};

	g_Device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		vertices,
		sizeof( CustomVertex )
	);
}

bool LoadTexture( TextureID tex_id ){
	HRESULT hr = D3DXCreateTextureFromFile(
		g_Device,
		g_TextureNameList[tex_id],
		&g_Textures[tex_id]
	);

	if ( FAILED( hr ) ){
		return false;
	}

	return true;
}

void ReleaseTexture(){
	for ( int i = 0; i < TextureID::TexIDMax; i++ ){
		if ( g_Textures[i] != nullptr ){
			g_Textures[i]->Release();
			g_Textures[i] = nullptr;
		}
	}
}

void DrawTexture( TextureID tex_id ){
	if ( g_Textures[tex_id] == nullptr ){
		return;
	}

	CustomVertex vertices[] = {
		{  0,   0, 0, 1, 0xffffff, 0.0f, 0.0f},
		{100,   0, 0, 1, 0xffffff, 1.0f, 0.0f},
		{100, 100, 0, 1, 0xffffff, 1.0f, 1.0f},
		{  0, 100, 0, 1, 0xffffff, 0.0f, 1.0f}
	};

	g_Device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );

	g_Device->SetTexture( 0, g_Textures[tex_id] );

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		vertices,
		sizeof( CustomVertex )
	);
}
