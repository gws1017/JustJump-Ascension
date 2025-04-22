#include "Sound.h"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>

int Sound::getindex()
{
	return index;
}

void Sound::setindex(int i)
{
	index = i;
}

void Sound::Sound_Setup()
{
	index = 0;
	char string[100];

	const char* effectPaths[5] = {
	"sound/Jump.mp3",
	"sound/Portal.mp3",
	"sound/Tombstone.mp3",
	"sound/BtMouseClick.mp3",
	"sound/BtMouseOver.mp3"
	};
	
	System = nullptr;
	FMOD_RESULT result = FMOD::System_Create(&System);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD create error: " << FMOD_ErrorString(result) << std::endl;
		return;
	}

	result = System->init(50, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD init error: " << FMOD_ErrorString(result) << std::endl;
		return;
	}

	for (int i = 0; i < 5; ++i)
	{
		sprintf_s(string, sizeof(string), "sound/bgm%d.mp3", i);

		result = System->createSound(string, FMOD_LOOP_NORMAL, nullptr, &bgmSound[i]);
		if (result != FMOD_OK)
		{
			std::cout << "배경음 생성 실패: " << FMOD_ErrorString(result) << std::endl;
		}
		result = System->createSound(effectPaths[i], FMOD_DEFAULT, nullptr, &effectSound[i]);
		if (result != FMOD_OK)
		{
			std::cout << "효과음 생성 실패 (" << effectPaths[i] << "): " << FMOD_ErrorString(result) << std::endl;
		}
	}
	
}
