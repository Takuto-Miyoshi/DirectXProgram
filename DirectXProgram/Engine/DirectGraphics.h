#ifndef DIRECT_GRAPHICS
#define DIRECT_GRAPHICS

#include <d3d9.h>
#include <d3dx9.h>

#define FVF_2D_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
#define FVF_2D_VERTEX_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_3D_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CustomVertex {
	float x;
	float y;
	float z;

	float rhw;

	DWORD color;

	float tu;
	float tv;
};

struct CustomVertex3D{
	float x;
	float y;
	float z;

	DWORD color;
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

void SetUpView();

void SetUpProjection();

void Draw3DPorigon( CustomVertex3D top1, CustomVertex3D top2, CustomVertex3D top3 );

void ReleaseTexture();

bool LoadXFile( LPCWSTR fileName_ );

void DrawXFile( float posX_, float posY_, float posZ_, float radX_, float radY_, float radZ_, float scaleX_, float scaleY_, float scaleZ_ );

void ReleaseXFile();

#endif // !DIRECT_GRAPHICS
