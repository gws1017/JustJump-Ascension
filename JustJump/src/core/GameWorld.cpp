#include "Global.h"
#include "core/GameWorld.h"
#include "core/InputManager.h"

#include "world/Map.h"
#include "system/Sound.h"

#include "object/character/player.h"
#include "object/view/Camera.h"
#include "object/core/ObjectManager.h"
#include "world/obstacle/obstacle.h"

GameWorld::GameWorld()
    : m_player(CreateUPtr<PLAYER>())
    , m_camera(CreateUPtr<CAMERA>())
{
}

GameWorld::~GameWorld() = default;

bool GameWorld::Initialize(HINSTANCE hinstance)
{
    m_hinstance = hinstance;

    AddFontResourceA("font/Maplestory Bold.ttf");
    AddFontResourceA("font/Maplestory Light.ttf");

    //맵 초기화
    m_map.CreateMap(m_hinstance);
    m_map.CreateUi(m_hinstance);
    m_map.CreateHP(m_hinstance);

    //플레이어 스프라이트는 UI 비트맵보다 먼저 로드 (GDI 핸들 우선 확보)
    m_player->SetBitMap(m_hinstance);
    m_player->InitializeAnimPosition();
    m_player->SelectBitmap();

    //UI 초기화
    m_titleUI.Load(m_hinstance);
    m_deadUI.Load(m_hinstance);

    //카메라 초기화
    if (m_map.GetMapNumber() == static_cast<int>(EMapId::Title))
    {
        m_camera->SetX(0);
        m_camera->SetY(0);
    }

    //블렌드 함수 초기화
    m_blendfunction.AlphaFormat = 0;
    m_blendfunction.BlendFlags = 0;
    m_blendfunction.BlendOp = AC_SRC_OVER;
    m_blendfunction.SourceConstantAlpha = 0;

    //사운드 초기화
    m_sound.Sound_Setup();

    if (m_sound.System && m_sound.bgmSound[0])
    {
        if (m_sound.Channel[0])
            m_sound.Channel[0]->stop();
        m_sound.System->playSound(m_sound.bgmSound[0], nullptr, false, &m_sound.Channel[0]);
    }

    if (m_map.GetMapNumber() == static_cast<int>(EMapId::Title))
        m_hbit1 = (HBITMAP)LoadImage(m_hinstance, TEXT("img/start_rayer1.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    else if (m_map.GetMapNumber() == static_cast<int>(EMapId::Clear))
        m_hbit1 = (HBITMAP)LoadImage(m_hinstance, TEXT("img/clear.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    m_ocount = m_object_manager.InitObject(static_cast<int>(EMapId::Title), m_hinstance);

    return true;
}

void GameWorld::Update(float dt)
{
    dt = std::min(dt, GameConst::kMaxDeltaTime);
    UpdateGameplay(dt);
    UpdateFadeAndCamera();
}

void GameWorld::Render(HDC hdc, const RECT& view)
{
    m_rectview = view;
    RenderScene(hdc);
}

void GameWorld::RenderScene(HDC hdc)
{
    HDC mem1dc = CreateCompatibleDC(hdc);
    HDC mem2dc = CreateCompatibleDC(hdc);

    if (!mem1dc || !mem2dc)
    {
        if (mem1dc) DeleteDC(mem1dc);
        if (mem2dc) DeleteDC(mem2dc);
        return;
    }

    if (m_hbit1 == nullptr)
    {
        m_hbit1 = CreateCompatibleBitmap(hdc, m_rectview.right, m_rectview.bottom);
    }

    SelectObject(mem1dc, m_hbit1);

    if (0 >= m_map.GetBlackTime())
    {
        m_map.DrawBK(mem1dc, mem2dc, m_rectview, m_player);
    }

    m_object_manager.SetMem1DC(&mem1dc);
    m_object_manager.DrawObjects();

    if (m_map.GetMapNumber() == static_cast<int>(EMapId::Title))
    {
        RenderStartMenu(mem1dc);
    }

    m_player->Render(mem1dc);

    if (m_map.GetMapNumber() >= static_cast<int>(EMapId::GameplayMin))
    {
        RenderInGameUI(mem1dc);
    }

    if (m_map.GetBlackTime() > 0) m_map.DrawLoadBK(mem1dc, mem2dc, m_blendfunction);

    BitBlt(hdc, 0, 0, GameConst::kViewportWidth, GameConst::kViewportHeight, mem1dc, m_camera->GetX(), m_camera->GetY(), SRCCOPY);

    DeleteDC(mem1dc);
    DeleteDC(mem2dc);
}

void GameWorld::UpdateGameplay(float dt)
{
    if (InputManager::IsRegistered())
    {
        m_player->ProcessInput(m_sound);

        if (m_player->GetGameMode() && !m_player->IsDead())
            m_camera->ProcessInput();
    }

    m_player->Update(dt);
    m_object_manager.AdjustPlayer(m_player, m_map, m_ocount, m_hinstance, m_sound);
    m_map.movemap();
    m_object_manager.UpdateAll(dt);
}

void GameWorld::UpdateFadeAndCamera()
{
    if (m_map.BlackTime())
    {
        if (m_blendfunction.SourceConstantAlpha + GameConst::kFadeStep > GameConst::kAlphaMax)
            m_blendfunction.SourceConstantAlpha = GameConst::kAlphaMax;
        else
            m_blendfunction.SourceConstantAlpha += GameConst::kFadeStep;
        return;
    }

    if (m_player->GetGameMode() == 0)
        m_object_manager.AdjustCamera(m_camera, m_player);
}

void GameWorld::RenderStartMenu(HDC mem1dc)
{
    m_titleUI.Render(mem1dc);
}

void GameWorld::RenderInGameUI(HDC mem1dc)
{
    HDC dc = nullptr;
    m_map.DrawUi(mem1dc, dc, m_camera);
    m_map.DrawHP(mem1dc, dc, m_camera, m_player);
    if (m_player->IsDead() == 1)
        m_deadUI.Render(mem1dc, m_camera);
}

void GameWorld::Shutdown()
{
    m_titleUI.Unload();
    m_deadUI.Unload();

    // FMOD
	for (int i = 0; i < 5; ++i)
	{
		if (m_sound.effectSound[i])
		{
			m_sound.effectSound[i]->release();
			m_sound.effectSound[i] = nullptr;
		}
	}
	for (int i = 0; i < 2; ++i)
	{
		if (m_sound.Channel[i])
		{
			m_sound.Channel[i]->stop();
			m_sound.Channel[i] = nullptr;
		}
	}
	if (m_sound.System)
	{
		m_sound.System->release();
		m_sound.System = nullptr;
	}
	// GDI 비트맵
	if (m_hbit1) { DeleteObject(m_hbit1); m_hbit1 = nullptr; }

	// 폰트 제거
	RemoveFontResourceA("font/Maplestory Bold.ttf");
	RemoveFontResourceA("font/Maplestory Light.ttf");

    m_object_manager.ResetObstacle();
}

void GameWorld::OnChar(WPARAM ch)
{
	if (ch == 'r')
	{
        int offset = GameConst::kPortalCheatOffset;
		for (const auto& obs : m_object_manager.GetObjects()) {
			if (obs->GetType() == EObstacleType::Portal)
            {
				m_player->SetX(obs->GetX() + offset);
				m_player->SetY(obs->GetY() + offset);
				break;
			}
		}
		return;
	}
	if (ch == 'c')
	{
		m_player->SetMoveCommand(EMoveCommand::None);
		if (m_player->GetGameMode() == 0)
			m_player->SetGameMode(1);
		else
			m_player->SetGameMode(0);
		return;
	}
}

void GameWorld::OnMouseMove(LPARAM mouse)
{
    if (m_player->IsDead() == 1)
    {
        m_deadUI.OnMouseMove(mouse, m_sound);
        return;
    }
    if (m_map.GetMapNumber() == static_cast<int>(EMapId::Title))
        m_titleUI.OnMouseMove(mouse, m_sound);
}

void GameWorld::OnMouseDown(LPARAM mouse)
{
    SetCursor(LoadCursorFromFile(TEXT("cursor/cursor4.cur")));
    if (m_player->IsDead() == 1)
    {
        m_deadUI.OnMouseDown(mouse, m_sound);
        return;
    }
    if (m_map.GetMapNumber() == static_cast<int>(EMapId::Title))
        m_titleUI.OnMouseDown(mouse, m_sound);
}

void GameWorld::OnMouseUp(LPARAM mouse)
{
    if (m_player->IsDead() == 1)
    {
        if (m_deadUI.OnMouseUp(mouse))
        {
            m_player->Initialzie();
            m_player->SetCurrentHP(100);
        }
        return;
    }
    if (m_map.GetMapNumber() == static_cast<int>(EMapId::Title))
    {
        if (m_titleUI.OnMouseUp(mouse))
        {
            m_map.SetBlackTime(50);
            m_map.SetMapNumber(m_map.GetMapNumber() + 1);
            m_player->Initialzie();
            m_object_manager.ResetObstacle();
            m_ocount = m_object_manager.InitObject(m_map.GetMapNumber(), m_hinstance);

            m_map.CreateMap(m_hinstance);
            m_hbit1 = (HBITMAP)LoadImage(m_hinstance, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            m_sound.SetIndex(m_sound.GetIndex() + 1);

            if (m_sound.Channel[1]) m_sound.Channel[1]->stop();
            m_sound.System->playSound(m_sound.effectSound[1], nullptr, false, &m_sound.Channel[1]);

            if (m_sound.Channel[0]) m_sound.Channel[0]->stop();
            m_sound.System->playSound(m_sound.bgmSound[1], nullptr, false, &m_sound.Channel[0]);

            m_player->SetCurrentHP(100);
            m_camera->SetX(0);
            m_camera->SetY(GameConst::kDefaultCameraY);
        }
    }
}
