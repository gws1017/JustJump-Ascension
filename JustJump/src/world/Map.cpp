#include "world/Map.h"
#include "system/Load.h"
#include "object/view/Camera.h"
#include "object/character/player.h"

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
	hbitbk = LoadBK(hbitbk,g_hinst, mapnum);
	if (mapnum == 9) ms = 0;
}

void MAP::CreateUi(HINSTANCE g_hinst)
{
	hbitui = LoadUi(hbitui, g_hinst);
}

void MAP::CreateHP(HINSTANCE g_hinst)
{
	hbithp = LoadHP(hbithp, g_hinst);
}

void MAP::CreateDie(HINSTANCE g_hinst)
{
	hbitdie = LoadDieNotice(g_hinst);
}

void MAP::CreateStart(HINSTANCE g_hinst)
{
	hbitstart[0] = LoadStart(hbitstart[0],g_hinst,0);
	hbitstart[1] = LoadStart(hbitstart[1], g_hinst,1);
	hbitstart[2] = LoadStart(hbitstart[2], g_hinst,2);
}

void MAP::CreateHelp(HINSTANCE g_hinst)
{
	hbithelp[0] = LoadHelp(hbithelp[0],g_hinst,0);
	hbithelp[1] = LoadHelp(hbithelp[1], g_hinst,1);
}


void MAP::ChangeDieNotice(HINSTANCE g_hinst,int i)
{
	hbitdie = LoadDieNoticeChange(g_hinst, i);
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
	ms += MAPSPEED;
	if (ms >= 3021) ms = 0;
}

void MAP :: DrawBK(HDC& mem1dc, HDC& mem2dc, RECT& rectview)
{
	
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitbk);
	HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(mem1dc, &rectview, blackBrush);
	if (mapnum == 9)
	{
		BitBlt(mem1dc, 0, 0, 3021, 768, mem2dc, ms, 0, SRCCOPY);
		if (ms >= 1997)
		BitBlt(mem1dc, (3021 - ms), 0, rectview.right, 768, mem2dc, 0, 0, SRCCOPY);
	}
	else	//�Ϲ�
	{
		BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, SRCCOPY);	//�� ��ü ���ΰ�ħ
	}

	if (mapnum == 13)
	{
		HFONT hfont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮 light"));
		HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);
		TCHAR count[100];
		TextOut(mem1dc, 100, 3400, L"���� Ƚ�� : ", lstrlenW(L"���� Ƚ�� : "));
		_itow_s(jumpcount, count, 10);
		TextOut(mem1dc, 300, 3400, count, lstrlenW(count));

		TextOut(mem1dc, 500, 3400, L"���� Ƚ�� : ", lstrlenW(L"���� Ƚ�� : "));
		_itow_s(diecount, count, 10);
		TextOut(mem1dc, 700, 3400, count, lstrlenW(count));
		SelectObject(mem1dc, oldfont);
		DeleteObject(hfont);
	}

	DeleteObject(blackBrush);
	DeleteObject(mem2dc);
}

void MAP::DrawLoadBK(HDC& mem1dc, HDC& mem2dc, BLENDFUNCTION bf)
{
	HDC gdidc = CreateCompatibleDC(mem1dc);
	//mem1dc�� ĳ���ͱ׸�������ŭ�� ���´�(���� mem1dc���� ����������Ƿ� 0,0 ���� MAPWIDTH,MAPHEIGHT ������ ��Ʈ���� ��)
	HBITMAP tmpdc = CreateCompatibleBitmap(mem1dc, MAPWIDTH, MAPHEIGHT);
	HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
	//���⼭ 0,0 ~62,50 ������ ��Ʈ���� ĳ���ͱ������� �ٲ��ش� (�÷��̾ �ִ� ��ġ�� ��Ʈ���� ������)
	BitBlt(gdidc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, BLACKNESS);



	if (black_t > 0)	//�� �̵��Ҷ�
	{
		GdiAlphaBlend(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, gdidc, 0, 0, MAPWIDTH, MAPHEIGHT, bf);
		//BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, BLACKNESS);
	}


	SelectObject(gdidc, oldtmpdc);
	DeleteObject(tmpdc);
	DeleteObject(gdidc);
}
//����â
void MAP::DrawUi(HDC& mem1dc, HDC& mem2dc,CAMERA camera)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitui);
	TransparentBlt(mem1dc, camera.GetX()+400, camera.GetY()+700, 199, 65, mem2dc, 0, 0, 199, 65, RGB(0, 255, 0));
	//BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, SRCCOPY);	//Ui ��ü ���ΰ�ħ
	DeleteObject(mem2dc);
}
//HP��
void MAP::DrawHP(HDC& mem1dc, HDC& mem2dc, CAMERA camera,PLAYER player)
{
	int hp = player.gethp() * 171 / 100;
	TCHAR hpname[100];
	_itow_s(player.gethp(), hpname, 10);
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮 light"));
	HFONT oldfont =(HFONT)SelectObject(mem1dc, hfont);
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbithp);
	BitBlt(mem1dc, camera.GetX() + 421, camera.GetY() + 728, hp, 65, mem2dc, 0, 0, SRCCOPY);
	SetBkMode(mem1dc, 1);
	SetTextColor(mem1dc, RGB(0, 0, 0));
	TextOut(mem1dc, camera.GetX() + 481, camera.GetY() + 728, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.GetX() + 483, camera.GetY() + 728, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.GetX() + 482, camera.GetY() + 727, hpname, lstrlenW(hpname));
	TextOut(mem1dc, camera.GetX() + 482, camera.GetY() + 729, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(255, 255, 255));			  
	TextOut(mem1dc, camera.GetX() + 482, camera.GetY() + 728, hpname, lstrlenW(hpname));
	SetTextColor(mem1dc, RGB(0, 0, 0));					  
	TextOut(mem1dc, camera.GetX() + 504, camera.GetY() + 728, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.GetX() + 506, camera.GetY() + 728, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.GetX() + 505, camera.GetY() + 727, L"/100", lstrlenW(L"/100"));
	TextOut(mem1dc, camera.GetX() + 505, camera.GetY() + 729, L"/100", lstrlenW(L"/100"));
	SetTextColor(mem1dc, RGB(255, 255, 255));					 
	TextOut(mem1dc, camera.GetX() + 505, camera.GetY() + 728, L"/100", lstrlenW(L"/100"));
	//StretchBlt(mem1dc, camera.GetX() + 421, camera.GetY() + 728, hp, 65, mem2dc, 0, 0,hp, 65,SRCCOPY);
	//BitBlt(mem1dc, 0, 0, MAPWIDTH, MAPHEIGHT, mem2dc, 0, 0, SRCCOPY);	//HP ��ü ���ΰ�ħ
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);
	DeleteObject(mem2dc);
}

void MAP::DrawDie(HDC& mem1dc, HDC& mem2dc, CAMERA camera, Sound& sound)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitdie);
	TransparentBlt(mem1dc, camera.GetX() + 380, camera.GetY() + 240, 260, 130, mem2dc, 0, 0, 260, 130, RGB(255, 0, 0));
	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮 bold"));
	HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);

	SetTextColor(mem1dc, RGB(32, 108, 168));
	TextOut(mem1dc, camera.GetX() + 459, camera.GetY() + 260, L"Ȯ���� �����ø� ��Ȱ�ϰ� �˴ϴ�.", lstrlenW(L"Ȯ���� �����ø� ��Ȱ�ϰ� �˴ϴ�."));
	TextOut(mem1dc, camera.GetX() + 461, camera.GetY() + 260, L"Ȯ���� �����ø� ��Ȱ�ϰ� �˴ϴ�.", lstrlenW(L"Ȯ���� �����ø� ��Ȱ�ϰ� �˴ϴ�."));
	TextOut(mem1dc, camera.GetX() + 460, camera.GetY() + 259, L"Ȯ���� �����ø� ��Ȱ�ϰ� �˴ϴ�.", lstrlenW(L"Ȯ���� �����ø� ��Ȱ�ϰ� �˴ϴ�."));
	TextOut(mem1dc, camera.GetX() + 460, camera.GetY() + 261, L"Ȯ���� �����ø� ��Ȱ�ϰ� �˴ϴ�.", lstrlenW(L"Ȯ���� �����ø� ��Ȱ�ϰ� �˴ϴ�."));

	SetTextColor(mem1dc, RGB(255, 255, 255));
	TextOut(mem1dc, camera.GetX() + 460, camera.GetY() + 260, L"Ȯ���� �����ø� ��Ȱ�ϰ� �˴ϴ�.", lstrlenW(L"Ȯ���� �����ø� ��Ȱ�ϰ� �˴ϴ�."));
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);

	hfont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮 light"));
	oldfont = (HFONT)SelectObject(mem1dc, hfont);
	TextOut(mem1dc, camera.GetX() + 460, camera.GetY() + 285, L"�ƽ����� Ȯ���� �����ø� ���� �� ó��", lstrlenW(L"�ƽ����� Ȯ���� �����ø� ���� �� ó��."));
	TextOut(mem1dc, camera.GetX() + 460, camera.GetY() + 297, L"��ġ�� ���ư��� �˴ϴ�. �������� ������!", lstrlenW(L"��ġ�� ���ư��� �˴ϴ�. �������� ������!"));



	DeleteObject(mem2dc);



}

void MAP::DrawStart(HDC& mem1dc, HDC& mem2dc,int i)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbitstart[i]);
	TransparentBlt(mem1dc, 292, 490, 138, 82, mem2dc, 0, 0,138,82,RGB(255,0,0));
	

	DeleteObject(mem2dc);



}
void MAP::DrawHelp(HDC& mem1dc, HDC& mem2dc, int i)
{
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, hbithelp[i]);
	if (i == 0)
	{
		TransparentBlt(mem1dc, 290, 345, 138,82, mem2dc, 0, 0, 138, 82, RGB(60, 60, 60));
	}
	
	else if(i == 1) TransparentBlt(mem1dc, 215,300, 400, 200, mem2dc, 0, 0, 400, 200, RGB(0, 0, 0));

	DeleteObject(mem2dc);

}
