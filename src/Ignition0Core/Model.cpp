/**
 * Author:	Varun Khambhata
 * Created: 12.07.2024
**/

#include <fstream> 
#include <cstring>
#include <GL/glew.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <ModelParsers/tiny_obj_loader.h>

#include <Ignition0Core/Model.h>

#include <Ignition0Core/Logger0.h>
#include <Ignition0Core/LitColor.h>


Model::Model() {
    meshes = make<std::vector<MeshGroup>>();
}

Model::~Model() {
    if(meshes.use_count() == 1) {
        for(MeshGroup m: *meshes) {
            glDeleteBuffers(1, &m.VBO);
            glDeleteVertexArrays(1, &m.VAO);
        }
    }
}

m<Model> Model::clone() {
    m<Model> clone = make<Model>();
    clone->meshes = meshes;
    clone->material = material;
    return clone;
}

void Model::onDraw(const RenderView& rView) {
    material->use();

    glm::mat4 mvp = rView.Projection * getGlobalTransformation();
    material->sharedUniforms.mvp = &mvp;

    if(material->sharedUniforms.model.hasLocation)
        material->sharedUniforms.model = &getGlobalTransformation();
    
    if(material->sharedUniforms.camPosition.hasLocation)
        material->sharedUniforms.camPosition = rView.Position;

    material->updateSharedUniforms();

    for(MeshGroup m: *meshes) {
        glBindVertexArray(m.VAO);
        glDrawArrays(GL_TRIANGLES, 0, m.vertexCount);
    }
}


void Model::load(std::string filename) {
    tinyobj::ObjReader reader;

    tinyobj::ObjReaderConfig reader_config;
    reader_config.triangulate = true;
    reader_config.vertex_color = false;

    if (!reader.ParseFromFile(filename, reader_config)) {
      if (!reader.Error().empty())
          Logger0E("TinyObjReader [error]: " << reader.Error());
      return;
    }

    if (!reader.Warning().empty())
        Logger0E("TinyObjReader [warning]: " << reader.Warning());

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    for(auto shape: shapes) {
        std::vector<float> buffer;

        size_t indexOffset = 0;
        for(size_t fv: shape.mesh.num_face_vertices) {
            for(int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                buffer.push_back(vx);
                buffer.push_back(vy);
                buffer.push_back(vz);

                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                    buffer.push_back(tx);
                    buffer.push_back(ty);
                }

                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                    tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                    tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];

                    buffer.push_back(nx);
                    buffer.push_back(ny);
                    buffer.push_back(nz);
                  }
            }
            indexOffset += fv;
        }


        MeshGroup m;
        m.vertexCount = shape.mesh.indices.size();

        glGenVertexArrays(1, &m.VAO);
        glBindVertexArray(m.VAO);
        glGenBuffers(1, &m.VBO);

        glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * buffer.size(), buffer.data(), GL_STATIC_DRAW);

        // position attribute
        glEnableVertexAttribArray(AttribLocation::VERTEX);
        glVertexAttribPointer(AttribLocation::VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        // texture coord attribute
        glEnableVertexAttribArray(AttribLocation::TEXTURE);
        glVertexAttribPointer(AttribLocation::TEXTURE, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        // normals attribute
        glEnableVertexAttribArray(AttribLocation::NORMAL);
        glVertexAttribPointer(AttribLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

        meshes->push_back(m);
    }
}