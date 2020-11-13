
#ifndef DIRECT_INPUT_H
#define DIRECT_INPUT_H

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

bool InitDirectInput();

void UpdateDirectInput();
bool IsKeyHeld( int key );

void ReleaseDirectInput();

#endif // DIRECT_INPUT_H
