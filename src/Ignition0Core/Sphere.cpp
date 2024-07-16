/**
 * Author:	Varun Khambhata
 * Created: 11.07.2024
**/

#include <GL/glew.h>

#include <Ignition0Core/Sphere.h>
#include <Ignition0Supplement/VoidMemory0.h>

static VoidMemory0 sphereSharedMem;

Sphere::Sphere() {
	if(!sphereSharedMem++) {
        VAO 	 = static_cast<unsigned int*>(sphereSharedMem.getMemory())[0];
        VBO 	 = static_cast<unsigned int*>(sphereSharedMem.getMemory())[1];
        EBO 	 = static_cast<unsigned int*>(sphereSharedMem.getMemory())[2];
        polygons = static_cast<unsigned int*>(sphereSharedMem.getMemory())[3];
        return;
    }

	std::vector<float> vertices;
	std::vector<int> indices;

	float x, y, z, xy;
	float nx, ny, nz;
	float s, t;

	float sectorCount;
	float stackCount;

	switch(internal::Ignition0.preferedDetail()) {
		case Detail::LOW:    sectorCount = stackCount = 06; break;
		case Detail::MEDIUM: sectorCount = stackCount = 10; break;
		case Detail::HIGH:   sectorCount = stackCount = 18; break;
		case Detail::ULTRA:  sectorCount = stackCount = 30; break;
	}
	
	float sectorStep = 2 * M_PI / sectorCount;
	float stackStep  = M_PI / stackCount;
	float sectorAngle, stackAngle;

	for(int i = 0; i <= stackCount; ++i) {
		stackAngle = M_PI / 2 - i * stackStep;
		xy = cosf(stackAngle);
		z  = sinf(stackAngle);

		for(int j = 0; j <= sectorCount; ++j) {
			sectorAngle = j * sectorStep;

			x = xy * sinf(sectorAngle);
			y = xy * cosf(sectorAngle);			

			nx = x;
			ny = y;
			nz = z;

			s = (float)j / sectorCount;
			t = (float)i / stackCount;

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			vertices.push_back(s);
			vertices.push_back(t);

			vertices.push_back(nx);
			vertices.push_back(ny);
			vertices.push_back(nz);
		}
	}

	int k1, k2;
	for(int i=0; i < stackCount; ++i) {
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for(int j=0; j < sectorCount; ++j, ++k1, ++ k2) {
			if(i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if(i != (stackCount-1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	polygons = indices.size();

	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    
	glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(AttribLocation::VERTEX);
    glVertexAttribPointer(AttribLocation::VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // texture coord attribute
    glEnableVertexAttribArray(AttribLocation::TEXTURE);
    glVertexAttribPointer(AttribLocation::TEXTURE, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // normals attribute
    glEnableVertexAttribArray(AttribLocation::NORMAL);
    glVertexAttribPointer(AttribLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));    

    sphereSharedMem.setMemory(new unsigned int[4] {VAO, VBO, EBO, polygons});
}

Sphere::~Sphere() {
	if(!sphereSharedMem--) {
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	    glDeleteVertexArrays(1, &VAO);
	    delete[] static_cast<unsigned int*>(sphereSharedMem.getMemory());
	}
}


void Sphere::onDraw(const RenderView& rView) {
	material->use();

    glm::mat4 mvp = rView.Projection * getGlobalTransformation();
    material->sharedUniforms.mvp = &mvp;

    if(material->sharedUniforms.model.hasLocation)
        material->sharedUniforms.model = &getGlobalTransformation();
    
    if(material->sharedUniforms.camPosition.hasLocation)
        material->sharedUniforms.camPosition = rView.Position;

	material->updateSharedUniforms();
	glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, polygons, GL_UNSIGNED_INT, (void*)0);
}