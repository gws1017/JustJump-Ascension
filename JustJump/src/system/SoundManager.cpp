#include "Global.h"
#include "system/SoundManager.h"
#include <cassert>

SoundManager* SoundManager::s_instance = nullptr;

void SoundManager::Register(SoundManager* instance)
{
	s_instance = instance;
}

void SoundManager::Unregister()
{
	s_instance = nullptr;
}

SoundManager& SoundManager::Get()
{
	assert(s_instance != nullptr && "SoundManager::Get — Register() 먼저 호출");
	return *s_instance;
}

bool SoundManager::IsRegistered()
{
	return s_instance != nullptr;
}

void SoundManager::Initialize()
{
	m_index = 0;

	char string[100];

	const char* effectPaths[static_cast<size_t>(EEffect::Count)] = {
	"sound/Jump.mp3",
	"sound/Portal.mp3",
	"sound/Tombstone.mp3",
	"sound/BtMouseClick.mp3",
	"sound/BtMouseOver.mp3"
	};

	FMOD::System* system = nullptr;
	FMOD_RESULT result = FMOD::System_Create(&system);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD create error: " << FMOD_ErrorString(result) << std::endl;
		return;
	}
	m_system.reset(system);

	result = m_system->init(50, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD init error: " << FMOD_ErrorString(result) << std::endl;
		return;
	}

	for (int i = 0; i < static_cast<int>(EBgm::Count); ++i)
	{
		sprintf_s(string, sizeof(string), "sound/bgm%d.mp3", i);

		FMOD::Sound* bgm = nullptr;
		result = m_system->createSound(string, FMOD_LOOP_NORMAL, nullptr, &bgm);
		if (result != FMOD_OK)
			std::cout << "배경음 생성 실패: " << FMOD_ErrorString(result) << std::endl;
		m_bgmSound[i].reset(bgm);
	}

	for (int i = 0; i < static_cast<int>(EEffect::Count); ++i)
	{
		FMOD::Sound* effect = nullptr;
		result = m_system->createSound(effectPaths[i], FMOD_DEFAULT, nullptr, &effect);
		if (result != FMOD_OK)
			std::cout << "효과음 생성 실패 (" << effectPaths[i] << "): " << FMOD_ErrorString(result) << std::endl;
		m_effectSound[i].reset(effect);
	}
}

void SoundManager::Shutdown()
{
	for (auto& ch : m_channel)
	{
		if (ch) { ch->stop(); ch = nullptr; }
	}
	for (auto& s : m_bgmSound) s.reset();
	for (auto& s : m_effectSound) s.reset();
	m_system.reset();
}

void SoundManager::PlayBgm(EBgm bgm)
{
	const int index = static_cast<int>(bgm);
	if (!m_system || !m_bgmSound[index]) return;
	if (m_channel[0]) m_channel[0]->stop();
	m_system->playSound(m_bgmSound[index].get(), nullptr, false, &m_channel[0]);
}

void SoundManager::PlayEffect(EEffect effect)
{
	const int index = static_cast<int>(effect);
	if (!m_system || !m_effectSound[index]) return;
	if (m_channel[1]) m_channel[1]->stop();
	m_system->playSound(m_effectSound[index].get(), nullptr, false, &m_channel[1]);
}

void SoundManager::Update()
{
	if (m_system) m_system->update();
}
