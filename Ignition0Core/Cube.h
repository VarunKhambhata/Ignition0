/**
 * Author:	Varun Khambhata
 * Created: 19.08.2023
**/

#ifndef __CUBE__
#define __CUBE__

#include <Ignition0Core/Object0.h>

class Cube: public Object0 {
private:
	unsigned int VAO, VBO;

public:
	Cube();
	~Cube();
	void onDraw(const RenderView& rView) override;
};

#endif