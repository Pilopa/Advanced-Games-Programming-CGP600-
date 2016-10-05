#include "MyMath.h"
#include <windows.h>

float Clamp(float f, float lower, float upper) {
	return max(lower, min(f, upper));
}