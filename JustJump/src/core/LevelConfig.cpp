#include "Global.h"
#include "core/LevelConfig.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace
{
	int g_gameplayMapCountCache = -1;

	void EnsureLoaded()
	{
		if (g_gameplayMapCountCache >= 0)
			return;

		g_gameplayMapCountCache = 3; //manifest.json이 없을 때 기본값 (map_1~map_3)
		std::ifstream in("map/manifest.json");
		if (in.is_open())
		{
			try
			{
				nlohmann::json root;
				in >> root;
				if (root.contains("gameplayMapCount"))
					g_gameplayMapCountCache = root["gameplayMapCount"].get<int>();
			}
			catch (const std::exception& e)
			{
				std::cout << "manifest.json 파싱 실패: " << e.what() << std::endl;
			}
		}
	}
}

namespace LevelConfig
{
	int GetGameplayMapCount()
	{
		EnsureLoaded();
		return g_gameplayMapCountCache;
	}

	int GetClearMapNumber()
	{
		return static_cast<int>(EMapId::GameplayMin) + GetGameplayMapCount();
	}

	void EnsureGameplayMapCount(int atLeast)
	{
		EnsureLoaded();
		if (g_gameplayMapCountCache >= atLeast)
			return;

		g_gameplayMapCountCache = atLeast;

		nlohmann::json root;
		root["gameplayMapCount"] = atLeast;
		std::ofstream out("map/manifest.json");
		if (out.is_open())
			out << root.dump(2);
		else
			std::cout << "manifest.json 저장 실패" << std::endl;
	}
}
