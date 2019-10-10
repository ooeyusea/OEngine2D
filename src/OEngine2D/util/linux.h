#ifndef __LINUX_H__
#define __LINUX_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <dlfcn.h>
#include <limits.h>
#include <libgen.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef char s8;
typedef short s16;
typedef int32_t s32;
typedef int64_t s64;

#define SafeSprintf snprintf
#define  SafeVSprintf vsnprintf
#define CSLEEP(t) usleep(t)

#define MAX_PATH 260

#endif //__LINUX_H__
