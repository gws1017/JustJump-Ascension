#include <windows.h>
#include <tchar.h>
#include <random>
#include <iostream>
#include "resource.h"
#include "Load.h"
#include "Map.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "Sound.h"
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
	WndClass.hCursor = LoadCursorFromFile(TEXT("cursor/cursor2.cur"));
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
	static HDC hdc, mem1dc, mem2dc, loaddc, playerdc, odc, pdc, ui_dc, hp_dc,die_dc,start_dc,help_dc; // odc = 오브젝트 dc, pdc = player dc,ui_Dc : 아래 전체적인 ui hp_Dc: hp통만 나오는거 dic_dc : 사망 ui 
	static RECT rectview;
	static HBITMAP hbit1,loadbit,oldload, oldbit1, hbitobj[100], Uibit, HPbit,Diebit,Startbit,Helpbit;
	static PLAYER player;
	static MAP map;
	static CAMERA camera;
	static OBJECT obj[100];
	static BLENDFUNCTION loadbf;
	static Sound sound;
	
	static int obj_t = 0; //오브젝트 애니메이션을 1번타이머에 넣기위해 추가한 변수
	static int ocount;		//obj 개수를 세주는 변수
	static int help_button = 0; //조작법 온오프
	static bool occur_button = 0;	//사망했을때의 button이 활성화되었는지 
	static bool gamemode = 0;	//0이면 기본 1이면 자유모드
	switch (iMessage)
	{
	case WM_CREATE:

		

		AddFontResourceA("font/Maplestory Bold.ttf");
		AddFontResourceA("font/Maplestory Light.ttf");
		GetClientRect(hwnd, &rectview);
		map.CreateMap(g_hinst);
		map.CreateUi(g_hinst);
		map.CreateHP(g_hinst);
		map.CreateDie(g_hinst);
		map.CreateStart(g_hinst);
		player.setBit(g_hinst);
		player.initBitPos();

		camera.setx(0);
		camera.sety(0);
		cout << camera.getx() << endl;
		sound.Sound_Setup();
		loadbf.AlphaFormat = 0;
		loadbf.BlendFlags = 0;
		loadbf.BlendOp = AC_SRC_OVER;
		loadbf.SourceConstantAlpha = 0;	//검은창으로 서서히 바뀌게 될 예정
		//loaddc = CreateCompatibleDC(mem1dc);
		FMOD_Channel_Stop(sound.Channel[0]);
		FMOD_System_PlaySound(sound.System, sound.bgmSound[0], NULL, 0, &sound.Channel[0]);
		
		if(map.getmapnum() == 9) hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/start_rayer1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		
		//else if (map.getblack_t() > 0)hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk_black.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Helpbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/help1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Startbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/start1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Uibit = (HBITMAP)LoadImage(g_hinst, TEXT("img/Ui.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		HPbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/Ui_HP.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Diebit = (HBITMAP)LoadImage(g_hinst, TEXT("img/Notice3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		//hbitobj[0] = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		//obj[0].create(0, 3910, 1023, 3999-3910, 1);
		//obj[1].create(537, 3825, 607-537, 100, 2);
		ocount = initObject(obj, 9, g_hinst);

		//ocount = 2;
		SetTimer(hwnd, 1, 1, NULL);
		SetTimer(hwnd, 2, 100, NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		mem1dc = CreateCompatibleDC(hdc);
		if (hbit1 == NULL)
		{
			hbit1 = CreateCompatibleBitmap(hdc, rectview.right, rectview.bottom);
		}

		if (map.getmapnum() == 10)cout << ocount << endl;
		SelectObject(mem1dc, hbit1);
		SelectObject(ui_dc, Uibit);
		SelectObject(hp_dc, HPbit);
		SelectObject(die_dc, Diebit);
		SelectObject(start_dc, Startbit);
		SelectObject(help_dc, Helpbit);

		//map.DrawBK(mem1dc, mem2dc, rectview);

		if (0 >= map.getblack_t())
		{
			map.DrawBK(mem1dc, mem2dc, rectview);

		}

		for (int i = 0; i <= ocount; i++)
			obj[i].DrawObj(mem1dc, odc);

		
		if (map.getmapnum() >= 10)
		{
			map.DrawUi(mem1dc, ui_dc, camera);
			map.DrawHP(mem1dc, hp_dc, camera, player);
			if (player.getCMD_die() == 1)
				map.DrawDie(mem1dc, die_dc, camera, sound);
		}
		else
		{
			map.DrawStart(mem1dc, start_dc);
			map.DrawHelp(mem1dc, help_dc, help_button);
			//cout << help_button << endl;
		}
		player.draw(mem1dc, pdc);
		/*if (player.getstate() == 3)
		{
			Ellipse(mem1dc, player.getx() - player.getw(), player.gety() - player.geth() + player.geth() / 2, player.getx() + player.getw(), player.gety() + player.geth());
		}
		else {
			Ellipse(mem1dc, player.getx() - player.getw(), player.gety() - player.geth(), player.getx() + player.getw(), player.gety() + player.geth());
		}*/
		if(map.getblack_t() >0) map.DrawLoadBK(mem1dc, mem2dc, loadbf);
		//cout << "현재HP상태: " << player.gethp() << endl;


		/*oldload = (HBITMAP) SelectObject(loaddc, loadbit);
		if (map.getblack_t() > 0)
		{
			BitBlt(mem1dc, camera.getx(), camera.gety(), 1024, 768, loaddc, 0, 0, BLACKNESS);
		}*/

		/*if (map.getblack_t() > 0)
		{
			map.DrawBK(mem1dc, mem2dc, rectview);
		}*/


		BitBlt(hdc, 0, 0, 1024, 768, mem1dc, camera.getx(), camera.gety(), SRCCOPY);
		

		/*SelectObject(loaddc, oldload);
		DeleteObject(loadbit);
		DeleteObject(loaddc);*/
		DeleteObject(mem1dc);
		EndPaint(hwnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			obj_t += 1;

			player.move(obj_t);
			adjustPlayer(player, obj, map, ocount, g_hinst,sound);

			map.movemap();

			if (map.BlackTime())
			{
				if (loadbf.SourceConstantAlpha+40 > 255)
					loadbf.SourceConstantAlpha = 255;
				else {
					loadbf.SourceConstantAlpha += 40;
				}
			}else{
				//캐릭터가 로딩중일땐 카메라 이동 금지 , 일반모드일때만 카메라 움직임
				if(player.getGamemode()==0)
					adjustCamera(camera, player);
			}

			player.selectBit();
			player.stealthtime();
			player.spike_hurttime();
			
			// 이거를 따로 넣는게 낳을듯 오브젝트 멤버함수로다가
			for (int i = 0; i <= ocount; i++)
			{
				if (obj[i].getType() == 0)
				{
					if (obj_t % 10 == 0)
					{
						obj[i].IndexChange();

					}

				}
				if (obj[i].getType() == 4)
				{
					if (obj_t % 8 == 0)
					{
						obj[i].IndexChange();

					}

				}
				if (obj[i].getType() == 103)
				{
					if (obj_t % 30 == 0)
					{
						obj[i].IndexChange();

					}

				}
				if (obj[i].getType() == 106|| obj[i].getType() == 107)
				{
					if (obj_t % 5 == 0)
					{
						obj[i].IndexChange();

					}
					obj[i].move();
				}
				else if (obj[i].getType() == 201)
				{
					if (obj_t % 20 == 0)
					{
						obj[i].IndexChange();

					}
				}
			}
			if (obj_t >= 27000) obj_t = 0;
			InvalidateRgn(hwnd, NULL, FALSE);
			break;
		
		}
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (player.getCMD_die() == 1)
			break;
		if (player.getGamemode() == 0)
			player.PlayerSetting(wParam, sound);
		else if (player.getGamemode() == 1)
			camera.CameraSetting(wParam);
		InvalidateRgn(hwnd, NULL, FALSE);
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		if (player.getCMD_die() == 1)
			break;
		if (player.getGamemode() == 0)
			player.PlayerWaiting(wParam);
		else if (player.getGamemode() == 1)
			camera.CameraSetting(wParam);
		InvalidateRgn(hwnd, NULL, FALSE);
		break;
	case WM_MOUSEMOVE:
		if (player.getCMD_die() == 1)
		{
			if (584 < LOWORD(lParam) && LOWORD(lParam) < 620)
			{
				if (338 < HIWORD(lParam) && HIWORD(lParam) < 352)
				{
					map.ChangeDieNotice(g_hinst, 1);
					if (occur_button == 0)
					{
						FMOD_Channel_Stop(sound.Channel[1]);
						FMOD_System_PlaySound(sound.System, sound.effectSound[4], NULL, 0, &sound.Channel[1]);
						occur_button = 1;
					}
					break;
				}
			}
			map.ChangeDieNotice(g_hinst, 0);
			occur_button = 0;
		}
		if (map.getmapnum() == 9)
		{
			if (290 < LOWORD(lParam) && LOWORD(lParam) < 430)
			{
				if (490 < HIWORD(lParam) && HIWORD(lParam) < 572)
				{
					map.ChangeStartButton(g_hinst, 1);
					if (occur_button == 0)
					{
						FMOD_Channel_Stop(sound.Channel[1]);
						FMOD_System_PlaySound(sound.System, sound.effectSound[4], NULL, 0, &sound.Channel[1]);
						occur_button = 1;
					}
					break;
				}
			}
			
			if (290 < LOWORD(lParam) && LOWORD(lParam) < 428)
			{
				if (345 < HIWORD(lParam) && HIWORD(lParam) < 427)
				{
					
					help_button = 1;
					map.ChangeHelp(g_hinst, help_button);
					if (occur_button == 0)
					{
						FMOD_Channel_Stop(sound.Channel[1]);
						FMOD_System_PlaySound(sound.System, sound.effectSound[4], NULL, 0, &sound.Channel[1]);
						occur_button = 1;
					}
					break;
				}
			}
			map.ChangeStartButton(g_hinst, 0);
			map.ChangeHelp(g_hinst, 0);
			help_button = 0;
			occur_button = 0;
		}
		break;
	case WM_LBUTTONDOWN:
		SetCursor(LoadCursorFromFile(TEXT("cursor/cursor4.cur")));

		if (player.getCMD_die() == 1)
		{
			if (584 < LOWORD(lParam) && LOWORD(lParam) < 620)
			{
				if (338 < HIWORD(lParam) && HIWORD(lParam) < 352)
				{
					map.ChangeDieNotice(g_hinst, 2);
					FMOD_Channel_Stop(sound.Channel[1]);
					FMOD_System_PlaySound(sound.System, sound.effectSound[3], NULL, 0, &sound.Channel[1]);
					break;
				}
			}
		}
		if (map.getmapnum() == 9)
		{
			if (290 < LOWORD(lParam) && LOWORD(lParam) < 430)
			{
				if (490 < HIWORD(lParam) && HIWORD(lParam) < 572)
				{
					map.ChangeStartButton(g_hinst, 2);
					FMOD_Channel_Stop(sound.Channel[1]);
					FMOD_System_PlaySound(sound.System, sound.effectSound[3], NULL, 0, &sound.Channel[1]);
					break;
				}
			}
			
		}
		cout << LOWORD(lParam) << endl;
		cout << HIWORD(lParam) << endl;
		break;
	case WM_LBUTTONUP:
		if (player.getCMD_die() == 1)
		{
			if (584 < LOWORD(lParam) && LOWORD(lParam) < 620)
			{
				if (338 < HIWORD(lParam) && HIWORD(lParam) < 352)
				{
					map.ChangeDieNotice(g_hinst, 0);
					player.initPos();
					break;
				}
			}
		}
		if (map.getmapnum() == 9)
		{
			if (290 < LOWORD(lParam) && LOWORD(lParam) < 430)
			{
				if (490 < HIWORD(lParam) && HIWORD(lParam) < 572)
				{
					map.ChangeStartButton(g_hinst, 0);
					map.setblack_t(50);			
					map.setmapnum(map.getmapnum() + 1);
					player.initPos();
					for (int j = 0; j < ocount; j++)
						obj[j].ResetObject();
					ocount = initObject(obj, map.getmapnum(), g_hinst);
					map.CreateMap(g_hinst);
					hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
					sound.setindex(sound.getindex() + 1);
					FMOD_Channel_Stop(sound.Channel[1]);
					FMOD_System_PlaySound(sound.System, sound.effectSound[1], NULL, 0, &sound.Channel[1]);
					FMOD_Channel_Stop(sound.Channel[0]);
					FMOD_System_PlaySound(sound.System, sound.bgmSound[1], NULL, 0, &sound.Channel[0]);
					InvalidateRgn(hwnd, NULL, FALSE);
					break;
				}
			}
			
		}
		break;
	case WM_CHAR:
		if (wParam == 'r')
		{
			player.setx(0);
			player.sety(300);
			break;
		}
		if (wParam == 'c')
		{
			player.setCMD_move(0);
			if (player.getGamemode() == 0)
				player.setGamemode(1);
			else
				player.setGamemode(0);
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