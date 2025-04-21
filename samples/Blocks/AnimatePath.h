/**
 * Author:	Varun Khambhata
 * Created: 21.04.2025
**/

#include <vector>

#include <glm/gtx/compatibility.hpp>

#include <Ignition0Core/Script0.h>
#include <Ignition0Core/InternalIgnition0.h>

class AnimatePath: public Script0 {
private:

    int currentPathPos = 0;
    int nextPathPos = 1;
    float step = 0;

    std::vector<glm::vec3> path;
    bool havePath;

protected:
	void start() {
        havePath = path.size() > 1;
    }

	void update() {
        if(!havePath)
            return;

        glm::vec3 pos = glm::lerp(path[currentPathPos], path[nextPathPos], step);

        if(step >= 1) {
            step = 0;
            currentPathPos ++;
            nextPathPos ++;

            if(currentPathPos == path.size()) currentPathPos = 0;
            if(nextPathPos == path.size()) nextPathPos = 0;
        }

        setPosition(pos.x, pos.y, pos.z);
        step += 0.1 * InternalIgnition0::deltaTime();
	}

public:
    AnimatePath(std::vector<glm::vec3> path) : path(path) {}
};