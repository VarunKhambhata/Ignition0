/**
 * Author:	Varun Khambhata
 * Created: 17.07.2024
**/

#include <GL/glew.h>

#include <Ignition0Core/Line.h>
#include <Ignition0Supplement/VoidMemory0.h>

static VoidMemory0 lineSharedMem;

Line::Line() {
	if(!lineSharedMem++){
        VAO = static_cast<unsigned int*>(lineSharedMem.getMemory())[0];
        VBO = static_cast<unsigned int*>(lineSharedMem.getMemory())[1];
        return;
    }

	float lineVertices[] = {
        // positions         // texCoords   // normals
        -0.0f, 0.0f, 0.0f,   0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
         1.0f, 0.0f, 0.0f,   1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), &lineVertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(AttribLocation::VERTEX);
    glVertexAttribPointer(AttribLocation::VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // texture coord attribute
    glEnableVertexAttribArray(AttribLocation::TEXTURE);
    glVertexAttribPointer(AttribLocation::TEXTURE, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // normals attribute
    glEnableVertexAttribArray(AttribLocation::NORMAL);
    glVertexAttribPointer(AttribLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    lineSharedMem.setMemory(new unsigned int[2] {VAO, VBO});
}

Line::~Line() {
	if(!lineSharedMem--) {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        delete[] static_cast<unsigned int*>(lineSharedMem.getMemory());
    }
}

void Line::onDraw(const RenderView& rView) {
	material->use();

    glm::mat4 mvp = rView.Projection * getGlobalTransformation();
    material->sharedUniforms.mvp = &mvp;

    if(material->sharedUniforms.model.hasLocation)
        material->sharedUniforms.model = &getGlobalTransformation();
    
    if(material->sharedUniforms.camPosition.hasLocation)
        material->sharedUniforms.camPosition = rView.Position;

	material->updateSharedUniforms();
	glBindVertexArray(VAO);
	glLineWidth(Scale.z);
    glDrawArrays(GL_LINES, 0, 2);
    glLineWidth(1);
}