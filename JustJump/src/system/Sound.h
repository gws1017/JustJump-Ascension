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
	FMOD::Sound* bgmSound[5];	//3�� ����ݰ���
	FMOD::Sound* effectSound[5];	//2�� ����Ʈ��ݰ���
	FMOD::Channel* Channel[2];	//2�� ���+����Ʈ �ؼ� ä��

	void Sound_Setup();
	void setindex(int);
};
