#pragma once

#include "Global.h"

#include "world/Map.h"

#include "object/core/ObjectManager.h"

#include "ui/TitleMenuUI.h"
#include "ui/DeadScreenUI.h"

class PLAYER;
class CAMERA;
class SoundManager;

class GameWorld
{
public:
	GameWorld();
	virtual ~GameWorld();

	GameWorld(const GameWorld&) = delete;
	GameWorld& operator=(const GameWorld&) = delete;

public:
	bool Initialize(HINSTANCE hinstance);
	void Update(float dt);
	void Render(HDC hdc, const RECT& view);
	void Shutdown();

	void OnChar(WPARAM key);
	void OnMouseMove(LPARAM mouse);
	void OnMouseDown(LPARAM mouse);
	void OnMouseUp(LPARAM mouse);

private:

	HINSTANCE m_hinstance = nullptr;
	UPtr<PLAYER> m_player;
	MAP m_map;
	UPtr<CAMERA> m_camera;
	UPtr<SoundManager> m_sound;
	ObjectManager m_object_manager;

	TitleMenuUI m_titleUI;
	DeadScreenUI m_deadUI;

	RECT m_rectview{};
	BLENDFUNCTION m_blendfunction{};

	int m_ocount = 0;

	//Render 관련 변수
	PAINTSTRUCT m_ps{};
	HBITMAP m_hbit1 = nullptr;

private:
	void UpdateGameplay(float dt);
	void UpdateFadeAndCamera();

	void RenderScene(HDC hdc);
	void RenderStartMenu(HDC mem1dc);
	void RenderInGameUI(HDC mem1dc);
};
