#include "Global.h"
#include "core/App.h"
#include "core/window.h"

App* App::s_instance = nullptr;

App::App(std::wstring_view app_name)
 : m_app_name(app_name)
{
	s_instance = this;

	m_window = std::make_shared<Window>(1024,768);
}

App::~App()
{
}

bool App::Initialize()
{
	bool result = InitializeWindow();
	return true;
}

void App::Shutdown()
{
}

void App::Run()
{
	MSG msg;

	bool bDone = false;

	/*PeekMessage�����δ� �� ���� �ϳ��� �޽����� ó���ϰ�, 
	�޽����� �� ������ ��� ��ȯ�ǹǷ�
    �� ���� �����ӡ� ���� �׿� �ִ� ��� �޽����� ��� ���, 
	���� �����ӿ� �ٽ� �޽����� �и��� �ʴ´�.*/

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
			Update(0.00016f);
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

	WNDCLASSEX wc{ sizeof(wc) };
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

	return false;
}

void App::DestroyWindow()
{
}

LRESULT CALLBACK App::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = NULL;
	const auto& app = App::GetApp();

	switch (message)
	{
	case WM_SIZE:
		break;
	case WM_ACTIVATE:
		break;
	case WM_MOVE:
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		result = DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return result;
}
