#include "Global.h"
#include "core/LevelConfig.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace LevelConfig
{
	int GetGameplayMapCount()
	{
		static int cached = -1;
		if (cached >= 0) return cached;

		cached = 3; //manifest.json이 없을 때 기본값 (map_1~map_3)
		std::ifstream in("map/manifest.json");
		if (in.is_open())
		{
			try
			{
				nlohmann::json root;
				in >> root;
				if (root.contains("gameplayMapCount"))
					cached = root["gameplayMapCount"].get<int>();
			}
			catch (const std::exception& e)
			{
				std::cout << "manifest.json 파싱 실패: " << e.what() << std::endl;
			}
		}
		return cached;
	}

	int GetClearMapNumber()
	{
		return static_cast<int>(EMapId::GameplayMin) + GetGameplayMapCount();
	}
}
