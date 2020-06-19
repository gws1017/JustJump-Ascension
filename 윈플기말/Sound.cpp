#include "Sound.h"


void Sound::Sound_Setup()
{
	char string[100];

	FMOD_System_Create(&System);
	FMOD_System_Init(System, 10, FMOD_INIT_NORMAL, NULL);

	for (int i = 0; i < 2; ++i)
	{
		wsprintfA(string, "sound/bgm%d.mp3", i);
		FMOD_System_CreateSound(System, string, FMOD_LOOP_NORMAL, 0, &bgmSound[i]);
	}
	FMOD_System_CreateSound(System, "sound/Jump.mp3", FMOD_DEFAULT, 0, &effectSound[0]);
	FMOD_System_CreateSound(System, "sound/Portal.mp3", FMOD_DEFAULT, 0, &effectSound[1]);
}