#pragma once

#include "Global.h"

#include "world/Map.h"

#include "object/core/ObjectManager.h"

#include "ui/TitleMenuUI.h"
#include "ui/DeadScreenUI.h"

class PLAYER;
class CAMERA;
class SoundManager;
class Obstacle;

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
	HCURSOR m_clickCursor = nullptr;

	//맵 편집 모드 (F1로 토글, 개발용)
	bool m_editMode = false;
	bool m_editSelectMode = false;	//false=배치모드(클릭=배치), true=선택모드(클릭=선택/해제). Space로 전환
	EObstacleType m_editPlaceType = EObstacleType::Platform;
	SPtr<Obstacle> m_editSelected;

	//로프는 시작점/끝점을 2번 클릭해서 길이를 정함
	bool m_editRopePending = false;
	int m_editRopeStartX = 0;
	int m_editRopeStartY = 0;
	int m_editMouseWorldX = 0;
	int m_editMouseWorldY = 0;

	//배치모드에서 커서 위치에 그려줄 미리보기 오브젝트 (타입 바뀔 때만 새로 생성)
	SPtr<Obstacle> m_editPreview;
	bool m_editPreviewValid = false;
	EObstacleType m_editPreviewType = EObstacleType::Ground;

	//저장 안 된 맵에서 F1 종료를 시도하면 잠깐 뜨는 경고 (프레임 카운트)
	int m_editUnsavedWarnTimer = 0;

private:
	void UpdateGameplay(float dt);
	void UpdateFadeAndCamera();
	void UpdateEditMode();
	void OnEditMouseDown(LPARAM mouse);
	//편집 모드: 포탈 없이 임의의 맵 번호로 바로 이동 (없는 번호면 빈 맵으로 새로 시작)
	void JumpToEditMap(int newMapNumber);

	void RenderScene(HDC hdc);
	void RenderStartMenu(HDC mem1dc);
	void RenderInGameUI(HDC mem1dc);
	void RenderEditOverlay(HDC mem1dc);
};
