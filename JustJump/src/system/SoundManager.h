#pragma once

namespace FMOD
{
	class Channel;
}

// bgm0~4.mp3에 대응 (SoundManager::PlayBgm)
enum class EBgm
{
	Title = 0,
	Level1,
	Level2,
	Level3,
	Clear,

	Count
};

// effectSound 5종에 대응 (SoundManager::PlayEffect)
enum class EEffect
{
	Jump = 0,
	Portal,
	Death,
	ButtonClick,
	ButtonHover,

	Count
};

class SoundManager
{
public:
	static void Register(SoundManager* instance);
	static void Unregister();
	static SoundManager& Get();
	static bool IsRegistered();

public:
	void Initialize();
	void Shutdown();

	//배경음을 채널(0번)에서 재생, 이미 재생중이면 정지 후 교체
	void PlayBgm(EBgm bgm);
	//효과음을 채널(1번)에서 재생, 이미 재생중이면 정지 후 교체
	void PlayEffect(EEffect effect);
	//FMOD::System::update() 위임
	void Update();

	int GetIndex() const { return m_index; }
	void SetIndex(int i) { m_index = i; }

private:
	static SoundManager* s_instance;

	FmodSystemPtr m_system;
	FmodSoundPtr m_bgmSound[static_cast<size_t>(EBgm::Count)];
	FmodSoundPtr m_effectSound[static_cast<size_t>(EEffect::Count)];
	FMOD::Channel* m_channel[2] = {};	//0=배경음, 1=효과음. Channel은 System이 관리하는 비소유 핸들

	int m_index = 0;
};
