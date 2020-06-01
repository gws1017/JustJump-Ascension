#pragma once
#include <windows.h>
#define MAPWIDTH 1024
#define MAPHEIGHT 4000
class MAP {
	HBITMAP hbitbk;
public:
	
	void DrawBK(HDC&, HDC&, RECT&);
	void CreateMap(HINSTANCE);
};
