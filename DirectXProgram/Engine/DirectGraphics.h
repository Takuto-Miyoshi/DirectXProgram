#ifndef DIRECT_GRAPHICS
#define DIRECT_GRAPHICS

#include <d3d9.h>
#include <d3dx9.h>

struct CustomVertex {
	float x;
	float y;
	float z;

	float rhw;

	DWORD color;
};

bool InitDirectGraphics(HWND window_handle);

void ReleaseDirectGraphics();

void StartDrawing();
void FinishDrawing();

void DrawPorigon_Triangle();
void DrawPorigon_Rect();

#endif // !DIRECT_GRAPHICS
