#pragma once

#include <windows.h>
#include <stdio.h>
#include <wincrypt.h>
#include <Shlwapi.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <process.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef char s8;
typedef short s16;
typedef int32_t s32;
typedef int64_t s64;

#define SafeSprintf sprintf_s
#define SafeVSprintf _vsnprintf
#define CSLEEP(t) Sleep(t)

#pragma comment(lib, "shlwapi.lib")

