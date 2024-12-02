/**
 * Author:	Varun Khambhata
 * Created: 27.11.2024
**/

#ifndef __MASSRENDER__
#define __MASSRENDER__

#include <Ignition0Core/Object0.h>

class MassRender: public Object0 {
private:
	unsigned int instanceVBO;
	unsigned int instances, parentInstances;

protected:
	void applyStateUpdate() override final;

public:
	MassRender();
	~MassRender();
	struct transformation { float posX, posY, posZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ; };
	void setMassTransforms(const std::vector<transformation> transforms);
	static void callback(const void* self);
	void enableVertexArray();
	void onDraw(const RenderView& rView) override;
};

#endif