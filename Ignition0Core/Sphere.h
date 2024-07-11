/**
 * Author:	Varun Khambhata
 * Created: 11.07.2024
**/

#ifndef __SPHERE0__
#define __SPHERE0__

#include <Ignition0Core/Object0.h>

class Sphere: public Object0 {
private:	
	unsigned int VAO, VBO, EBO;
	unsigned int polygons;

public:
	Sphere();
	~Sphere();
	void onDraw(const RenderView& rView) override;
};

#endif