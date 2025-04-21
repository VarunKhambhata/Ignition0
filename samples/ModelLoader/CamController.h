/**
 * Author:	Varun Khambhata
 * Created: 20.08.2023
**/

#include <Ignition0Core/Script0.h>
#include <Ignition0Core/IgnitionInputs.h>
#include <Ignition0Core/InternalIgnition0.h>

class CamControler: public Script0 {
	void start() {}

	void update() {
		float move = 25 * InternalIgnition0::deltaTime();
		float look = move * 4;

		if(Keys.a.hold) translate(-move, 0,  0   );
		if(Keys.d.hold) translate( move, 0,  0   );
		if(Keys.w.hold) translate( 0   , 0,  move);
		if(Keys.s.hold) translate( 0   , 0, -move);

		if(Keys.LShift.hold) translate(0, -move, 0);
		if(Keys.Space.hold)  translate(0,  move, 0);
		
		rotate(Mouse.y.delta? (Mouse.y.delta > 0 ? -look : look) : 0,
			   Mouse.x.delta? (Mouse.x.delta < 0 ? look : -look) : 0,
			   0);
	}
};