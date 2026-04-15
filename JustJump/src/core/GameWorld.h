#pragma once

#include "Global.h"
//#include "core/GameWorld.h"

#include "world/Map.h"
#include "system/Sound.h"

#include "object/character/player.h"
#include "object/view/Camera.h"
#include "object/core/ObjectManager.h"

//class PLAYER;
//class MAP;
//class CAMERA;
//class Sound;
//class ObjectManager;

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
	void OnKeyDown(WPARAM key);
	void OnKeyUp(WPARAM key);
	void OnMouseMove(LPARAM mouse);
	void OnMouseDown(LPARAM mouse);
	void OnMouseUp(LPARAM mouse);

private:

	HINSTANCE m_hinstance = nullptr;
	PLAYER m_player;
	MAP m_map;
	CAMERA m_camera;
	Sound m_sound;
	ObjectManager m_object_manager;


	RECT m_rectview{};
	BLENDFUNCTION m_blendfunction{};

	// animation / ui state
	int m_obj_t = 0;
	int m_ocount = 0;
	int m_help_button = 0;
	int m_start_button = 0;
	bool m_occur_button = false;
	bool m_gamemode = false;

	//Render 관련 변수
	PAINTSTRUCT m_ps{};
	HDC m_mem1dc = nullptr;
	HDC m_mem2dc = nullptr;
	HDC m_ui_dc = nullptr;
	HDC m_hp_dc = nullptr;
	HDC m_die_dc = nullptr;
	HDC m_start_dc = nullptr;
	HDC m_help_dc = nullptr;
	HDC m_player_dc = nullptr;
	HBITMAP m_hbit1 = nullptr;
	HBITMAP m_ui_bit = nullptr;
	HBITMAP m_hp_bit = nullptr;
	HBITMAP m_die_bit = nullptr;
	HBITMAP m_start_bit = nullptr;
	HBITMAP m_help_bit = nullptr;

private:
	// 이후 WndProc에서 옮길 내부 헬퍼들
	void UpdateGameLogic(float dt) {};
	void UpdateLoadingFade() {};
	void UpdateCamera() {};
};