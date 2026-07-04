#include "Global.h"
#include "world/Map.h"

#include "object/view/Camera.h"
#include "object/character/player.h"


//void MAP::CreateBlack(HINSTANCE g_hinst)
//{
//	hbitbk = LoadBlack(hbitbk, g_hinst);
//}

void MAP::CreateMap(HINSTANCE g_hinst)
{
	hbitbk = CreateBmpPtr(LoadBK(hbitbk.get(), g_hinst, mapnum));
	if (mapnum == static_cast<int>(EMapId::Title)) ms = 0;
}

void MAP::CreateUi(HINSTANCE g_hinst)
{
	hbitui = CreateBmpPtr(LoadUi(hbitui.get(), g_hinst));
}

void MAP::CreateHP(HINSTANCE g_hinst)
{
	hbithp = CreateBmpPtr(LoadHP(hbithp.get(), g_hinst));
}


bool MAP::BlackTime()
{
	if (black_t > 0) {
		black_t--; 
		return true;
	}
	return false;
}

void MAP::movemap()
{
	ms += GameConst::kMapScrollSpeed;
	if (ms >= 3021) ms = 0;
}

void MAP :: DrawBK(HDC& mem1dc, HDC& mem2dc, RECT& rectview, const PLAYER& player)
{
	
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitbk.get());
	HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(mem1dc, &rectview, blackBrush);
	if (mapnum == static_cast<int>(EMapId::Title))
	{
		BitBlt(mem1dc, 0, 0, 3021, 768, mem2dc, ms, 0, SRCCOPY);
		if (ms >= 1997)
		BitBlt(mem1dc, (3021 - ms), 0, rectview.right, 768, mem2dc, 0, 0, SRCCOPY);
	}
	else	//일반
	{
		BitBlt(mem1dc, 0, 0, GameConst::kViewportWidth, GameConst::kMapBitmapHeight, mem2dc, 0, 0, SRCCOPY);	//맵 전체 새로고침
	}

	if (mapnum == static_cast<int>(EMapId::Clear))
	{
		HFONT hfont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 light"));
		HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);
		TCHAR count[100];
		TextOut(mem1dc, 100, 3400, L"점프 횟수 : ", lstrlenW(L"점프 횟수 : "));
		_itow_s(player.GetJumpCount(), count, 10);
		TextOut(mem1dc, 300, 3400, count, lstrlenW(count));

		TextOut(mem1dc, 500, 3400, L"죽은 횟수 : ", lstrlenW(L"죽은 횟수 : "));
		_itow_s(player.GetDieCount(), count, 10);
		TextOut(mem1dc, 700, 3400, count, lstrlenW(count));
		SelectObject(mem1dc, oldfont);
		DeleteObject(hfont);
	}

	DeleteObject(blackBrush);
	DeleteDC(mem2dc);
}

void MAP::DrawLoadBK(HDC& mem1dc, HDC& mem2dc, BLENDFUNCTION bf)
{
	HDC gdidc = CreateCompatibleDC(mem1dc);
	//mem1dc의 캐릭터그릴공간만큼만 얻어온다(실제 mem1dc에는 배경이있으므로 0,0 부터 GameConst::kViewportWidth,GameConst::kMapBitmapHeight 까지의 비트맵이 들어감)
	HBITMAP tmpdc = CreateCompatibleBitmap(mem1dc, GameConst::kViewportWidth, GameConst::kMapBitmapHeight);
	HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
	//여기서 0,0 ~62,50 까지의 비트맵을 캐릭터기준으로 바꿔준다 (플레이어가 있는 위치의 비트맵을 복사함)
	BitBlt(gdidc, 0, 0, GameConst::kViewportWidth, GameConst::kMapBitmapHeight, mem2dc, 0, 0, BLACKNESS);

	if (black_t > 0)	//맵 이동할때
	{
		GdiAlphaBlend(mem1dc, 0, 0, GameConst::kViewportWidth, GameConst::kMapBitmapHeight, gdidc, 0, 0, GameConst::kViewportWidth, GameConst::kMapBitmapHeight, bf);
		//BitBlt(mem1dc, 0, 0, GameConst::kViewportWidth, GameConst::kMapBitmapHeight, mem2dc, 0, 0, BLACKNESS);
	}

	SelectObject(gdidc, oldtmpdc);
	DeleteObject(tmpdc);
	DeleteDC(gdidc);
}
//상태창
void MAP::DrawUi(HDC& mem1dc, HDC& mem2dc,CAMERA camera)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitui.get());
	TransparentBlt(mem1dc, camera.GetX()+400, camera.GetY()+660, 199, 65, mem2dc, 0, 0, 199, 65, RGB(0, 255, 0));
	//BitBlt(mem1dc, 0, 0, GameConst::kViewportWidth, GameConst::kMapBitmapHeight, mem2dc, 0, 0, SRCCOPY);	//Ui 전체 새로고침
	DeleteDC(mem2dc);
}
//HP바
void MAP::DrawHP(HDC& mem1dc, HDC& mem2dc, CAMERA camera, PLAYER& player)
{
	int hp = player.GetCurrentHP() * 171 / 100;
	TCHAR hpname[100];
	_itow_s(player.GetCurrentHP(), hpname, 10);
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 light"));
	HFONT oldfont =(HFONT)SelectObject(mem1dc, hfont);
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbithp.get());
	BitBlt(mem1dc, camera.GetX() + 421, camera.GetY() + 688, hp, 13, mem2dc, 0, 0, SRCCOPY);
	SetBkMode(mem1dc, 1);
	SetTextColor(mem1dc, RGB(0, 0, 0));
	TextOut(mem1dc, camera.GetX() + 481, camera.GetY() + 688, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.GetX() + 483, camera.GetY() + 688, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.GetX() + 482, camera.GetY() + 687, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.GetX() + 482, camera.GetY() + 689, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(255, 255, 255));
	TextOut(mem1dc, camera.GetX() + 482, camera.GetY() + 688, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(0, 0, 0));
	TextOut(mem1dc, camera.GetX() + 504, camera.GetY() + 688, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.GetX() + 506, camera.GetY() + 688, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.GetX() + 505, camera.GetY() + 687, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.GetX() + 505, camera.GetY() + 689, L"/100", lstrlenW(L"/100"));
	SetTextColor(mem1dc, RGB(255, 255, 255));
	TextOut(mem1dc, camera.GetX() + 505, camera.GetY() + 688, L"/100", lstrlenW(L"/100"));
	//StretchBlt(mem1dc, camera.GetX() + 421, camera.GetY() + 688, hp, 13, mem2dc, 0, 0,hp, 13,SRCCOPY);
	//BitBlt(mem1dc, 0, 0, GameConst::kViewportWidth, GameConst::kMapBitmapHeight, mem2dc, 0, 0, SRCCOPY);	//HP 전체 새로고침
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);
	DeleteDC(mem2dc);
}

