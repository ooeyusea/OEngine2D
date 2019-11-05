#pragma once

#ifdef WIN32
#include "detail/windows/win32.h"
#else
#include "detail/linux/linux.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
    void __OAssert(const char * file, int line, const char * funname, const char * debug);
#ifdef __cplusplus
};
#endif

#ifndef WIN32
#define OASSERT(p, format, a...) { \
    char debug[4096] = {0}; \
    SafeSprintf(debug, sizeof(debug), format, ##a); \
    ((p) ? (void)0 : (void)__OAssert(__FILE__, __LINE__, __FUNCTION__, debug)); \
}
#else
#define OASSERT(p, format, ...) { \
    char debug[4096] = {0}; \
    SafeSprintf(debug, sizeof(debug), format, ##__VA_ARGS__); \
    ((p) ? (void)0 : (void)__OAssert(__FILE__, __LINE__, __FUNCTION__, debug)); \
}
#endif 

#ifdef __cplusplus
extern "C" {
#endif
	inline void __OMemcpy(void * dst, const int32_t maxSize, const void * src, const int32_t size) {
		OASSERT(size <= maxSize, "memcpy out of range");

		const int32_t len = (size > maxSize ? maxSize : size);
		memcpy(dst, src, len);
	}

	inline void __OMemset(void * dst, const int32_t maxSize, const int32_t val, const int32_t size) {
		OASSERT(size <= maxSize, "memset out of range");

		const int32_t len = (size > maxSize ? maxSize : size);
		memset(dst, val, len);
	}
#ifdef __cplusplus
};
#endif

#define SafeMemcpy __OMemcpy
#define SafeMemset __OMemset

#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#define FLOAT_PRECISION 0.000001f

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <functional>
#include <algorithm>

#define CHECK(p, msg) \
	if (!(p)) {\
		std::cout << msg << std::endl; \
		return false;\
	}\

#define INIT_STEP(f, msg) \
	if (!(f)) {\
		std::cout << msg << " failed" << std::endl; \
		break;\
	}\
	std::cout << msg << " success" << std::endl

#define SAFE_DELETE(p) \
if (p) {\
	delete p;\
	p = nullptr;\
}