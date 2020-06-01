#include "Map.h"
#include <Windows.h>
#include "Load.h"

void MAP::CreateMap(HINSTANCE g_hinst)
{
	hbitbk = LoadBK(hbitbk,g_hinst);
}

void MAP :: DrawBK(HDC& mem1dc, HDC& mem2dc, RECT& rectview)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitbk);
	BitBlt(mem1dc, 0, 0,MAPWIDTH, MAPHEIGHT, mem2dc, 0,0, SRCCOPY);	//맵 전체 새로고침
	DeleteObject(mem2dc);
}

