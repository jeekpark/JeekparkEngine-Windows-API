#pragma once


#include <Windows.h>
#include <mmsystem.h>
#include <dinput.h>
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "winmm.lib")

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <cassert>
#include <functional>
#include <filesystem>
#include <bitset>
#include <stack>
#include <queue>
#include <mutex>
#include <thread>

#include "jkMath.h"
#include "jkEnums.h"

#define CAST_UINT(value) static_cast<UINT>(value)
#define CAST_FLOAT(value) static_cast<float>(value)