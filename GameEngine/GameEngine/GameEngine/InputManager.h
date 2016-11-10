#pragma once

#include <map>

enum KeyCode {
	KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I,
	KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R,
	KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, KEY_0,
	KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
	KEY_UNDEFINED, LAST
};

class InputManager {

	public:
		void reset();

		// Returns if the given key has been pressed in this frame.
		bool getKeyDown(KeyCode key);
		
		// Returns if the given key is currently pressed
		bool getKey(KeyCode key);

		#pragma region Internal

		void setKeyDown(KeyCode key, bool val);
		void setKey(KeyCode key, bool val);

		#pragma endregion

		static InputManager* instance() {
			if (!s_instance)
				s_instance = new InputManager();
			return s_instance;
		}

		static KeyCode convertVirtualKeyCode(long virtualKeyCode);
		static long convertKeyCode(KeyCode keyCode);

	private:
		std::map<KeyCode, bool> keyDownMap;
		std::map<KeyCode, bool> keyMap;
		static InputManager* s_instance;
};