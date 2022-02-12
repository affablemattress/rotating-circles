#pragma once
#include <stdlib.h>
#include <math.h>
#include "include/raylib.h"

typedef struct Circle {
	Color color;
	float radius;
	float rodLength;
	float w;
	float wPosition;
	float x;
	float y;
}Circle;

Circle* CircleCreate(Color color, float radius, float rodLength, float w, float parentX, float parentY);
