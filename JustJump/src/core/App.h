#pragma once

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

private:
	void Update(float delta_time);
	void Render();

private:
	virtual bool InitializeWindow();
	void DestroyWindow();

protected:
	static App* s_instance;

	std::wstring m_app_name;
	std::shared_ptr<Window> m_window;

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

