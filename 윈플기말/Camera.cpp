#include "Camera.h"

CAMERA::CAMERA()
{
	x = 0;
	y = 3232;
}

void CAMERA::setx(int _x)
{
	x = _x;
}

void CAMERA::sety(int _y)
{
	y = _y;
}

int CAMERA::getx()
{
	return x;
}

int CAMERA::gety()
{
	return y;
}



