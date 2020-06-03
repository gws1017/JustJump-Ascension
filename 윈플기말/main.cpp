#include <windows.h>
#include <tchar.h>
#include <random>
#include <iostream>
#include "resource.h"
#include "Load.h"
#include "Map.h"
#include "ObjectManager.h"
#include "Camera.h"
//#ifdef _DEBUG
////#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

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
		100, 50, 1024, 768,
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
	static HDC hdc, mem1dc, mem2dc,playerdc,odc; // odc = 오브젝트 dc
	static RECT rectview;
	static HBITMAP hbit1, oldbit1,hbitobj[100];
	static PLAYER player;
	static MAP map;
	static CAMERA camera;
	static OBJECT obj[100];

	static int ocount;		//obj 개수를 세주는 변수
	switch (iMessage)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rectview);
		map.CreateMap(g_hinst);
		
		hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		//hbitobj[0] = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		//obj[0].create(0, 3910, 1023, 3999-3910, 1);
		//obj[1].create(537, 3825, 607-537, 100, 2);
		ocount = initObject(obj,10,g_hinst);
			
		//ocount = 2;
		SetTimer(hwnd, 1,1, NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		if (hbit1 == NULL)
		{
			hbit1 = CreateCompatibleBitmap(hdc, rectview.right, rectview.bottom);
		}

		SelectObject(mem1dc,hbit1);
		

		map.DrawBK(mem1dc, mem2dc, rectview);
		for (int i = 0; i <= ocount; i++)
			obj[i].DrawObj(mem1dc, odc);

		if (player.getstate() == 3)
		{
			Ellipse(mem1dc, player.getx() - player.getw(), player.gety() - player.geth() + player.geth() / 2, player.getx() + player.getw(), player.gety() + player.geth());
		}
		else {
			Ellipse(mem1dc, player.getx() - player.getw(), player.gety() - player.geth(), player.getx() + player.getw(), player.gety() + player.geth());
		}
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
			cout << player.gety() << endl;
			InvalidateRgn(hwnd, NULL, FALSE);
			break;

		}
	case WM_KEYDOWN:
		player.PlayerSetting(wParam);
		break;
	case WM_KEYUP:
		player.PlayerWaiting(wParam);
		break;
	case WM_CHAR:

		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hwnd, iMessage, wParam, lParam));
}










































