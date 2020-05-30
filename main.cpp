#include <windows.h>
#include <tchar.h>
#include <random>
#include <iostream>
#include "resource.h"
#include "player.h"
#include "Load.h"
#include "Map.h"
#include "object.h"
//#ifdef _DEBUG
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

HINSTANCE g_hinst;
LPCTSTR lpszClass = L"windows program 2-2";
LPCTSTR lpszWinodwName = L"windows progragm 2-2";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);



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
		100, 50, 1024 ,768,
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
	static HDC hdc, mem1dc,mem2dc;
	static RECT rectview;
	static HBITMAP hbit1, oldbit1;
	static PLAYER player;
	static Map map;
	static object obj[100];

	static int ocount;
	switch (iMessage)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rectview);
		map.CreateMap(g_hinst);
		hbit1 = (HBITMAP)LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_BITMAP1));

		obj[0].create(0, 3910, 1023, 3999,1);
		obj[1].create(537, 3825, 607, 3825,2);
		ocount = 0;

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		map.DrawBK(mem1dc, mem2dc, rectview);

		BitBlt(hdc, 0, 0, 1024, 768, mem1dc, 0, 4000-768, SRCCOPY);
		
		DeleteObject(mem1dc);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT)
		{
			player.setx(player.getx() - 1);
			break;
		}
		if (wParam == VK_RIGHT)
		{
			player.setx(player.getx() + 1);
			break;
		}
		if (wParam == VK_UP)
		{
			player.sety(player.gety() - 1);
			break;
		}
		if (wParam == VK_DOWN)
		{
			player.sety(player.gety() + 1);
			break;
		}
		break;
	case WM_CHAR:

		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hwnd, iMessage, wParam, lParam));
}










































