#pragma once
#include <windows.h>
#include "player.h"

class OBJECT {
	int x, y;					//오브젝트의 x y 는 왼쪽상단이 기준이다.
	int w, h;
	int type;					//1~100 플랫폼 101~장애물
	HBITMAP hbit;
public:
	//오브젝트의 x y 는 왼쪽상단이 기준이다.
	int getX();
	//오브젝트의 x y 는 왼쪽상단이 기준이다.
	int getY();
	int getW();
	int getH();
	//1~100 플랫폼 101~장애물
	int getType();
	//오브젝트가 몇개들어갔는지
	int getocount();

	void create(int, int, int, int, int);
	//오브젝트의 x y 는 왼쪽상단이 기준이다.
	void setX(int);
	//오브젝트의 x y 는 왼쪽상단이 기준이다.
	void setY(int);
	void setW(int);
	void setH(int);
	//1~100 플랫폼 101~장애물
	void setType(int);
	void setHbit(HINSTANCE);

	void DrawObj(HDC&, HDC&);
};

//bool collp2w(PLAYER, OBJECT);