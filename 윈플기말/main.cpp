#include <windows.h>
#include <tchar.h>
#include <random>
#include <iostream>
#include "resource.h"
#include "Load.h"
#include "Map.h"
#include "ObjectManager.h"
#include "Camera.h"
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

HINSTANCE g_hinst;
LPCTSTR lpszClass = L"Just Jump";
LPCTSTR lpszWinodwName = L"Just Jump";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//extern int ROWSPEED;

//extern int COLSPEED;

//한줄에 79자까지 입력가능한 메모장

using namespace std;
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hinst = hinstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hinstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);


	hWnd = CreateWindow
	(
		lpszClass, lpszWinodwName,
		WS_OVERLAPPEDWINDOW,
		100, 50, 1040, 807,
		NULL, (HMENU)NULL,
		hinstance, NULL
	);


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;


}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	static HDC hdc, mem1dc, mem2dc,playerdc,odc,pdc,ui_dc,hp_dc; // odc = 오브젝트 dc, pdc = player dc,ui_Dc : 아래 전체적인 ui hp_Dc: hp통만 나오는거
	static RECT rectview;
	static HBITMAP hbit1, oldbit1,hbitobj[100],Uibit,HPbit;
	static PLAYER player;
	static MAP map;
	static CAMERA camera;
	static OBJECT obj[100];
	static int gas_t = 0; //가스 애니메이션을 1번타이머에 넣기위해 추가한 변수
	static int ocount;		//obj 개수를 세주는 변수
	switch (iMessage)
	{
	case WM_CREATE:
		AddFontResourceA("font/Maplestory Bold.ttf");
		AddFontResourceA("font/Maplestory Light.ttf");
		GetClientRect(hwnd, &rectview);
		map.CreateMap(g_hinst);
		map.CreateUi(g_hinst);
		map.CreateHP(g_hinst);
		player.setBit(g_hinst);
		player.initBitPos();
		hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Uibit = (HBITMAP)LoadImage(g_hinst, TEXT("img/Ui.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		HPbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/Ui_HP.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		//hbitobj[0] = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		//obj[0].create(0, 3910, 1023, 3999-3910, 1);
		//obj[1].create(537, 3825, 607-537, 100, 2);
		ocount = initObject(obj,10,g_hinst);
			
		//ocount = 2;
		SetTimer(hwnd, 1,1, NULL);
		SetTimer(hwnd, 2,100, NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		if (hbit1 == NULL)
		{
			hbit1 = CreateCompatibleBitmap(hdc, rectview.right, rectview.bottom);
		}

		SelectObject(mem1dc,hbit1);
		SelectObject(ui_dc, Uibit);
		SelectObject(hp_dc, HPbit);

		map.DrawBK(mem1dc, mem2dc, rectview);
		

		for (int i = 0; i <= ocount; i++)
			obj[i].DrawObj(mem1dc, odc);

		player.draw(mem1dc, pdc);

		/*if (player.getstate() == 3)
		{
			Ellipse(mem1dc, player.getx() - player.getw(), player.gety() - player.geth() + player.geth() / 2, player.getx() + player.getw(), player.gety() + player.geth());
		}
		else {
			Ellipse(mem1dc, player.getx() - player.getw(), player.gety() - player.geth(), player.getx() + player.getw(), player.gety() + player.geth());
		}*/
		map.DrawUi(mem1dc, ui_dc, camera);
		map.DrawHP(mem1dc, hp_dc, camera, player);
		//cout << "현재HP상태: " << player.gethp() << endl;
		BitBlt(hdc, 0, 0, 1024, 768, mem1dc, camera.getx(), camera.gety(), SRCCOPY);

		DeleteObject(mem1dc);
		EndPaint(hwnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			player.move();

			adjustPlayer(player, obj, ocount);
			adjustCamera(camera, player);

			player.selectBit();	
			player.stealthtime();
			player.spike_hurttime();
			gas_t += 1;

			for (int i = 0; i <= ocount; i++)
			{
				if (obj[i].getType() == 103)
				{
					if (gas_t >= 30)
					{
						obj[i].IndexChange();
						gas_t = 0;
					}
					
				}
			}

			
			InvalidateRgn(hwnd, NULL,FALSE);
			break;
		case 2:
			player.BitMove();
			InvalidateRgn(hwnd, NULL, FALSE);
			break;
		}
		break;
	case WM_KEYDOWN:
		
		player.PlayerSetting(wParam);
		InvalidateRgn(hwnd, NULL, FALSE);
		break;
	case WM_KEYUP:
		player.PlayerWaiting(wParam);
		InvalidateRgn(hwnd, NULL, FALSE);
		break;
	case WM_CHAR:
		if (wParam == 'r')
		{
			player.setx(0);
			player.sety(300);
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hwnd, iMessage, wParam, lParam));
}










































