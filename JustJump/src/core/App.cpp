#include "Global.h"
#include "core/App.h"
#include "core/window.h"
#include "core/Timer.h"
#include "core/GameWorld.h"

#include "world/Map.h"
#include "object/core/ObjectManager.h"
#include "object/view/Camera.h"
#include "object/character/player.h"
#include "world/obstacle/obstacle.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

App* App::s_instance = nullptr;

App::App(std::wstring_view app_name)
 : m_app_name(app_name)
{
	s_instance = this;
	m_timer = CreateUPtr<Timer>();
	m_window = CreateSPtr<Window>(APP_WIDTH, APP_HEIGHT);
	m_game_world = CreateUPtr<GameWorld>();
}

App::~App()
{
}

bool App::Initialize()
{
	bool result = InitializeWindow();
	if (result == false)
	{
		std::cout << "Fail Window Initialize" << std::endl;
		return false;
	}

	result = m_timer->Initialize();
	if (result == false)
	{
		std::cout << "Fail Timer Initialize" << std::endl;
		return false;
	}

	result = m_game_world->Initialize(m_window->hInstance);
	if (result == false)
	{
		std::cout << "Fail GameWorld Initialize" << std::endl;
		return false;
	}

	return true;
}

void App::Shutdown()
{
}

void App::Run()
{
	MSG msg;

	bool bDone = false;

	/*PeekMessage만으로는 한 번에 하나의 메시지만 처리하고, 
	메시지가 더 없으면 즉시 반환되므로
    그 “한 프레임” 동안 쌓여 있는 모든 메시지를 비워 줘야, 
	다음 프레임에 다시 메시지가 밀리지 않는다.*/

	while (!bDone)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				bDone = true;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_timer->Update();
			Update(m_timer->GetDeltaTime());
			Render();
		}
		if (bDone) break;
	}
}

void App::Update(float delta_time)
{
}

void App::Render()
{
}

bool App::InitializeWindow()
{
	m_window->hInstance = GetModuleHandle(nullptr);
	if (m_window->hInstance == nullptr)
		return false;

	WNDCLASSEXW wc{ sizeof(wc) };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_window->hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursorFromFile(TEXT("cursor/cursor2.cur"));
	wc.hbrBackground = reinterpret_cast<HBRUSH>(WHITE_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_app_name.c_str();
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	if (!RegisterClassExW(&wc)) {
		MessageBox(nullptr, TEXT("Failed to register window class"), m_app_name.c_str(), MB_ICONERROR);
		return false;
	}
	DWORD dw_style = WS_OVERLAPPED | WS_MINIMIZEBOX;

	int posX = (GetSystemMetrics(SM_CXSCREEN) == m_window->width) ? 0 : (GetSystemMetrics(SM_CXSCREEN) - m_window->width) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) == m_window->height) ? 0 : (GetSystemMetrics(SM_CYSCREEN) - m_window->height) / 2;


	m_window->hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_app_name.c_str(), m_app_name.c_str(),
			dw_style, posX, posY, m_window->width, m_window->height, NULL, NULL, m_window->hInstance, NULL);

	if (m_window->hWnd == nullptr) {
		MessageBox(nullptr, TEXT("Failed to create window"), m_app_name.c_str(), MB_ICONERROR);
		return false;
	}
#ifdef _DEBUG
	ShowWindow(GetConsoleWindow(), SW_SHOW);
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
	ShowWindow(m_window->hWnd, SW_SHOW);
	UpdateWindow(m_window->hWnd);

	SetForegroundWindow(m_window->hWnd);
	SetFocus(m_window->hWnd);

	return true;
}

void App::DestroyWindow()
{
	::DestroyWindow(m_window->hWnd);
	UnregisterClassW(m_app_name.c_str(), m_window->hInstance);
}

//LRESULT CALLBACK App::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	LRESULT result = NULL;
//	const auto& app = App::GetApp();
//
//	switch (message)
//	{
//	case WM_SIZE:
//		break;
//	case WM_ACTIVATE:
//		break;
//	case WM_MOVE:
//		break;
//	case WM_DESTROY:
//	case WM_CLOSE:
//		PostQuitMessage(0);
//		break;
//	default:
//		result = DefWindowProc(hWnd, message, wParam, lParam);
//		break;
//	}
//	return result;
//}

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	static HDC hdc, mem1dc, mem2dc, loaddc, playerdc, odc, pdc, ui_dc, hp_dc, die_dc, start_dc, help_dc; // odc = 오브젝트 dc, pdc = player dc,ui_Dc : 아래 전체적인 ui hp_Dc: hp통만 나오는거 dic_dc : 사망 ui 
	static RECT rectview;
	static HBITMAP hbit1, loadbit, oldload, oldbit1, hbitobj[100], Uibit, HPbit, Diebit, Startbit, Helpbit;
	static PLAYER player;
	static MAP map;
	static CAMERA camera;
	static BLENDFUNCTION loadbf;
	static Sound sound;


	static int obj_t = 0; //오브젝트 애니메이션을 1번타이머에 넣기위해 추가한 변수
	static int ocount;		//obj 개수를 세주는 변수
	static int help_button = 0, start_button = 0; //조작법 온오프
	static bool occur_button = 0;	//사망했을때의 button이 활성화되었는지 
	static bool gamemode = 0;	//0이면 기본 1이면 자유모드
	const auto& app = App::GetApp();
	const auto& gameWorld = app->m_game_world;

	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 1, nullptr);
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		RECT view{};

		gameWorld->Render(hdc, view);

		EndPaint(hwnd, &ps);
		break;
	}

	case WM_TIMER:

		gameWorld->Update(0.f);
		InvalidateRgn(hwnd, nullptr, FALSE);
		break;

	case WM_KEYDOWN:

		gameWorld->OnKeyDown(wParam);
		InvalidateRgn(hwnd, nullptr, FALSE);
		break;

	case WM_KEYUP:

		gameWorld->OnKeyUp(wParam);
		InvalidateRgn(hwnd, nullptr, FALSE);
		break;

	case WM_MOUSEMOVE:
		gameWorld->OnMouseMove(lParam);

		if (player.IsDead() == 1)
		{
			if (584 < LOWORD(lParam) && LOWORD(lParam) < 620)
			{
				if (338 < HIWORD(lParam) && HIWORD(lParam) < 352)
				{
					map.ChangeDieNotice(app->m_window->hInstance, 1);
					if (occur_button == 0)
					{
						if (sound.Channel[1]) {
							sound.Channel[1]->stop();
						}

						FMOD_RESULT result = sound.System->playSound(
							sound.effectSound[4],
							nullptr,
							false,
							&sound.Channel[1]
						);
						occur_button = 1;
					}
					break;
				}
			}
			map.ChangeDieNotice(app->m_window->hInstance, 0);
			occur_button = 0;
		}
		if (map.GetMapNumber() == 9)
		{
			if (290 < LOWORD(lParam) && LOWORD(lParam) < 430)
			{
				if (490 < HIWORD(lParam) && HIWORD(lParam) < 572)
				{
					//map.ChangeStartButton(app->m_window->hInstance, 1);
					if (start_button == 0)
					{
						if (sound.Channel[1]) {
							sound.Channel[1]->stop();
						}

						FMOD_RESULT result = sound.System->playSound(
							sound.effectSound[4],
							nullptr,
							false,
							&sound.Channel[1]
						);
						start_button = 1;
					}
					break;
				}
			}

			if (290 < LOWORD(lParam) && LOWORD(lParam) < 428)
			{
				if (345 < HIWORD(lParam) && HIWORD(lParam) < 427)
				{

					//map.ChangeHelp(app->m_window->hInstance, help_button);
					if (help_button == 0)
					{

						if (sound.Channel[1]) {
							sound.Channel[1]->stop();
						}

						FMOD_RESULT result = sound.System->playSound(
							sound.effectSound[4],
							nullptr,
							false,
							&sound.Channel[1]
						);
						help_button = 1;
					}
					break;
				}
			}
			//map.ChangeStartButton(app->m_window->hInstance, 0);
			//map.ChangeHelp(app->m_window->hInstance, 0);
			start_button = 0;
			help_button = 0;
			occur_button = 0;
		}
		break;
	case WM_LBUTTONDOWN:
		gameWorld->OnMouseDown(lParam);

		SetCursor(LoadCursorFromFile(TEXT("cursor/cursor4.cur")));

		if (player.IsDead() == 1)
		{
			if (584 < LOWORD(lParam) && LOWORD(lParam) < 620)
			{
				if (338 < HIWORD(lParam) && HIWORD(lParam) < 352)
				{
					map.ChangeDieNotice(app->m_window->hInstance, 2);
					if (sound.Channel[1]) {
						sound.Channel[1]->stop();
					}

					FMOD_RESULT result = sound.System->playSound(
						sound.effectSound[3],
						nullptr,
						false,
						&sound.Channel[1]
					);
					break;
				}
			}
		}
		if (map.GetMapNumber() == 9)
		{
			if (290 < LOWORD(lParam) && LOWORD(lParam) < 430)
			{
				if (490 < HIWORD(lParam) && HIWORD(lParam) < 572)
				{
					if (start_button == 1)
					{

						//map.ChangeStartButton(app->m_window->hInstance, 2);
						if (sound.Channel[1]) {
							sound.Channel[1]->stop();
						}

						FMOD_RESULT result = sound.System->playSound(
							sound.effectSound[3],
							nullptr,
							false,
							&sound.Channel[1]
						);
						start_button = 2;

						break;
					}

				}
			}

		}
		cout << LOWORD(lParam) << endl;
		cout << HIWORD(lParam) + camera.GetY() << endl;
		break;
	case WM_LBUTTONUP:

		gameWorld->OnMouseUp(lParam);
		InvalidateRgn(hwnd, nullptr, FALSE);
		break;
	case WM_CHAR:
		if (wParam == 'r')
		{
			//player.SetX(obj[ocount - 1].GetX() + 10);
			//player.SetY(obj[ocount - 1].GetY() - 25);
			break;
		}
		if (wParam == 'c')
		{
			player.SetMoveCommand(EMoveCommand::None);
			if (player.GetGameMode() == 0)
				player.SetGameMode(1);
			else
				player.SetGameMode(0);
			break;
		}
		InvalidateRect(hwnd, nullptr, FALSE);
		break;
	case WM_DESTROY:
		for (int i = 0; i < 5; ++i)
		{
			if (sound.effectSound[i]) {
				sound.effectSound[i]->release();
				sound.effectSound[i] = nullptr;
			}
		}
		for (int i = 0; i < 2; ++i)
		{
			if (sound.Channel[i]) {
				sound.Channel[i]->stop();
				sound.Channel[i] = nullptr;
			}
		}
		sound.System->release();
		KillTimer(hwnd, 1);
		hbit1, loadbit, hbitobj[100], Uibit, HPbit, Diebit, Startbit, Helpbit;
		if (hbit1) DeleteObject(hbit1);
		if (loadbit) DeleteObject(loadbit);
		if (Uibit) DeleteObject(Uibit);
		if (Diebit) DeleteObject(Diebit);
		if (Startbit) DeleteObject(Startbit);
		if (Helpbit) DeleteObject(Helpbit);
		for (int i = 0; i < 100; ++i)
			if (hbitobj[i]) DeleteObject(hbitobj[i]);


		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hwnd, iMessage, wParam, lParam));
}
