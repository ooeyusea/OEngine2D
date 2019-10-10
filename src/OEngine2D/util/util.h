#ifndef __util_h__
#define __util_h__
#ifdef WIN32
#include "win32.h"
#else
#include "linux.h"
#endif

#define MALLOC malloc
#define FREE free
#define REALLOC realloc
#define NEW new
#define DEL delete
#define ALLOCATOR(T) std::allocator<T>

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
	inline void __OMemcpy(void * dst, const s32 maxSize, const void * src, const s32 size) {
		OASSERT(size <= maxSize, "memcpy out of range");

		const s32 len = (size > maxSize ? maxSize : size);
		memcpy(dst, src, len);
	}

	inline void __OMemset(void * dst, const s32 maxSize, const s32 val, const s32 size) {
		OASSERT(size <= maxSize, "memset out of range");

		const s32 len = (size > maxSize ? maxSize : size);
		memset(dst, val, len);
	}
#ifdef __cplusplus
};
#endif

#define SafeMemcpy __OMemcpy
#define SafeMemset __OMemset

#include "vulkan/vulkan.h"

#define FLOAT_PRECISION 0.000001f

#endif //__util_h__
