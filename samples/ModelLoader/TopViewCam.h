/**
 * Author:	Varun Khambhata
 * Created: 21.04.2025
**/

#include <Ignition0Core/Script0.h>
#include <Ignition0Core/Camera.h>
#include <Ignition0Core/Logger0.h>
#include <Ignition0Core/IgnitionInputs.h>
#include <Ignition0Core/InternalIgnition0.h>

class TopViewCam: public Script0 {
protected:
    Object0* followedObj;

	void start() {}

	void update() {
        glm::vec3 fPos = followedObj->getPosition();
        setPosition(fPos.x, fPos.y + 200, fPos.z);
	}

public:
    TopViewCam(m<Object0> follow) {
        followedObj = follow.get();
    }
};