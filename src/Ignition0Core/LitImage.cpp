/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Ignition0Core/LitImage.h>
#include <Ignition0Core/stb_image.h>
#include <Ignition0Core/Logger0.h>

#include <Ignition0Supplement/VoidMemory0.h>


std::string LitImage::vertexShaderSource() {
	return R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoords;
		layout (location = 2) in vec3 aNormal;

		uniform mat4 mvp;
		uniform mat4 model;

		out vec2 TexCoords;
		out vec3 FragPos;
	    out vec3 Normal;

		void main()
		{
		    gl_Position = mvp * vec4(aPos, 1.0);
		    FragPos = vec3(model * vec4(aPos, 1.0));
	       	Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
		    TexCoords = aTexCoords;
		}
	)";
}

std::string LitImage::fragmentShaderSource() {
	return R"(
		#version 330 core

		in vec3 FragPos;
	    in vec3 Normal;
		in vec2 TexCoords;

		uniform vec3 viewPos;
		uniform sampler2D tex;

		uniform float ambientStrength;
		uniform float diffStrength;
		uniform float specularStrength;
		uniform float shininess;


		int getMaxLights();
		vec3 getLightPosition(int index);
		vec3 getLightColor(int index);
		float getLightRadius(int index);
		float getLightIntensity(int index);
		float getLightFallOff(int index);


		float attenuate_cusp(float distance, float radius, float max_intensity, float falloff) {
			float s = min(distance / radius, 1.0);
			float s2 = pow(s,2);
			return max_intensity * pow(1 - s2,2) / (1 + falloff * s);
		}

		vec3 PointLightEffect(vec3 lPosition, vec3 lColor, float radius, float intensity, float fallOff, vec3 viewDir, vec3 norm) {
			// ambient
    		vec3 ambient 		  = lColor * ambientStrength;

    		// diffuse
    		vec3 lightDir 		  = normalize(lPosition - FragPos);
    		float diffuseStrength = max(dot(norm, lightDir), 0.0);
    		vec3 diffuse 		  = lColor * (diffuseStrength * diffStrength);

    		// specular - Blinn-Phong
    		vec3 halfwayDir       = normalize(lightDir + viewDir);
    		float spec            = pow(max(dot(norm, halfwayDir), 0.0), shininess);
			vec3 specular   	  = lColor * (spec * specularStrength);

			// attenuation
			float distance    = length(lPosition - FragPos);
			float attenuation = attenuate_cusp(distance, radius, intensity, fallOff);
			ambient  *= attenuation;
			diffuse  *= attenuation;
			specular *= attenuation;

    		return (ambient + diffuse + specular);
		}

		void main()
		{
		    vec4 texColor = texture2D(tex, TexCoords);

		    vec3 result;
	    	vec3 norm    = normalize(Normal);
	    	vec3 viewDir = normalize(viewPos - FragPos);

	    	for(int i = 0; i < getMaxLights(); i++) {
	    		vec3 lightPos        = getLightPosition(i);
	    		vec3 lightColor      = getLightColor(i);
	    		float lightRadius    = getLightRadius(i);
	    		float lightIntensity = getLightIntensity(i);
	    		float lightFallOff   = getLightFallOff(i);

		    	result += PointLightEffect(lightPos, lightColor, lightRadius, lightIntensity, lightFallOff, viewDir, norm);
	    	}

		    gl_FragColor = texColor;
		    gl_FragColor.rgb *= result;
		}
    )";
}

LitImage::LitImage(bool init) {
	build();
	updateUniforms();
}

void LitImage::setMaterialProperties(float ambient, float diffuse, float specular, float shininess) {
	uniforms.ambient   = ambient;
	uniforms.diffuse   = diffuse;
	uniforms.specular  = specular;
	uniforms.shininess = shininess;
	updateUniforms();
}

void LitImage::onUsed() {
	glBindTexture(GL_TEXTURE_2D, texture);
	updateUniforms();
}

void LitImage::setTexture(GLuint tex) {
	texture = tex;
}

void LitImage::setTexture(const char* imgFile) {
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(imgFile, &width, &height, &nrChannels, 0);	

	if(data) {
		glGenTextures(1, &texture);
	 	glBindTexture(GL_TEXTURE_2D, texture);
    
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	else {
		Logger0E("Missing Texture File: " << imgFile);
	}
}

void LitImage::initUniforms() {
	sharedUniforms.mvp(getLocation("mvp"));
	sharedUniforms.model(getLocation("model"));
	sharedUniforms.camPosition(getLocation("viewPos"));

	uniforms.ambient(getLocation("ambientStrength"));
	uniforms.diffuse(getLocation("diffStrength"));
	uniforms.specular(getLocation("specularStrength"));
	uniforms.shininess(getLocation("shininess"));
}


void LitImage::onUniformsUpdate() {
	glUniform1f(uniforms.ambient, ~uniforms.ambient);
	glUniform1f(uniforms.diffuse, ~uniforms.diffuse);
	glUniform1f(uniforms.specular, ~uniforms.specular);
	glUniform1f(uniforms.shininess, ~uniforms.shininess);
}
