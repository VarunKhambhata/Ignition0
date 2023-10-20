/**
 * Author:	Varun Khambhata
 * Created: 20.08.2023
**/

#include <Ignition0Core/Script0.h>
#include <Ignition0Core/Camera.h>
#include <Ignition0Core/IgnitionInputs.h>

class CamControler: public Script0 {
	Camera* cam;

	void start() {
		cam = static_cast<Camera*>(boundObject());
		cam->projection(42, 0.1f, 400.0f);
		cam->setBackground(0.1f, 0.1f, 0.2f);
	}

	void update() {
		if(Keys.a.hold) translate(-0.05, 0,  0   );
		if(Keys.d.hold) translate( 0.05, 0,  0   );
		if(Keys.w.hold) translate( 0   , 0,  0.05);
		if(Keys.s.hold) translate( 0   , 0, -0.05);

		if(Keys.LShift.hold) translate(0, -0.05, 0);
		if(Keys.Space.hold)  translate(0,  0.05, 0);
		
		rotate( Mouse.y.delta? (Mouse.y.delta > 0 ? -0.5 : 0.5) : 0, Mouse.x.delta? (Mouse.x.delta <0 ? 0.5 : -0.5 ) : 0 , 0.5 * Keys.q.hold +  -0.5 * Keys.e.hold);
	}
};
