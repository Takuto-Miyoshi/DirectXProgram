#ifndef DIRECT_GRAPHICS
#define DIRECT_GRAPHICS

#include <d3d9.h>
#include <d3dx9.h>

bool InitDirectGraphics(HWND window_handle);

void ReleaseDirectGraphics();

#endif // !DIRECT_GRAPHICS