#pragma once

#include <Windows.h>

#define LogError(message) OutputDebugStringA(message);
#define LogInfo(message) OutputDebugStringA(message);