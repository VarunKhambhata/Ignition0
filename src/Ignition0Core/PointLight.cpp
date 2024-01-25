/**
 * Author:	Varun Khambhata
 * Created: 17.01.2024
**/

#include <GL/glew.h>

#include <Ignition0Core/PointLight.h>
#include <Ignition0Core/Material0.h>

void PointLight::setColor(float r, float g, float b) 						 { Color = glm::vec4(r,g,b, 0); }
void PointLight::setProperties(float radius, float intensity, float fallOff) { Radius = radius; Intensity = intensity; FallOff = fallOff; }

PointLight::PointLight(): Color(glm::vec4(1,1,1,0)), Radius(10), Intensity(1), FallOff(1) {}

void PointLight::updateUBO(GLuint lpArrayBuffer, int index) {
	if(STATE) {
		Material0::LightProperties data;
		data.position = glm::vec4(getPosition(), 0);
		data.position.z = -data.position.z;
		data.color = Color;
		data.properties = glm::vec4(Radius, Intensity, FallOff, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, lpArrayBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(int) * 4) +  (sizeof(Material0::LightProperties) * index), sizeof(Material0::LightProperties), &data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}