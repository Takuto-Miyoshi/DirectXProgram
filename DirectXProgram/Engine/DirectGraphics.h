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

	float tu;
	float tv;
};

enum TextureID{
	TexIDPlayer,
	TexIDMax
};

bool InitDirectGraphics(HWND window_handle);

void ReleaseDirectGraphics();

void StartDrawing();
void FinishDrawing();

void DrawPorigon_Triangle();
void DrawPorigon_Rect();
void DrawTexture( TextureID tex_id );

bool LoadTexture( TextureID tex_id );

void ReleaseTexture();

#endif // !DIRECT_GRAPHICS
