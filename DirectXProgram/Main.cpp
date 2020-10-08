#include <Windows.h>

// ウィンドウプロシージャの作成
LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam){
	return DefWindowProc(window_handle, message_id, wparam, lparam);
}

// メイン関数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmpLine, INT nCmdShow){

	// ウィンドウ初期化
	WNDCLASSA window_class = {
		CS_HREDRAW | CS_VREDRAW,
		WindowProcedure,
		0,
		0,
		hInstance,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		"WindowClass",
	};

	// ウィンドウ登録
	if (RegisterClassA(&window_class) == 0) {
		return 0;
	}

	int width = 640;
	int height = 480;
	// ウィンドウ作成
	HWND window_handle = CreateWindowA(
		"WindowClass",
		"ウィンドウ作成課題",
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// ウィンドウの表示
	ShowWindow(window_handle, SW_SHOW);

	// メインループ
	while (true) {
		MSG message;
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {

			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else {
			// ゲーム処理
		}
	}
}