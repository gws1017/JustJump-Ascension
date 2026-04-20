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
	float m_anim_accum = 0.0f;
    float m_anim_tick = 1.0f / 60.0f;
	
	int m_obj_t = 0;
	int m_ocount = 0;
	int m_help_button = 0;
	int m_start_button = 0;
	bool m_occur_button = false;
	bool m_gamemode = false;

	//Render 관련 변수
	PAINTSTRUCT m_ps{};
	HBITMAP m_hbit1 = nullptr;
	HBITMAP m_ui_bit = nullptr;
	HBITMAP m_hp_bit = nullptr;
	HBITMAP m_die_bit = nullptr;
	HBITMAP m_start_bit = nullptr;
	HBITMAP m_help_bit = nullptr;

private:
	void TickAnimation(float dt);
	void UpdateGameplay();
	void UpdateFadeAndCamera();

	void RenderScene(HDC hdc);
	void RenderStartMenu(HDC mem1dc, HDC start_dc, HDC help_dc);
	void RenderInGameUI(HDC mem1dc, HDC ui_dc, HDC hp_dc, HDC die_dc);

	bool IsInRect(LPARAM mouse, int left, int top, int right, int bottom) const;
	bool IsReviveButtonArea(LPARAM mouse) const;
	bool IsStartButtonArea(LPARAM mouse) const;
	bool IsHelpButtonArea(LPARAM mouse) const;
	void ResetMenuButtons();
};