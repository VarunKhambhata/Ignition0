/**
 * Author:	Varun Khambhata
 * Created: 01.02.2025
**/

#ifndef __Ignition0Environment__
#define __Ignition0Environment__

#include <memory>

#define STB_IMAGE_IMPLEMENTATION

enum Detail {
	LOW,
	MEDIUM,
	HIGH,
	ULTRA,
};

template <typename T>
using m = std::shared_ptr<T>;

template <typename T>
using mU = std::unique_ptr<T>;

template<typename T, typename... Args>
m<T> make(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
mU<T> makeUniq(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

#endif