#include "object.h"



int OBJECT::getX()
{
	return x;
}
int OBJECT::getY()
{
	return y;
}
int OBJECT::getW()
{
	return w;
}
int OBJECT::getH() {
	return y;
}

int OBJECT::getType()
{
	return type;
}
void OBJECT::create(int _x, int _y, int _w, int _h, int _type)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	type = _type;
}
void OBJECT::setX(int _x)
{
	x = _x;
}
void OBJECT::setY(int _y)
{
	y = _y;
}
void OBJECT::setW(int _w)
{
	w = _w;
}
void OBJECT::setH(int _h)
{
	h = _h;
}
void OBJECT::setType(int _type)
{
	type = _type;
}
void OBJECT::setHbit(HBITMAP _hbit)
{
	hbit = _hbit;
}

//땅바닥과 플레이어 충돌체크 1이면 부닥침
//
//bool collp2w(PLAYER player, OBJECT obj)
//{
//	if (obj.getX() <= player.getx() && player.getx() + player.getw() <= obj.getX() + obj.getW())
//	{
//		if (obj.getY() <= player.gety()+player.geth())
//		{
//			return 1;
//		}
//	}
//	return 0;
//	/*if (player.getx() + player.getw() < obj.getX() || obj.getX() + obj.getW() < player.getx()) return 0;
//	if (player.gety() + player.geth() < obj.getY() || obj.getY() + obj.getH() < player.gety())return 0;
//
//	return 1;
//*/
//
//}