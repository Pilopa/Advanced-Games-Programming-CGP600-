#pragma once

#include <Windows.h>
#include <string>

#define LogError(message) OutputDebugStringA(message);
#define LogInfo(message) OutputDebugStringA(message);