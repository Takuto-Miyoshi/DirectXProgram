#include "DirectGraphics.h"

LPDIRECT3D9 g_Interface = nullptr;
IDirect3DDevice9* g_Device = nullptr;

bool InitDirectGraphics(HWND window_handle) {
	g_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_Interface == nullptr) {
		return false; // 初期化失敗
	}

	return true;

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