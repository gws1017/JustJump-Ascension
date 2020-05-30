#include "Map.h"
#include <Windows.h>
#include "Load.h"

void Map::CreateMap(HINSTANCE g_hinst)
{
	hbitbk = LoadBK(hbitbk,g_hinst);
}

void Map :: DrawBK(HDC& mem1dc, HDC& mem2dc, RECT& rectview)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem1dc, hbitbk);
	BitBlt(mem1dc, rectview.left, rectview.top, rectview.right, rectview.bottom, mem2dc, 0,0, SRCCOPY);
	DeleteObject(mem2dc);
}

