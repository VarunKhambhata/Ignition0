#include <Ignition0Core/UnlitColor.h>

class UnlitNegative: public UnlitImage {
public:
	std::string fragmentShaderSource() {
		return R"(
			#version 330 core
			out vec4 FragColor;
			in vec2 TexCoords;
			uniform sampler2D screenTexture;

			void main()
			{ 
			    FragColor = 1.0 - texture(screenTexture, TexCoords);
			}
	    )";
	}

	UnlitNegative(): UnlitImage(false) {
		build();
	}
};
