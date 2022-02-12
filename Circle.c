#include "Circle.h"

Circle* CircleCreate(Color color, float radius, float rodLength, float w, float parentX, float parentY) {
	Circle* pCreatedCircle = malloc(sizeof(Circle));
	pCreatedCircle->color = color;
	pCreatedCircle->radius = radius;
	pCreatedCircle->rodLength = rodLength;
	pCreatedCircle->w = w * (PI / 180);
	pCreatedCircle->wPosition = 0;
	pCreatedCircle->x = parentX + rodLength * cos(pCreatedCircle->wPosition);
	pCreatedCircle->y = parentY + rodLength * sin(pCreatedCircle->wPosition);
	return pCreatedCircle;
}
