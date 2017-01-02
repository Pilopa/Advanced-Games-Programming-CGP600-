#pragma once

#include <Windows.h>
#include <string>

#define LogError(message) OutputDebugStringA(message); //OutputDebugStringA("\n");
#define LogInfo(message) OutputDebugStringA(message); OutputDebugStringA("\n");