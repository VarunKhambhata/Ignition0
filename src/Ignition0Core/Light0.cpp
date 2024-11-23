/**
 * Author:	Varun Khambhata
 * Created: 23.11.2024
**/

#include <Ignition0Core/Light0.h>

unsigned int Light0::LightBuffer::readHead()  { return head; }
void 		 Light0::LightBuffer::resetHead() { head = -1; }
bool 		 Light0::LightBuffer::next() 	  { bool t = (head < size); head += (int)t; return t; };
Light0::LightBuffer::LightBuffer() 			  { size = 0; head = -1; needsUpdate = false; }


Light0::Light0(): Color(glm::vec3(1,1,1)), Intensity(1) { PENDING_STATE |= PROPERTY_CHANGED; }

void Light0::setColor(float r, float g, float b) {
	Color = glm::vec3{r, g, b};
	PENDING_STATE |= PROPERTY_CHANGED;
}

void Light0::setIntensity(float intensity) {
	Intensity = intensity;
	PENDING_STATE |= PROPERTY_CHANGED;
}

void Light0::updateLight(LightBuffer& lBuffer) {
	if(lBuffer.next()) {
		updateUBO(lBuffer.readHead());
	}
}