#include "Global.h"
#include "core/App.h"
#include "core/entry_point.h"

class JustJumpApp : public App
{
public:

	JustJumpApp() : App(L"Just Jump")
	{
	}

	bool Initialize() override
	{
		bool result = App::Initialize();

		return result;
	}

	void Shutdown() override
	{
		App::Shutdown();
	}


};

UPtr<App> CreateApp()
{
	return CreateUPtr<JustJumpApp>();
}