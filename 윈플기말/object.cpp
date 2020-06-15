#pragma comment (lib, "Msimg32.lib")
#include "object.h"
#include "Load.h"


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
	return h;
}

int OBJECT::getType()
{
	return type;
}
int OBJECT::getindex()
{
	return index;
}
void OBJECT::create(int _x, int _y, int _w, int _h, int _type)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	type = _type;
	index = 0;
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
void OBJECT::setHbit(HINSTANCE g_hinst)
{
	hbit = LoadObj(hbit, g_hinst, type);

}

void OBJECT::IndexChange()
{
	index += 1;
	if (index >= 4) index = 0;
}

void OBJECT::DrawObj(HDC& mem1dc, HDC& odc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit);
	if (type == 1) TransparentBlt(mem1dc, x, y, w, h, odc, 0, 0, 1023, 62, RGB(255, 255, 255));
	if (type == 2) TransparentBlt(mem1dc, x, y, w, h + 17, odc, 11, 15, 77, 18, RGB(255, 255, 255));	// 원본그림에서 x 11~88 y 15 33 만큼 잘라내서 투명처리후 출력
	if (type == 3) TransparentBlt(mem1dc, x, y, w, h + 18, odc, 0, 0, 19, 19, RGB(255, 255, 255));
	if (type == 101) TransparentBlt(mem1dc, x, y, w, h, odc, 1, 0, 26, 15, RGB(255, 255, 255));
	if (type == 102) TransparentBlt(mem1dc, x, y, w, h, odc, 0, 1, 17, 75, RGB(255, 255, 255));
	if (type == 103) // 103번의 경우 102번의 y값에서 51을 뺀 위치가 파이프 깨진부분이다.
	{
		//그림의 크기가 각각 다르기때문에 임시변수를 만들어  값을 저장하고 인덱스에따라 바꿔주는 형식이다.
		RECT gas[4] = { {0,2,7,7},{10,2,18,7},{31,0,33,9},{0,0,0,0} }; //1번째인자는 아무것도 안그리는것
		int tx;
		if (index == 0) tx = x + 26;
		else if (index == 1) tx = x + 15;
		else if (index == 2) tx = x;
		else tx = 0;
		TransparentBlt(mem1dc, tx, y, gas[index].right, gas[index].bottom, odc, gas[index].left, gas[index].top, gas[index].right, gas[index].bottom, RGB(255, 255, 255));
	}
	DeleteObject(odc);
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