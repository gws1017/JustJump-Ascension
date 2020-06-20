#pragma once
#include <Windows.h>

class CAMERA{
	int x, y;
public:
	CAMERA();
	void setx(int);
	void sety(int);

	int getx();
	int gety();
	void CameraSetting(WPARAM);
};