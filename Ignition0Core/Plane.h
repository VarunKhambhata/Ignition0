/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __PLANE__
#define __PLANE__

#include <Ignition0Core/Object0.h>

class Plane: public Object0 {
private:
	unsigned int VAO, VBO;

public:
	Plane();
	~Plane();
	void onDraw();
};

#endif