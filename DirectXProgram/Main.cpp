#include <Windows.h>
#include "Engine/DirectGraphics.h"

// �E�B���h�E�v���V�[�W���̍쐬
LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam) {
	return DefWindowProc(window_handle, message_id, wparam, lparam);
}

// ���C���֐�
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmpLine, INT nCmdShow) {

	// �E�B���h�E������
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

	// �E�B���h�E�o�^
	if (RegisterClassA(&window_class) == 0) {
		return 0;
	}

	int width = 640;
	int height = 480;
	// �E�B���h�E�쐬
	HWND window_handle = CreateWindowA(
		"WindowClass",
		"�E�B���h�E�쐬�ۑ�",
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

	// �E�B���h�E�̕\��
	ShowWindow(window_handle, SW_SHOW);

	if (InitDirectGraphics(window_handle) == false) {
		return 0;
	}

	// ���C�����[�v
	while (true) {
		MSG message;
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
			if (message.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else {
			// �Q�[������
		}
	}

	if (InitDirectGraphics(window_handle) == false) {
		return 0;
	}

	return 0;
}