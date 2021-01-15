#include <Windows.h>
#include "Engine/DirectGraphics.h"
#include "Engine/DirectInput.h"

// ウィンドウプロシージャの作成
LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam) {
	switch( message_id )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( window_handle, message_id, wparam, lparam );
		break;
	}
	return 0;
}

// メイン関数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmpLine, INT nCmdShow) {

	// ウィンドウ初期化
	WNDCLASS window_class = {
		CS_HREDRAW | CS_VREDRAW,
		WindowProcedure,
		0,
		0,
		hInstance,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		TEXT("WindowClass"),
	};

	// ウィンドウ登録
	if (RegisterClass(&window_class) == 0) {
		return 0;
	}

	int width = 640;
	int height = 480;
	// ウィンドウ作成
	HWND window_handle = CreateWindow(
		TEXT("WindowClass"),
		TEXT("ウィンドウ作成課題"),
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

	RECT window_rect;
	if (GetWindowRect(window_handle, &window_rect) == false) {
		return 0;
	}

	RECT client_rect;
	if (GetClientRect(window_handle, &client_rect) == false) {
		return 0;
	}

	int flame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int flame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	int resize_width = flame_size_x + width;
	int resize_height = flame_size_y + height;

	SetWindowPos(
		window_handle,
		nullptr,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		resize_width,
		resize_height,
		SWP_NOMOVE
	);

	// ウィンドウの表示
	ShowWindow(window_handle, SW_SHOW);

	if (InitDirectGraphics(window_handle) == false) {
		return 0;
	}

	if ( InitDirectInput() == false ){
		return 0;
	}

	if ( LoadTexture( TextureID::TexIDPlayer ) == false ){
		return 0;
	}

	if( LoadXFile( TEXT( "Witchwait.x" ) ) == false ) return 0;

	float angle = 0.0f;
	float posX = 0.0f;
	float posY = 0.0f;
	float posZ = 0.0f;

	// メインループ
	while( true ) {
		MSG message;
		if( PeekMessage( &message, nullptr, 0, 0, PM_REMOVE ) ) {
			if( message.message == WM_QUIT ) {
				break;
			}

			TranslateMessage( &message );
			DispatchMessage( &message );
		}
		else {
			// ゲーム処理

			UpdateDirectInput();

			if( IsKeyHeld( DIK_LEFTARROW ) ){
				angle -= 3.0f;
			}
			else if( IsKeyHeld( DIK_RIGHTARROW ) ){
				angle += 3.0f;
			}

			if( IsKeyHeld( DIK_UPARROW ) ){
				float vecZ = cosf( D3DXToRadian( angle ) );
				float vecX = sinf( D3DXToRadian( angle ) );
				float speed = 2.0f;

				posX += vecX * speed;
				posZ += vecZ * speed;
			}

			if( IsKeyHeld( DIK_SPACE ) ){
				UpdateTPSCamera( posX, posY, posZ, angle );
			}
			else{
				SetUpView();
			}

			StartDrawing();
			SetUpProjection();

			// ----- 3D -------

			DrawXFile( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f );

			//Draw3DPorigon(
			//	{ -5.0f, -5.0f, 0.0f, 0xffff0000 },
			//	{ 0.0f,  5.0f, 0.0f, 0xffff0000 },
			//	{ 5.0f, -5.0f, 0.0f, 0xffff0000 }
			//);

			// ----- 2D -----

			//DrawTexture( TextureID::TexIDPlayer );

			FinishDrawing();

			// ---------------
		}
	}

	ReleaseTexture();
	ReleaseXFile();
	ReleaseDirectInput();
	ReleaseDirectGraphics();

	return 0;
}
