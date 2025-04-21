/**
 * Author:	Varun Khambhata
 * Created: 17.07.2024
**/

#ifndef __LINE0__
#define __LINE0__

#include <Ignition0Core/Object0.h>

class Line: public Object0 {
private:
	unsigned int VAO, VBO;
	void onDraw(RenderInfo& rInfo) override;
	
public:
	Line();
	~Line();
};

#endif