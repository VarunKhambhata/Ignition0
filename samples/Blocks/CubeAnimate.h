/**
 * Author:	Varun Khambhata
 * Created: 20.08.2023
**/

#include <Ignition0Core/Script0.h>

class CubeAnimate: public Script0 {
	void start() {}

	void update() {
		translate(0.004, -0.004, 0.004);

		glm::vec3 pos = getPosition();

		if(pos.x > 25)  pos.x = -15;
		if(pos.y < -15) pos.y =  25;
		if(pos.z > 25)  pos.z = -15;

		pos.y += sin(pos.x) / 50;

		setPosition(pos.x, pos.y, pos.z);

		int dir = pos.x < 0 ? -1 : 1;
		rotate(0.1 * dir, 0.15 * dir, 0.05 * dir);
	}
};
