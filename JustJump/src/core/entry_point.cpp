#pragma once
#include "Global.h"
#include "core/App.h"
#include "entry_point.h"

std::unique_ptr<App> CreateApp();

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	const auto& app = CreateApp();
	if (app->Initialize())
	{
		app->Run();
		app->Shutdown();
	}
	else
	{
		std::cout << "Fail Initalize App" << std::endl;
	}
}

