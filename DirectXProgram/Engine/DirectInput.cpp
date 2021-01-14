
#include "DirectInput.h"

LPDIRECTINPUT8 g_InputInterface = nullptr;
LPDIRECTINPUTDEVICE8 g_InputKeyboardDevice = nullptr;

InputState g_KeyStates[256];

InputState UpdateInputState( bool isPush_, InputState state );

// isPush_ 現在の入力状態, state 前回の入力状態
InputState UpdateInputState( bool isPush_, InputState state ){
	if( isPush_ == true ){
		if( state == InputState::NoHold ){
			return InputState::Pushed;
		}
		else if( state == InputState::Pushed || state == InputState::Held ){
			return InputState::Held;
		}
	}
	else{
		if( state == InputState::Pushed || state == InputState::Held ){
			return InputState::Released;
		}
	}
	return InputState::NoHold;
}

bool InitDirectInput(){
	HRESULT hr = DirectInput8Create(
		GetModuleHandle( nullptr ),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		( void** ) &g_InputInterface,
		nullptr
	);

	if ( FAILED( hr ) ) return false;

	hr = g_InputInterface->CreateDevice(
		GUID_SysKeyboard,
		&g_InputKeyboardDevice,
		nullptr
	);

	if ( FAILED( hr ) ) return false;

	hr = g_InputKeyboardDevice->SetDataFormat( &c_dfDIKeyboard );

	if ( FAILED( hr ) ) return false;

	hr = g_InputKeyboardDevice->SetCooperativeLevel(
		FindWindow( TEXT( "WindowClass" ), nullptr ),
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE
	);

	if ( FAILED( hr ) ) return false;

	hr = g_InputKeyboardDevice->Acquire();

	if ( FAILED( hr ) ) return false;

	return true;
}

void UpdateDirectInput(){
	BYTE key_states[256];

	HRESULT hr = g_InputKeyboardDevice->GetDeviceState(
		256,
		key_states
	);

	if ( SUCCEEDED( hr ) ){
		for ( int i = 0; i < 256; i++ ){
			bool isPush = false;

			if ( key_states[i] & 0x80 ){
				isPush = true;
			}

			g_KeyStates[i] = UpdateInputState( isPush, g_KeyStates[i] );
		}
	}
}

void ReleaseDirectInput(){
	if ( g_InputKeyboardDevice != nullptr ){
		g_InputKeyboardDevice->Release();
		g_InputKeyboardDevice = nullptr;
	}

	if ( g_InputInterface != nullptr ){
		g_InputInterface->Release();
		g_InputInterface = nullptr;
	}
}

bool IsKeyHeld( int key ) {
	if( key < 0 || key >= 256 )return false;

	return g_KeyStates[key] == InputState::Held ? true : false;
}

bool IsKeyPushed( int key ){
	if( key < 0 || key >= 256 )return false;

	return g_KeyStates[key] == InputState::Pushed ? true : false;
}

bool IsKeyReleased( int key ){
	if( key < 0 || key >= 256 )return false;

	return g_KeyStates[key] == InputState::Released ? true : false;
}
