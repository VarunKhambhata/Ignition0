/**
 * Author:	Varun Khambhata
 * Created: 21.04.2025
**/

#include <Ignition0Core/Material0.h>

static ShaderContext singlePSTPContext;

class PostProcess: public Material0 {
private:
    GLuint texture;

protected:
	std::string vertexShaderSource() override {
        return R"(
            #version 430 core
            in vec3 aVertex;
            in vec2 aTexCoords;
            uniform mat4 mvp;
            out vec2 TexCoords;

            void main()
            {
                gl_Position = vec4(aVertex,1.0);
                TexCoords = aTexCoords;
            }
        )";
    }

    std::string fragmentShaderSource() override {
        return R"(
            #version 430 core
            in vec2 TexCoords;
            out vec4 FragColor;
            uniform sampler2D tex;

            vec3 adjustContrast(vec3 color, float contrast) {
                return (color - 0.5) * contrast + 0.5;
            }

            float random(vec2 uv) {
                return fract(sin(dot(uv, vec2(12.9898,78.233))) * 43758.5453);
            }

            vec3 addNoise(vec2 uv, vec3 color, float amount) {
                float n = random(uv) * amount;
                return color + vec3(n);
            }

            void main()
            { 
                FragColor = texture2D(tex, TexCoords);
                FragColor.rgb = adjustContrast(FragColor.rgb, 1.35);
                FragColor.rgb = addNoise(TexCoords, FragColor.rgb, 0.1);
            }
        )";
    }

    AttribNames bindShaderAttribs() {
        return {"aVertex", "aTexCoords"};
    }	

    void onUsed() override {
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    
    void setTexture(GLuint tex) override {
        texture = tex;
    }

public:
    PostProcess() {
		build(&singlePSTPContext);
	}
};