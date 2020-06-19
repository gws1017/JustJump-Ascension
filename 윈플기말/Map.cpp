#include "Map.h"
#include "Load.h"
int MAP::getmapnum() { return mapnum; }

int MAP::getblack_t() { return black_t; }

void MAP::setmapnum(int i) { mapnum = i; }

void MAP::setblack_t(int i) { black_t = i; }

//void MAP::CreateBlack(HINSTANCE g_hinst)
//{
//	hbitbk = LoadBlack(hbitbk, g_hinst);
//}

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

bool MAP::BlackTime()
{
	if (black_t > 0) {
		black_t--; 
		return true;
	}
	return false;
}


void MAP :: DrawBK(HDC& mem1dc, HDC& mem2dc, RECT& rectview)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitbk);
	FillRect(mem1dc, &rectview, RGB(0, 0, 0));
	if (0 >= black_t)	//일반
	{
		BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, SRCCOPY);	//맵 전체 새로고침
	}

	

	DeleteObject(mem2dc);
}

void MAP::DrawLoadBK(HDC& mem1dc, HDC& mem2dc, BLENDFUNCTION bf)
{
	HDC gdidc = CreateCompatibleDC(mem1dc);
	//mem1dc의 캐릭터그릴공간만큼만 얻어온다(실제 mem1dc에는 배경이있으므로 0,0 부터 MAPWIDTH,MAPHEIGHT 까지의 비트맵이 들어감)
	HBITMAP tmpdc = CreateCompatibleBitmap(mem1dc, MAPWIDTH, MAPHEIGHT);
	HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
	//여기서 0,0 ~62,50 까지의 비트맵을 캐릭터기준으로 바꿔준다 (플레이어가 있는 위치의 비트맵을 복사함)
	BitBlt(gdidc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, BLACKNESS);



	if (black_t > 0)	//맵 이동할때
	{
		GdiAlphaBlend(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, gdidc, 0, 0, MAPWIDTH, MAPHEIGHT, bf);
		//BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, BLACKNESS);
	}


	SelectObject(gdidc, oldtmpdc);
	DeleteObject(tmpdc);
	DeleteObject(gdidc);
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
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);
	DeleteObject(mem2dc);
}


