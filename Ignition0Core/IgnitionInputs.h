/**
 * Author:	Varun Khambhata
 * Created: 03.10.2023
**/

#ifndef __IGNITIONINPUTS__
#define __IGNITIONINPUTS__

class EngineCore;

namespace inputs {
	class Key {
	private:
		bool _pressed, _hold;
		void operator=(int state);

	public:
		Key();
		bool pressed, hold;
		friend class ::EngineCore;
	};

	template <typename T>
	class Axis {
	private:
		T _value;
		T _delta;

		void operator=(T val);
		void operator=(Axis& other);

	public:
		T value;
		T delta;

		friend class MouseDevice;
	};


	class Keyset {
	private:
		Key* map[349];

	public:	
		Key num0, num1, num2, num3, num4, num5, num6, num7, num8, num9;
		Key a, b, c, d, e, f, g ,h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;
		Key Up, Down, Left, Right, Enter, Escape, Space, Backspace, Delete, LShift, RShift, LCtrl, RCtrl, LAlt, RAlt, Tab, Home, End, PageUp, PageDown;
		Key F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;

		Keyset();

		friend class ::EngineCore;
	};

	class MouseDevice {
	private:
		Axis<int> _x, _y;
		Key* keyMap[2];

		void pos(int X, int Y);
		void clearDelta();

	public:
		Key button1, button2;
		Axis<int> x, y;

		MouseDevice();
		friend class ::EngineCore;
	};
}

extern inputs::Keyset Keys;
extern inputs::MouseDevice Mouse;

#endif