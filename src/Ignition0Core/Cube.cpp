/**
 * Author:  Varun Khambhata
 * Created: 19.08.2023
**/

#include <GL/glew.h>

#include <Ignition0Supplement/VoidMemory0.h>
#include <Ignition0Core/Cube.h>

static VoidMemory0 cubeSharedMem;

Cube::Cube() {
    if(!cubeSharedMem++) {
        VAO = static_cast<unsigned int*>(cubeSharedMem.getMemory())[0];
        VBO = static_cast<unsigned int*>(cubeSharedMem.getMemory())[1];
        return;
    }

	float cubeVertices[] = {
        // positions				// texCoords		// normals

        // back
        -1.0f, -1.0f, -1.0f,        0.0f, 0.0f,         0.0f, 0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,        1.0f, 0.0f,         0.0f, 0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,        1.0f, 1.0f,         0.0f, 0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,        1.0f, 1.0f,         0.0f, 0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,        0.0f, 1.0f,         0.0f, 0.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,        0.0f, 0.0f,         0.0f, 0.0f, -1.0f,
    
        // front
        -1.0f, -1.0f,  1.0f,        1.0f, 0.0f,         0.0f, 0.0f, 1.0f,
         1.0f, -1.0f,  1.0f,        0.0f, 0.0f,         0.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        0.0f, 1.0f,         0.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        0.0f, 1.0f,         0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,        1.0f, 1.0f,         0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,        1.0f, 0.0f,         0.0f, 0.0f, 1.0f,

        // left
        -1.0f,  1.0f,  1.0f,        0.0f, 1.0f,         -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,        1.0f, 1.0f,         -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,        1.0f, 0.0f,         -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,        1.0f, 0.0f,         -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,        0.0f, 0.0f,         -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,        0.0f, 1.0f,         -1.0f, 0.0f, 0.0f,

        // right
         1.0f,  1.0f,  1.0f,        1.0f, 1.0f,         1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,        0.0f, 1.0f,         1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,        0.0f, 0.0f,         1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,        0.0f, 0.0f,         1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,        1.0f, 0.0f,         1.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 1.0f,         1.0f, 0.0f, 0.0f,

        // bottom
        -1.0f, -1.0f, -1.0f,        0.0f, 1.0f,         0.0f, -1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,        1.0f, 1.0f,         0.0f, -1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,        1.0f, 0.0f,         0.0f, -1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,        1.0f, 0.0f,         0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,        0.0f, 0.0f,         0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,        0.0f, 1.0f,         0.0f, -1.0f, 0.0f,

        // top
        -1.0f,  1.0f, -1.0f,        0.0f, 0.0f,         0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,        1.0f, 0.0f,         0.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 1.0f,         0.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 1.0f,         0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,        0.0f, 1.0f,         0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,        0.0f, 0.0f,         0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(AttribLocation::VERTEX);
    glVertexAttribPointer(AttribLocation::VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);    
    // texture coord attribute
    glEnableVertexAttribArray(AttribLocation::TEXTURE);
    glVertexAttribPointer(AttribLocation::TEXTURE, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // normals attribute
    glEnableVertexAttribArray(AttribLocation::NORMAL);
    glVertexAttribPointer(AttribLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    cubeSharedMem.setMemory(new unsigned int[2] {VAO, VBO});
}

Cube::~Cube() {
    if(!cubeSharedMem--) {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        delete[] static_cast<unsigned int*>(cubeSharedMem.getMemory());
    }
}

void Cube::onDraw(RenderInfo& rInfo) {
    if(material->sharedUniforms.model.hasLocation)
    material->sharedUniforms.model = &getGlobalTransformation();

    material->use();

	glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, rInfo.DrawInstances);
}