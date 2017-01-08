#pragma once

#include <map>
#include <set>

enum KeyCode {
	KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I,
	KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R,
	KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, KEY_0,
	KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
	KEY_ARROW_UP, KEY_ARROW_DOWN, KEY_ARROW_LEFT, KEY_ARROW_RIGHT,
	SPACE,
	KEY_UNDEFINED, LAST
};

class InputManager {

	public:
		
		// Recalculates the input values for the current frame
		void reset();

		// Returns if the given key has been pressed in this frame.
		bool getKeyDown(KeyCode key);
		
		// Returns if the given key is currently pressed
		bool getKey(KeyCode key);

		#pragma region Internal

		void setKeyDown(KeyCode key, bool val);
		void setKey(KeyCode key, bool val);
		void keyPressed(KeyCode keyCode);
		void keyReleased(KeyCode keyCode);
		void reset(KeyCode keyCode);

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
		std::set<KeyCode> keyBuffer;
		static InputManager* s_instance;
};