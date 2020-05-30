#pragma once
#include <windows.h>

class object {
	int x, y;
	int w, h;
	int type;
	HBITMAP hbit;
public:
	int getX();
	int getY();
	int getW();
	int getH();
	int getType();

	void create(int, int, int, int, int);
	void setX(int);
	void setY(int);
	void setW(int);
	void setH(int);
	void setType(int);
	void setHbit(HBITMAP);
};