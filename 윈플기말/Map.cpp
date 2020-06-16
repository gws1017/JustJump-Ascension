#include "Map.h"
#include "Load.h"
void MAP::CreateMap(HINSTANCE g_hinst)
{
	hbitbk = LoadBK(hbitbk,g_hinst);
}

void MAP::CreateUi(HINSTANCE g_hinst)
{
	hbitui = LoadUi(hbitbk, g_hinst);
}

void MAP::CreateHP(HINSTANCE g_hinst)
{
	hbithp = LoadHP(hbitbk, g_hinst);
}

void MAP :: DrawBK(HDC& mem1dc, HDC& mem2dc, RECT& rectview)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitbk);
	BitBlt(mem1dc, 0, 0,MAPWIDTH, MAPHEIGHT, mem2dc, 0,0, SRCCOPY);	//맵 전체 새로고침
	DeleteObject(mem2dc);
}
//상태창
void MAP::DrawUi(HDC& mem1dc, HDC& mem2dc,CAMERA camera)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitui);
	TransparentBlt(mem1dc, camera.getx()+400, camera.gety()+700, 199, 65, mem2dc, 0, 0, 199, 65, RGB(0, 255, 0));
	//BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, SRCCOPY);	//Ui 전체 새로고침
	DeleteObject(mem2dc);
}
//HP바
void MAP::DrawHP(HDC& mem1dc, HDC& mem2dc, CAMERA camera,PLAYER player)
{
	int hp = player.gethp() * 171 / 100;
	TCHAR hpname[100];
	_itow_s(player.gethp(), hpname, 10);
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 light"));
	HFONT oldfont =(HFONT)SelectObject(mem1dc, hfont);
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbithp);
	BitBlt(mem1dc, camera.getx() + 421, camera.gety() + 728, hp, 65, mem2dc, 0, 0, SRCCOPY);
	SetBkMode(mem1dc, 1);
	SetTextColor(mem1dc, RGB(0, 0, 0));
	TextOut(mem1dc, camera.getx() + 481, camera.gety() + 728, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.getx() + 483, camera.gety() + 728, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.getx() + 482, camera.gety() + 727, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.getx() + 482, camera.gety() + 729, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(255, 255, 255));			  
	TextOut(mem1dc, camera.getx() + 482, camera.gety() + 728, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(0, 0, 0));					  
	TextOut(mem1dc, camera.getx() + 504, camera.gety() + 728, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.getx() + 506, camera.gety() + 728, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.getx() + 505, camera.gety() + 727, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.getx() + 505, camera.gety() + 729, L"/100", lstrlenW(L"/100"));
	SetTextColor(mem1dc, RGB(255, 255, 255));					 
	TextOut(mem1dc, camera.getx() + 505, camera.gety() + 728, L"/100", lstrlenW(L"/100"));
	//StretchBlt(mem1dc, camera.getx() + 421, camera.gety() + 728, hp, 65, mem2dc, 0, 0,hp, 65,SRCCOPY);
	//BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, SRCCOPY);	//HP 전체 새로고침
	DeleteObject(oldfont);
	DeleteObject(mem2dc);
}


