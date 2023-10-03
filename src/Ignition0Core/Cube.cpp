/**
 * Author:  Varun Khambhata
 * Created: 19.08.2023
**/

#include <GL/glew.h>

#include <Ignition0Core/Cube.h>
#include <Ignition0Supplement/VoidMemory0.h>

static VoidMemory0 cubeSharedMem;

Cube::Cube() {
    if(!cubeSharedMem++) {
        VAO = static_cast<unsigned int*>(cubeSharedMem.getMemory())[0];
        VBO = static_cast<unsigned int*>(cubeSharedMem.getMemory())[1];
        return;
    }

	float cubeVertices[] = {
        // positions        // texCoords
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,

        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,

         1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f,

        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,

        -1.0f,  1.0f, -1.0f,  0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);    
    // texture coord attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    cubeSharedMem.setMemory(new unsigned int[2] {VAO, VBO});
}

Cube::~Cube() {
    if(!cubeSharedMem--) {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        delete[] static_cast<unsigned int*>(cubeSharedMem.getMemory());
    }
}

void Cube::onDraw() {
	material->sharedUniforms.projection(&getProjection());
	material->use();
	material->updateSharedUniforms();
	glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}