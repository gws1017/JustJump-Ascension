#pragma once
#include <windows.h>
#include "player.h"

class OBJECT {
	int x, y;
	int w, h;
	int type;					//1~100 ÇÃ·§Æû 101~Àå¾Ö¹°
	HBITMAP hbit;
public:
	int getX();
	int getY();
	int getW();
	int getH();
	//1~100 ÇÃ·§Æû 101~Àå¾Ö¹°
	int getType();
	//¿ÀºêÁ§Æ®°¡ ¸î°³µé¾î°¬´ÂÁö
	int getocount();

	void create(int, int, int, int, int);
	void setX(int);
	void setY(int);
	void setW(int);
	void setH(int);
	//1~100 ÇÃ·§Æû 101~Àå¾Ö¹°
	void setType(int);
	void setHbit(HINSTANCE);

	void DrawObj(HDC&, HDC&);
};

//bool collp2w(PLAYER, OBJECT);