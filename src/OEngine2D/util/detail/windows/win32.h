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

#define SafeSprintf sprintf_s
#define SafeVSprintf _vsnprintf
#define CSLEEP(t) Sleep(t)

#pragma comment(lib, "shlwapi.lib")

