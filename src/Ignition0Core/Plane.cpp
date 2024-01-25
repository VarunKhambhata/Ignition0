/**
 * Author:  Varun Khambhata
 * Created: 12.08.2023
**/

#include <GL/glew.h>

#include <Ignition0Core/Plane.h>
#include <Ignition0Core/Scene.h>
#include <Ignition0Supplement/VoidMemory0.h>

static VoidMemory0 planeSharedMem;

Plane::Plane() {
    if(!planeSharedMem++){
        VAO = static_cast<unsigned int*>(planeSharedMem.getMemory())[0];
        VBO = static_cast<unsigned int*>(planeSharedMem.getMemory())[1];
        return;
    }

	float quadVertices[] = {
        // positions        // texCoords     // normals
        -1.0f,  1.0f, 0.0f,      0.0f, 1.0f,      0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,      0.0f, 0.0f,      0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,      1.0f, 0.0f,      0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,      0.0f, 1.0f,      0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,      1.0f, 0.0f,      0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f,      1.0f, 1.0f,      0.0f, 0.0f, 1.0f,
    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(AttribLocation::VERTEX);
    glVertexAttribPointer(AttribLocation::VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // texture coord attribute
    glEnableVertexAttribArray(AttribLocation::TEXTURE);
    glVertexAttribPointer(AttribLocation::TEXTURE, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // normals attribute
    glEnableVertexAttribArray(AttribLocation::NORMAL);
    glVertexAttribPointer(AttribLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    planeSharedMem.setMemory(new unsigned int[2] {VAO, VBO});
}

Plane::~Plane() {
    if(!planeSharedMem--) {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        delete[] static_cast<unsigned int*>(planeSharedMem.getMemory());
    }
}

void Plane::onDraw() {
	material->use();

    material->sharedUniforms.mvp = &getProjection();

    if(material->sharedUniforms.model.hasLocation)
        material->sharedUniforms.model = &getTransformation();
    
    if(material->sharedUniforms.camPosition.hasLocation) {
        glm::vec3 camPos = Scene::getCurrentCamera()->getPosition();
        camPos.z *= -1;
        material->sharedUniforms.camPosition = camPos;
    }

	material->updateSharedUniforms();
	glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
