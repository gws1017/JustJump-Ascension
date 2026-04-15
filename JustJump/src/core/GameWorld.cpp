#include "Global.h"
#include "core/GameWorld.h"

#include "world/Map.h"
#include "system/Sound.h"

#include "object/character/player.h"
#include "object/view/Camera.h"
#include "object/core/ObjectManager.h"


GameWorld::GameWorld() = default;

GameWorld::~GameWorld() = default;

bool GameWorld::Initialize(HINSTANCE hinstance)
{
    m_hinstance = hinstance;

    AddFontResourceA("font/Maplestory Bold.ttf");
    AddFontResourceA("font/Maplestory Light.ttf");

    //맵 초기화화
    m_map.CreateMap(m_hinstance);
    m_map.CreateUi(m_hinstance);
    m_map.CreateHP(m_hinstance);
    m_map.CreateDie(m_hinstance);
    m_map.CreateStart(m_hinstance);
    m_map.CreateHelp(m_hinstance);

    //플레이어 초기화화
    m_player.SetBitMap(m_hinstance);
    m_player.InitializeAnimPosition();

    //카메라 초기화
    if (m_map.GetMapNumber() == 9)
    {
        m_camera.SetX(0);
        m_camera.SetY(0);
    }

    //블렌드 함수 초기화
    m_blendfunction.AlphaFormat = 0;
    m_blendfunction.BlendFlags = 0;
    m_blendfunction.BlendOp = AC_SRC_OVER;
    m_blendfunction.SourceConstantAlpha = 0;

    //사운드 초기화
    m_sound.Sound_Setup();
    
    if (m_sound.Channel[0])
    {
        m_sound.Channel[0]->stop();
    }
    m_sound.System->playSound(m_sound.bgmSound[0], nullptr, false, &m_sound.Channel[0]);
    m_ocount = m_object_manager.InitObject(9, m_hinstance);

    if (m_map.GetMapNumber() == 9)
    m_hbit1 = (HBITMAP)LoadImage(m_hinstance, TEXT("img/start_rayer1.bmp"), 
IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    else if (m_map.GetMapNumber() == 13)
        m_hbit1 = (HBITMAP)LoadImage(m_hinstance, TEXT("img/clear.bmp"), 
    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    m_help_bit = (HBITMAP)LoadImage(m_hinstance, TEXT("img/help1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    m_start_bit = (HBITMAP)LoadImage(m_hinstance, TEXT("img/start1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    m_ui_bit = (HBITMAP)LoadImage(m_hinstance, TEXT("img/Ui.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    m_hp_bit = (HBITMAP)LoadImage(m_hinstance, TEXT("img/Ui_HP.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    m_die_bit = (HBITMAP)LoadImage(m_hinstance, TEXT("img/Notice3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    
    m_ocount = m_object_manager.InitObject(9, m_hinstance);

    return true;
}

void GameWorld::Update(float dt)
{
    ++m_obj_t;

    m_player.UpdateMovement(m_obj_t);
    m_object_manager.AdjustPlayer(m_player, m_map, m_ocount, m_hinstance, m_sound);
    m_map.movemap();

    //페이드 효과 업데이트트
    if (m_map.BlackTime())
    {
        if (m_blendfunction.SourceConstantAlpha + 40 > 255)
            m_blendfunction.SourceConstantAlpha = 255;
        else
            m_blendfunction.SourceConstantAlpha += 40;
    }
    else
    {
        if (m_player.GetGameMode() == 0)
            m_object_manager.AdjustCamera(m_camera, m_player);
    }

    m_player.SelectBitmap();
    m_player.UpdateInvincibilityTimer();
    m_player.UpdateSpikeKnockback();

    m_object_manager.IndexChange(m_obj_t);
    if (m_obj_t >= 27000) m_obj_t = 0;
}

void GameWorld::Render(HDC hdc, const RECT& view)
{
    m_rectview = view;
    
    m_mem1dc = CreateCompatibleDC(hdc);
    if (m_hbit1 == nullptr)
    {
        m_hbit1 = CreateCompatibleBitmap(hdc, m_rectview.right, m_rectview.bottom);
    }

    SelectObject(m_mem1dc, m_hbit1);
    SelectObject(m_ui_dc, m_ui_bit);
    SelectObject(m_hp_dc, m_hp_bit);
    SelectObject(m_die_dc, m_die_bit);
    SelectObject(m_start_dc, m_start_bit);
    SelectObject(m_help_dc, m_help_bit);

    if (0 >= m_map.GetBlackTime())
    {
        m_map.DrawBK(m_mem1dc, m_mem2dc, m_rectview);
    }

    m_object_manager.SetMem1DC(&m_mem1dc);
    m_object_manager.DrawObjects();

    if (m_map.GetMapNumber() == 9)
    {
        m_map.DrawStart(m_mem1dc, m_start_dc, m_start_button);
        m_map.DrawHelp(m_mem1dc, m_help_dc, m_help_button);
    }

    m_player.Render(m_mem1dc, m_player_dc);

    if (m_map.GetMapNumber() >= 10)
    {
        m_map.DrawUi(m_mem1dc, m_ui_dc, m_camera);
        m_map.DrawHP(m_mem1dc, m_hp_dc, m_camera, m_player);
        if (m_player.IsDead() == 1)
            m_map.DrawDie(m_mem1dc, m_die_dc, m_camera, m_sound);
    }

    if (m_map.GetBlackTime() > 0) m_map.DrawLoadBK(m_mem1dc, m_mem2dc, m_blendfunction);

    BitBlt(hdc, 0, 0, 1024, 768, m_mem1dc, m_camera.GetX(), m_camera.GetY(), SRCCOPY);
    DeleteDC(m_mem1dc);
}
void GameWorld::Shutdown()
{
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
	// GDI 비트맵 (GameWorld가 LoadImage로 들고 있는 것들)
	auto del_bitmap = [](HBITMAP& b) {
		if (b) { DeleteObject(b); b = nullptr; }
	};
	del_bitmap(m_hbit1);
	del_bitmap(m_help_bit);
	del_bitmap(m_start_bit);
	del_bitmap(m_ui_bit);
	del_bitmap(m_hp_bit);
	del_bitmap(m_die_bit);
	// AddFontResourceA 제거
	RemoveFontResourceA("font/Maplestory Bold.ttf");
	RemoveFontResourceA("font/Maplestory Light.ttf");
}

void GameWorld::OnChar(WPARAM ch)
{
	if (ch == 'r')
	{
		// 포탈 위치(다음맵 이동) 조정하는 치트키 코드 추가
		return;
	}
	if (ch == 'c')
	{
		m_player.SetMoveCommand(EMoveCommand::None);
		if (m_player.GetGameMode() == 0)
			m_player.SetGameMode(1);
		else
			m_player.SetGameMode(0);
		return;
	}
}

void GameWorld::OnKeyDown(WPARAM key)
{
    if (m_player.IsDead() == 1) return; //사망시 체크 하지않음음
    if (m_player.GetGameMode() == 0) m_player.OnKeyPressed(key, m_sound);
    else if (m_player.GetGameMode() == 1) m_camera.CameraSetting(key);
}
void GameWorld::OnKeyUp(WPARAM key)
{
    if (m_player.IsDead() == 1) return;
    if (m_player.GetGameMode() == 0) m_player.OnKeyReleased(key);
    else if (m_player.GetGameMode() == 1) m_camera.CameraSetting(key);
}
void GameWorld::OnMouseMove(LPARAM mouse) 
{
    if (m_player.IsDead() == 1)
	{
		if (584 < LOWORD(mouse) && LOWORD(mouse) < 620)
		{
			if (338 < HIWORD(mouse) && HIWORD(mouse) < 352)
			{
				m_map.ChangeDieNotice(m_hinstance, 1);
				if (m_occur_button == 0)
				{
					if (m_sound.Channel[1])
						m_sound.Channel[1]->stop();
					m_sound.System->playSound(
						m_sound.effectSound[4],
						nullptr,
						false,
						&m_sound.Channel[1]);
					m_occur_button = true;
				}
				return;
			}
		}
		m_map.ChangeDieNotice(m_hinstance, 0);
		m_occur_button = false;
	}
	if (m_map.GetMapNumber() == 9)
	{
		if (290 < LOWORD(mouse) && LOWORD(mouse) < 430)
		{
			if (490 < HIWORD(mouse) && HIWORD(mouse) < 572)
			{
				if (m_start_button == 0)
				{
					if (m_sound.Channel[1])
						m_sound.Channel[1]->stop();
					m_sound.System->playSound(
						m_sound.effectSound[4],
						nullptr,
						false,
						&m_sound.Channel[1]);
					m_start_button = 1;
				}
				return;
			}
		}
		if (290 < LOWORD(mouse) && LOWORD(mouse) < 428)
		{
			if (345 < HIWORD(mouse) && HIWORD(mouse) < 427)
			{
				if (m_help_button == 0)
				{
					if (m_sound.Channel[1])
						m_sound.Channel[1]->stop();
					m_sound.System->playSound(
						m_sound.effectSound[4],
						nullptr,
						false,
						&m_sound.Channel[1]);
					m_help_button = 1;
				}
				return;
			}
		}
		m_start_button = 0;
		m_help_button = 0;
		m_occur_button = false;
	}
}
void GameWorld::OnMouseDown(LPARAM mouse) 
{
    SetCursor(LoadCursorFromFile(TEXT("cursor/cursor4.cur")));
	if (m_player.IsDead() == 1)
	{
		if (584 < LOWORD(mouse) && LOWORD(mouse) < 620)
		{
			if (338 < HIWORD(mouse) && HIWORD(mouse) < 352)
			{
				m_map.ChangeDieNotice(m_hinstance, 2);
				if (m_sound.Channel[1])
					m_sound.Channel[1]->stop();
				m_sound.System->playSound(
					m_sound.effectSound[3],
					nullptr,
					false,
					&m_sound.Channel[1]);
				return;
			}
		}
	}
	if (m_map.GetMapNumber() == 9)
	{
		if (290 < LOWORD(mouse) && LOWORD(mouse) < 430)
		{
			if (490 < HIWORD(mouse) && HIWORD(mouse) < 572)
			{
				if (m_start_button == 1)
				{
					if (m_sound.Channel[1])
						m_sound.Channel[1]->stop();
					m_sound.System->playSound(
						m_sound.effectSound[3],
						nullptr,
						false,
						&m_sound.Channel[1]);
					m_start_button = 2;
				}
				return;
			}
		}
    }
}
void GameWorld::OnMouseUp(LPARAM mouse) 
{
    if (m_player.IsDead() == 1)
    {
        if (584 < LOWORD(mouse) && LOWORD(mouse) < 620)
        {
            if (338 < HIWORD(mouse) && HIWORD(mouse) < 352)
            {
                m_map.ChangeDieNotice(m_hinstance, 0);
                m_player.Initialzie();
                m_player.SetCurrentHP(100);
            }
        }
    }
    if (m_map.GetMapNumber() == 9)
    {
        if (290 < LOWORD(mouse) && LOWORD(mouse) < 430)
        {
            if (490 < HIWORD(mouse) && HIWORD(mouse) < 572)
            {
                m_occur_button = 0;
                m_map.SetBlackTime(50);
                m_map.SetMapNumber(m_map.GetMapNumber() + 1);
                m_player.Initialzie();
                m_object_manager.ResetObstacle();
                m_ocount = m_object_manager.InitObject( m_map.GetMapNumber(), m_hinstance);

                m_map.CreateMap(m_hinstance);
                m_hbit1 = (HBITMAP)LoadImage(m_hinstance, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
                m_sound.SetIndex(m_sound.GetIndex() + 1);

                if (m_sound.Channel[1]) {
                    m_sound.Channel[1]->stop();
                }

                FMOD_RESULT result = m_sound.System->playSound(
                    m_sound.effectSound[1],
                    nullptr,
                    false,
                    &m_sound.Channel[1]
                );

                if (m_sound.Channel[0]) {
                    m_sound.Channel[0]->stop();
                }

                result = m_sound.System->playSound(
                    m_sound.bgmSound[1],
                    nullptr,
                    false,
                    &m_sound.Channel[0]
                );
                m_player.SetCurrentHP(100);
                m_camera.SetX(0);
                m_camera.SetY(3232);
            }
        }
    }
}