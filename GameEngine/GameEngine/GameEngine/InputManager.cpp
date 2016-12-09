#pragma once

#include <Windows.h>
#include "InputManager.h"

InputManager* InputManager::s_instance = nullptr;

void InputManager::keyPressed(KeyCode keyCode)
{
	keyBuffer.insert(keyCode);
}

void InputManager::keyReleased(KeyCode keyCode)
{
	keyBuffer.erase(keyCode);
}

void InputManager::reset()
{
	// Iterate through supported keyCodes
	for (int keyCodeInt = KEY_A; keyCodeInt != LAST; keyCodeInt++) {
		KeyCode keyCode = static_cast<KeyCode>(keyCodeInt);
		reset(keyCode);
	}
}

void InputManager::reset(KeyCode keyCode)
{
	// Check if the key is pressed
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646293(v=vs.85).aspx
	if (keyBuffer.count(keyCode)) {

		// Check if key had already been pressed in the previous frame
		if (!getKey(keyCode)) {
			setKeyDown(keyCode, true);
		}
		else {
			setKeyDown(keyCode, false);
		}
		setKey(keyCode, true);

	}
	else {

		setKey(keyCode, false);
		setKeyDown(keyCode, false);

	}
}

bool InputManager::getKeyDown(KeyCode key)
{
	if (!keyDownMap.count(key)) return false;
	else return keyDownMap.at(key);
}
bool InputManager::getKey(KeyCode key)
{
	if (!keyMap.count(key)) return false;
	else return keyMap.at(key);
}
void InputManager::setKeyDown(KeyCode key, bool val)
{
	keyDownMap.insert_or_assign(key, val);
}

void InputManager::setKey(KeyCode key, bool val)
{
	keyMap.insert_or_assign(key, val);
}

KeyCode InputManager::convertVirtualKeyCode(long virtualKeyCode)
{
	switch (virtualKeyCode) {
		case 0x30:
			return KEY_0;
		case 0x31:
			return KEY_1;
		case 0x32:
			return KEY_2;
		case 0x33:
			return KEY_3;
		case 0x34:
			return KEY_4;
		case 0x35:
			return KEY_5;
		case 0x36:
			return KEY_6;
		case 0x37:
			return KEY_7;
		case 0x38:
			return KEY_8;
		case 0x39:
			return KEY_9;
		case 0x41:
			return KEY_A;
		case 0x42:
			return KEY_B;
		case 0x43:
			return KEY_C;
		case 0x44:
			return KEY_D;
		case 0x45:
			return KEY_E;
		case 0x46:
			return KEY_F;
		case 0x47:
			return KEY_G;
		case 0x48:
			return KEY_H;
		case 0x49:
			return KEY_I;
		case 0x4A:
			return KEY_J;
		case 0x4B:
			return KEY_K;
		case 0x4C:
			return KEY_L;
		case 0x4D:
			return KEY_M;
		case 0x4E:
			return KEY_N;
		case 0x4F:
			return KEY_O;
		case 0x50:
			return KEY_P;
		case 0x51:
			return KEY_Q;
		case 0x52:
			return KEY_R;
		case 0x53:
			return KEY_S;
		case 0x54:
			return KEY_T;
		case 0x55:
			return KEY_U;
		case 0x56:
			return KEY_V;
		case 0x57:
			return KEY_W;
		case 0x58:
			return KEY_X;
		case 0x59:
			return KEY_Y;
		case 0x5A:
			return KEY_Z;
		case VK_UP:
			return KEY_ARROW_UP;
		case VK_DOWN:
			return KEY_ARROW_DOWN;
		case VK_LEFT:
			return KEY_ARROW_LEFT;
		case VK_RIGHT:
			return KEY_ARROW_RIGHT;
		default:
			return KEY_UNDEFINED;
	}
}

long InputManager::convertKeyCode(KeyCode keyCode)
{
	switch (keyCode) {
	case KEY_0:
		return 0x30;
	case KEY_1:
		return 0x31;
	case KEY_2:
		return 0x32;
	case KEY_3:
		return 0x33;
	case KEY_4:
		return 0x34;
	case KEY_5:
		return 0x35;
	case KEY_6:
		return 0x36;
	case KEY_7:
		return 0x37;
	case KEY_8:
		return 0x38;
	case KEY_9:
		return 0x39;
	case KEY_A:
		return 0x41;
	case KEY_B:
		return 0x42;
	case KEY_C:
		return 0x43;
	case KEY_D:
		return 0x44;
	case KEY_E:
		return 0x45;
	case KEY_F:
		return 0x46;
	case KEY_G:
		return 0x47;
	case KEY_H:
		return 0x48;
	case KEY_I:
		return 0x49;
	case KEY_J:
		return 0x4A;
	case KEY_K:
		return 0x4B;
	case KEY_L:
		return 0x4C;
	case KEY_M:
		return 0x4D;
	case KEY_N:
		return 0x4E;
	case KEY_O:
		return 0x4F;
	case KEY_P:
		return 0x50;
	case KEY_Q:
		return 0x51;
	case KEY_R:
		return 0x52;
	case KEY_S:
		return 0x53;
	case KEY_T:
		return 0x54;
	case KEY_U:
		return 0x55;
	case KEY_V:
		return 0x56;
	case KEY_W:
		return 0x57;
	case KEY_X:
		return 0x58;
	case KEY_Y:
		return 0x59;
	case KEY_Z:
		return 0x5A;
	case KEY_ARROW_UP:
		return VK_UP;
	case KEY_ARROW_DOWN:
		return VK_DOWN;
	case KEY_ARROW_LEFT :
		return VK_LEFT;
	case KEY_ARROW_RIGHT :
		return VK_RIGHT;
	default:
		return 0;
	}
}
