/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __INTERNALIGNITION0__
#define __INTERNALIGNITION0__

#include <memory>
#include <glm/glm.hpp>
#include <Ignition0Core/UnlitColor.h>
#include <Ignition0Core/UnlitImage.h>

template <typename T>
using m = std::shared_ptr<T>;

template<typename T, typename... Args>
std::shared_ptr<T> make(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

enum Detail {
	LOW,
	MEDIUM,
	HIGH,
	ULTRA,
};

#define MAX_LIGHT 1024

namespace internal {
	class __engine__ {
	private:
		glm::ivec2 _displaySize;
		bool _initialized;
		Detail detailLevel;

	public:
		bool initialize();
		void displaySize(int w, int h);
		glm::ivec2 displaySize();
		void setDetailLevel(Detail level);
		Detail preferedDetail();
		m<UnlitColor> missing;
		m<UnlitImage> colorImage;
		static const glm::mat4 IDENTITY;
		static const glm::vec3 ORIGIN;
	} extern Ignition0;
}

#endif