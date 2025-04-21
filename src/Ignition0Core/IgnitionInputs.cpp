#include <Ignition0Core/IgnitionInputs.h>

namespace inputs {
	Keyset::Keyset() {
		map[48] = &num0;	map[50] = &num2;	map[52] = &num4;	map[54] = &num6;	map[56] = &num8;
		map[49] = &num1;  	map[51] = &num3;	map[53] = &num5;	map[55] = &num7;	map[57] = &num9;

		map[65] = &a;	map[70] = &f;	map[75] = &k;	map[80] = &p;	map[85] = &u;	map[90] = &z;
		map[66] = &b;	map[71] = &g;	map[76] = &l;	map[81] = &q;	map[86] = &v;
		map[67] = &c;	map[72] = &h;	map[77] = &m;	map[82] = &r;	map[87] = &w;
		map[68] = &d;	map[73] = &i;	map[78] = &n;	map[83] = &s;	map[88] = &x;
		map[69] = &e;	map[74] = &j;	map[79] = &o;	map[84] = &t;	map[89] = &y;


		map[32]  = &Space;		map[266] = &PageUp;
		map[257] = &Enter;		map[267] = &PageDown;
		map[258] = &Tab;		map[268] = &Home;
		map[256] = &Escape;		map[269] = &End;
		map[259] = &Backspace;	map[340] = &LShift;
		map[261] = &Delete;		map[341] = &LCtrl;
		map[262] = &Right;		map[342] = &LAlt;
		map[263] = &Left;		map[344] = &RShift;
		map[264] = &Down;		map[345] = &RCtrl;
		map[265] = &Up;			map[346] = &RAlt;

		map[290] = &F1;		map[294] = &F5;		map[298] = &F9;
		map[291] = &F2;		map[295] = &F6;		map[299] = &F10;
		map[292] = &F3;		map[296] = &F7;		map[300] = &F11;
		map[293] = &F4;		map[297] = &F8;		map[301] = &F12;
	}

	Key::Key(): _pressed(0), _hold(0), pressed(0), hold(0) {}

	void Key::operator=(int state) {
		pressed = _pressed = state == 1;
		hold    = _hold    = state > 0;
	}

	MouseDevice::MouseDevice() {
		keyMap[0] =  &button1;
		keyMap[1] =  &button2;
	}

	void MouseDevice::pos(int X, int Y) {
		_x = X;
		_y = Y;

		x = _x;
		y = _y;
	}

	void MouseDevice::clearDelta() {
		_x.delta = x.delta = _y.delta = y.delta = 0;
	}

	template <typename T>
	void Axis<T>::operator=(T val) {
		delta = _delta = _value - val;
		value = _value = val;
	}

	template <typename T>
	void Axis<T>::operator=(Axis<T>& other) {
		value = other.value;
		delta = other.delta;
	}
};