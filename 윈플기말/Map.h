#pragma once
#include <windows.h>
#include "Camera.h"
#include "player.h"
#define MAPWIDTH 1024
#define MAPHEIGHT 4098
class MAP {
	int mapnum = 10;
	int black_t = 0;
	HBITMAP hbitbk;
	HBITMAP hbitui;
	HBITMAP hbithp;
public:
	//맵번호를 읽음 10~
	int getmapnum();
	//Black Screen Time get
	int getblack_t();

	//맵번호를 바꿈
	void setmapnum(int);
	//Black Screen Time set
	void setblack_t(int);

	//For 2 sec Screen hide
	bool BlackTime();
	void DrawBK(HDC&, HDC&, RECT&);
	//서서히 거매지는 것 구현
	void DrawLoadBK(HDC&, HDC&, BLENDFUNCTION);
	//상태창
	void DrawUi(HDC&, HDC&, CAMERA);
	//HP바
	void DrawHP(HDC&, HDC&, CAMERA,PLAYER);
	/*void CreateBlack(HINSTANCE)*/;
	void CreateMap(HINSTANCE);
	void CreateUi(HINSTANCE);
	void CreateHP(HINSTANCE);
};
