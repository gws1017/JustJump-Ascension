#pragma once

namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class Sound {
	int index;
public:
	int getindex();

	FMOD::System* System;
	FMOD::Sound* bgmSound[5];	//3는 배경브금갯수
	FMOD::Sound* effectSound[5];	//2는 이펙트브금갯수
	FMOD::Channel* Channel[2];	//2는 배경+이펙트 해서 채널

	void Sound_Setup();
	void setindex(int);
};
