#include "Global.h"
#include "core/App.h"
#include "core/window.h"
#include "core/Timer.h"
#include "core/GameWorld.h"
#include "core/InputManager.h"

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
	m_input_manager = CreateUPtr<InputManager>();
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
	InputManager::Register(m_input_manager.get());
	return true;
}

void App::Shutdown()
{
	if(m_game_world)
		m_game_world->Shutdown();

	InputManager::Unregister();
	if (m_game_world)
		m_game_world->Shutdown();
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
			InvalidateRect(m_window->hWnd, nullptr, FALSE);
		}
		if (bDone) break;
	}
}

void App::Update(float delta_time)
{
	if (!m_game_world || !m_input_manager)
		return;

	m_input_manager->BeginFrame();
	m_game_world->Update(delta_time);
	m_input_manager->EndFrame();
}

void App::Render(HDC hdc, const RECT& client_rect)
{
	if(m_game_world)
        m_game_world->Render(hdc, client_rect);
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

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	const auto& app = App::GetApp();
	const auto& gameWorld = app ? app->GetGameWorld() : nullptr;
	const auto& input = app ? app->GetInputManager() : nullptr;

	//Null Check
	if(!app || !gameWorld || !input)
		return DefWindowProc(hwnd, iMessage, wParam, lParam);

	switch (iMessage)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps{};
		HDC hdc = BeginPaint(hwnd, &ps);
		RECT view{};

		GetClientRect(hwnd, &view);

		App::GetApp()->Render(hdc,view);

		EndPaint(hwnd, &ps);
		break;
	}

	case WM_KEYDOWN:
		input->OnKeyDown(wParam);
		break;

	case WM_KEYUP:
		input->OnKeyUp(wParam);
		break;

	case WM_MOUSEMOVE:
		input->OnMouseMove(lParam);

		gameWorld->OnMouseMove(lParam);
		break;

	case WM_LBUTTONDOWN:
		input->OnMouseDown(wParam,lParam);
		gameWorld->OnMouseDown(lParam);
		break;

	case WM_LBUTTONUP:
		input->OnMouseUp(wParam,lParam);
		gameWorld->OnMouseUp(lParam);
		break;

	case WM_CHAR:
		gameWorld->OnChar(wParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hwnd, iMessage, wParam, lParam));
}
