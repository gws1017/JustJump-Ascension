#pragma once
#include <windows.h>
#include "Camera.h"
#include "player.h"
#define MAPWIDTH 1024
#define MAPHEIGHT 4098
class MAP {
	HBITMAP hbitbk;
	HBITMAP hbitui;
	HBITMAP hbithp;
public:
	
	void DrawBK(HDC&, HDC&, RECT&);
	//상태창
	void DrawUi(HDC&, HDC&, CAMERA);
	//HP바
	void DrawHP(HDC&, HDC&, CAMERA,PLAYER);
	void CreateMap(HINSTANCE);
	void CreateUi(HINSTANCE);
	void CreateHP(HINSTANCE);
};
