
#ifndef DIRECT_INPUT_H
#define DIRECT_INPUT_H

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

enum InputState{
	NoHold,
	Pushed,
	Held,
	Released
};

bool InitDirectInput();

void UpdateDirectInput();

void ReleaseDirectInput();

bool IsKeyHeld( int key );
bool IsKeyPushed( int key );
bool IsKeyReleased( int key );

#endif // DIRECT_INPUT_H
