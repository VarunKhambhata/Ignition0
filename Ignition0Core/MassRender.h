/**
 * Author:	Varun Khambhata
 * Created: 27.11.2024
**/

#ifndef __MASSRENDER__
#define __MASSRENDER__

#include <Ignition0Core/Object0.h>

class MassRender: public Object0 {
private:
	unsigned int instances;
	GLuint transformSSBO;

protected:
	void onDraw(RenderInfo &rInfo) override;

public:
	struct TransformVecs { glm::vec3 position, rotation, scale; };

	MassRender();
	~MassRender();
	void setMassTransforms(const std::vector<TransformVecs> transforms);
};

#endif