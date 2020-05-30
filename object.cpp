#include "object.h"

int object::getX()
{
	return x;
}
int object::getY()
{
	return y;
}
int object::getW()
{
	return w;
}
int object::getH() {
	return y;
}

int object::getType()
{
	return type;
}
void object::create(int _x, int _y, int _w, int _h, int _type)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	type = _type;
}
void object::setX(int _x)
{
	x = _x;
}
void object::setY(int _y)
{
	y = _y;
}
void object::setW(int _w)
{
	w = _w;
}
void object::setH(int _h)
{
	h = _h;
}
void object::setType(int _type)
{
	type = _type;
}
void object::setHbit(HBITMAP _hbit)
{
	hbit = _hbit;
}