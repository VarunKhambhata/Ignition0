/**
 * Author:	Varun Khambhata
 * Created: 25.10.2023
**/

#include <GL/glew.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Ignition0Core/LitColor.h>
#include <Ignition0Core/Scene.h>

std::string LitColor::vertexShaderSource() {
	return R"(
		#version 330 core
	    layout (location = 0) in vec3 aPos;
	    layout (location = 2) in vec3 aNormal;

	    out vec3 FragPos;
	    out vec3 Normal;

	    uniform mat4 mvp;
	    uniform mat4 model;

	    void main()
	    {
	       gl_Position = mvp * vec4(aPos, 1.0);
	       FragPos = vec3(model * vec4(aPos, 1.0));
	       Normal = mat3(transpose(inverse(model))) * aNormal;
	    }
	)";
}

std::string LitColor::fragmentShaderSource() {
	return R"(
		#version 330 core

	    in vec3 FragPos;
	    in vec3 Normal;

	    uniform vec3 viewPos;
	    uniform vec3 color;

		uniform float ambientStrength;
		uniform float diffStrength;
		uniform float specularStrength;
		uniform float shininess;


		int getMaxDirectionalLights();
		vec3 getDirectionalLight(int index);
		vec3 getDirectionalLightColor(int index);
		float getDirectionalLightIntensity(int index);

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

		vec3 CalcDirectionalLight(vec3 lColor, vec3 lDirection, float lIntensity, vec3 viewDir, vec3 norm) {
			// ambient
			vec3 ambient = lColor * ambientStrength;
			
			// diffuse
			float diffuseStrength = max(dot(norm, lDirection), 0.0);
			vec3 diffuse = lColor * (diffuseStrength * diffStrength);

			// specular - Blinn-Phong
			vec3 halfwayDir       = normalize(lDirection + viewDir);
    		float spec            = pow(max(dot(norm, halfwayDir), 0.0), shininess);
			vec3 specular   	  = lColor * (spec * specularStrength);

			// attenuation
			return (ambient + diffuse + specular) * lIntensity;
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
	    	vec3 result;
	    	vec3 norm    = normalize(Normal);
	    	vec3 viewDir = normalize(viewPos - FragPos);

	    	for(int i = 0; i < getMaxDirectionalLights(); i++) {
	    		result += CalcDirectionalLight(getDirectionalLightColor(i), getDirectionalLight(i), getDirectionalLightIntensity(i), viewDir, norm);
	    	}

	    	for(int i = 0; i < getMaxLights(); i++) {
	    		vec3 lightPos        = getLightPosition(i);
	    		vec3 lightColor      = getLightColor(i);
	    		float lightRadius    = getLightRadius(i);
	    		float lightIntensity = getLightIntensity(i);
	    		float lightFallOff   = getLightFallOff(i);
		    	
		    	result += PointLightEffect(lightPos, lightColor, lightRadius, lightIntensity, lightFallOff, viewDir, norm);
	    	}

		    gl_FragColor = vec4(result * color, 1.0);
	    }
    )";
}


LitColor::LitColor(float r, float g, float b, float ambient, float diffuse, float specular, float shininess) {
	uniforms.color     = glm::vec3(r, g, b);
	uniforms.ambient   = ambient;
	uniforms.diffuse   = diffuse;
	uniforms.specular  = specular;
	uniforms.shininess = shininess;

	build();
	updateUniforms();
}

void LitColor::initUniforms() {
	sharedUniforms.mvp(getLocation("mvp"));
	sharedUniforms.model(getLocation("model"));
	sharedUniforms.camPosition(getLocation("viewPos"));

	uniforms.color(getLocation("color"));
	uniforms.ambient(getLocation("ambientStrength"));
	uniforms.diffuse(getLocation("diffStrength"));
	uniforms.specular(getLocation("specularStrength"));
	uniforms.shininess(getLocation("shininess"));
}

void LitColor::onUsed() {
	updateUniforms();
}

void LitColor::onUniformsUpdate() {
	glUniform3fv(uniforms.color, 1, & (~uniforms.color)[0]);
	glUniform1f(uniforms.ambient, ~uniforms.ambient);
	glUniform1f(uniforms.diffuse, ~uniforms.diffuse);
	glUniform1f(uniforms.specular, ~uniforms.specular);
	glUniform1f(uniforms.shininess, ~uniforms.shininess);
}
