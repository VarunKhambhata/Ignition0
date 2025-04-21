/**
 * Author:	Varun Khambhata
 * Created: 12.07.2024
**/

#ifndef __MODEL0__
#define __MODEL0__

#include <Ignition0Core/Object0.h>

class Model: public Object0 {
private:
	struct MeshGroup {
		unsigned int VAO, VBO;
		unsigned int vertexCount;
		m<Material0> material;
	};

	m<std::vector<MeshGroup>> meshes;

protected:
	void onDraw(RenderInfo& rInfo) override;

public:
	Model();
	~Model();
	
	m<Model> clone();
	void 	 load(std::string filename);
};

#endif