#pragma once
#include <windows.h>

class Map {
	HBITMAP hbitbk;
public:
	
	void DrawBK(HDC&, HDC&, RECT&);
	void CreateMap(HINSTANCE);
};
