/**
 * Author:	Varun Khambhata
 * Created: 21.10.2023
**/

#include <Ignition0Core/Script0.h>

class Rotation: public Script0 {
	bool isCamera = false;

	void start() {
		if(dynamic_cast<Camera*>(boundObject()))
			isCamera = true;
	}

	void update() {
		if(isCamera)
			rotate(0,0,0.3);
		else
			rotate(0.1, 0.05, 0.12);
	}
};