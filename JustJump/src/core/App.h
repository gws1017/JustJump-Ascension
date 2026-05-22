#pragma once

namespace 
{
	constexpr int APP_WIDTH = GameConst::kViewportWidth;
	constexpr int APP_HEIGHT = GameConst::kViewportHeight;
}

class Timer;
class ObjectManager;
class GameWorld;
class InputManager;
struct Window;

class App
{
public:
	App(std::wstring_view app_name);
	virtual ~App();

	App(const App&) = delete;
	App& operator=(const App&) = delete;

	virtual bool Initialize();
	virtual void Shutdown();
	void Run();

public:

	inline static App* GetApp() { return s_instance; }
	GameWorld* GetGameWorld() { return m_game_world.get(); }
	InputManager* GetInputManager() { return m_input_manager.get(); }

private:
	void Update(float delta_time);
	void Render(HDC hdc, const RECT& client_rect);

private:
	virtual bool InitializeWindow();
	void DestroyWindow();

protected:
	static App* s_instance;

	std::wstring m_app_name;
	SPtr<Window> m_window;

private:
    UPtr<GameWorld> m_game_world;
	UPtr<InputManager> m_input_manager;
	UPtr<Timer> m_timer;

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

UPtr<App> CreateApp();

