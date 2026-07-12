#include "Global.h"
#include "core/GameWorld.h"
#include "core/InputManager.h"
#include "core/LevelConfig.h"
#include <algorithm>
#include <cstdlib>

#include "world/Map.h"
#include "system/SoundManager.h"

#include "object/character/player.h"
#include "object/view/Camera.h"
#include "object/core/ObjectManager.h"
#include "world/obstacle/obstacle.h"

namespace
{
	//편집 모드에서 Tab으로 순환할 타입 목록 (EObstacleType 값이 연속적이지 않아서 직접 나열)
	//AnimatedBg는 맵 전체 배경용이라 개별 배치 대상에서 제외
	constexpr EObstacleType kEditableTypes[] = {
		EObstacleType::Ground, EObstacleType::Platform, EObstacleType::SmallPlat,
		EObstacleType::BeltRight, EObstacleType::BeltLeft, EObstacleType::Transparent, EObstacleType::LongPlat,
		EObstacleType::Nail, EObstacleType::BrokenPipe, EObstacleType::Gas,
		EObstacleType::GearRow, EObstacleType::GearCol, EObstacleType::Portal, EObstacleType::Rope,
	};
	constexpr int kEditableTypeCount = sizeof(kEditableTypes) / sizeof(kEditableTypes[0]);

	EObstacleType NextEditableType(EObstacleType current)
	{
		for (int i = 0; i < kEditableTypeCount; ++i)
		{
			if (kEditableTypes[i] == current)
				return kEditableTypes[(i + 1) % kEditableTypeCount];
		}
		return kEditableTypes[0];
	}

	//편집 모드 배치용 기본 크기 (드래그 리사이즈는 아직 없어서 대략적인 값)
	void GetDefaultObstacleSize(EObstacleType type, int& outW, int& outH)
	{
		switch (type)
		{
		//가능하면 실제 스프라이트 원본 크기(ObstacleSprite:: 상수)와 1:1로 맞춰서
		//TransparentBlt가 늘리거나 압축하지 않도록 함
		case EObstacleType::Ground:      outW = 1023; outH = 62;  break;
		case EObstacleType::AnimatedBg:  outW = 1024; outH = 768; break;
		case EObstacleType::Platform:    outW = 77;   outH = 1;   break;
		case EObstacleType::SmallPlat:   outW = ObstacleSprite::kSmall.w; outH = 1; break;
		case EObstacleType::BeltRight:
		case EObstacleType::BeltLeft:    outW = ObstacleSprite::kBelt.w;  outH = 1; break;
		case EObstacleType::Transparent: outW = 100;  outH = 20;  break;
		case EObstacleType::LongPlat:    outW = 300;  outH = 1;   break;
		case EObstacleType::Nail:        outW = ObstacleSprite::kNail.w - ObstacleSprite::kNail.dst_w_offset; outH = 15; break;
		case EObstacleType::BrokenPipe:  outW = 17;   outH = 75;  break;
		case EObstacleType::Gas:         outW = 40;   outH = 20;  break;
		case EObstacleType::GearRow:
		case EObstacleType::GearCol:     outW = ObstacleSprite::kGear.w; outH = ObstacleSprite::kGear.h; break;
		case EObstacleType::Portal:      outW = 63;   outH = 135; break;
		case EObstacleType::Rope:        outW = 24;   outH = 200; break;
		default:                        outW = 50;   outH = 50;  break;
		}
	}

	//편집 모드 클릭판정/선택 표시용 박스. 발판류처럼 충돌판정 height가 1px인 오브젝트도
	//중심 기준으로 최소 크기만큼 넓혀서, 판정도 되고 테두리도 선처럼 안 보이게 한다
	RECT GetEditHighlightBox(const SPtr<Obstacle>& obs)
	{
		constexpr int kMinSize = 20;
		const int w = std::max(obs->GetWidth(), kMinSize);
		const int h = std::max(obs->GetHeight(), kMinSize);
		const int cx = obs->GetX() + obs->GetWidth() / 2;
		const int cy = obs->GetY() + obs->GetHeight() / 2;
		return RECT{ cx - w / 2, cy - h / 2, cx + w / 2, cy + h / 2 };
	}
}

GameWorld::GameWorld()
    : m_player(CreateUPtr<PLAYER>())
    , m_camera(CreateUPtr<CAMERA>())
    , m_sound(CreateUPtr<SoundManager>())
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
    SoundManager::Register(m_sound.get());
    m_sound->Initialize();
    m_sound->PlayBgm(EBgm::Title);

    if (m_map.GetMapNumber() == static_cast<int>(EMapId::Title))
        m_hbit1 = (HBITMAP)LoadImage(m_hinstance, TEXT("img/start_rayer1.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    else if (m_map.GetMapNumber() == LevelConfig::GetClearMapNumber())
        m_hbit1 = (HBITMAP)LoadImage(m_hinstance, TEXT("img/clear.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    m_ocount = m_object_manager.InitObject(static_cast<int>(EMapId::Title), m_hinstance);

    m_clickCursor = LoadCursorFromFile(TEXT("cursor/cursor4.cur"));

    return true;
}

void GameWorld::Update(float dt)
{
    dt = std::min(dt, GameConst::kMaxDeltaTime);

    if (InputManager::IsRegistered() && InputManager::Get().IsKeyPressed(VK_F1)
        && m_map.GetMapNumber() >= static_cast<int>(EMapId::GameplayMin))
    {
        if (m_editMode && !ObjectManager::MapFileExists(m_map.GetMapNumber()))
        {
            //저장 안 된(파일 없는) 맵에서는 편집 모드를 못 나가게 막는다.
            //안 그러면 유령 맵을 플레이하다 포탈 타면 빈 맵으로 떨어짐 (F5 저장 유도)
            m_editUnsavedWarnTimer = 120;
        }
        else
        {
            m_editMode = !m_editMode;
            m_editSelected.reset();
            m_editSelectMode = false;
        }
    }

    if (m_editUnsavedWarnTimer > 0)
        --m_editUnsavedWarnTimer;

    if (m_editMode)
    {
        UpdateEditMode();
        return;
    }

    UpdateGameplay(dt);
    UpdateFadeAndCamera();
}

void GameWorld::UpdateEditMode()
{
    if (!InputManager::IsRegistered())
        return;

    m_camera->ProcessInput();	//위/아래 화살표로 카메라 이동 (기존 플라잉캠 로직 재사용)

    auto& input = InputManager::Get();

    if (input.IsKeyPressed(VK_TAB))
    {
        m_editPlaceType = NextEditableType(m_editPlaceType);
        m_editRopePending = false;	//타입 바뀌면 로프 시작점 대기 취소
    }

    if (input.IsKeyPressed(VK_SPACE))
    {
        m_editSelectMode = !m_editSelectMode;
        m_editSelected.reset();
        m_editRopePending = false;
    }

    if (input.IsKeyPressed(VK_DELETE) && m_editSelected)
    {
        m_object_manager.RemoveObstacle(m_editSelected);
        m_editSelected.reset();
    }

    if (input.IsKeyPressed(VK_F5))
    {
        const bool ok = m_object_manager.SaveObstaclesToJson(m_map.GetMapNumber());
        std::cout << (ok ? "맵 저장 완료" : "맵 저장 실패") << std::endl;
        if (ok) m_editUnsavedWarnTimer = 0;	//저장했으니 경고 해제
    }

    //PageUp/PageDown: 포탈 없이 맵 번호를 직접 옮겨다님. 위로 넘어가면 새 맵을 만드는 셈
    if (input.IsKeyPressed(VK_PRIOR))
        JumpToEditMap(m_map.GetMapNumber() + 1);
    else if (input.IsKeyPressed(VK_NEXT))
        JumpToEditMap(m_map.GetMapNumber() - 1);

    //배치모드에서 커서 위치에 그릴 미리보기 오브젝트 갱신 (타입 바뀔 때만 새로 만들고, 위치는 매번 갱신)
    if (!m_editSelectMode && !m_editRopePending)
    {
        if (!m_editPreviewValid || m_editPreviewType != m_editPlaceType)
        {
            int w, h;
            GetDefaultObstacleSize(m_editPlaceType, w, h);
            m_editPreview = m_object_manager.CreateObstacle(m_editPlaceType, m_editMouseWorldX, m_editMouseWorldY, w, h, m_hinstance);
            m_editPreviewType = m_editPlaceType;
            m_editPreviewValid = true;
        }
        m_editPreview->SetX(m_editMouseWorldX);
        m_editPreview->SetY(m_editMouseWorldY);
    }
}

void GameWorld::JumpToEditMap(int newMapNumber)
{
    if (newMapNumber < static_cast<int>(EMapId::GameplayMin))
        return;	//타이틀 이전으로는 못 감

    //클리어맵보다 뒤로 가려 하면 = "새 게임맵을 클리어맵 앞에 끼워넣기".
    //클리어맵 개수를 늘려 클리어맵을 한 칸 뒤로 밀고, 방금 비워진 옛 클리어 자리를 새 게임맵으로 연다.
    if (newMapNumber > LevelConfig::GetClearMapNumber())
    {
        const int oldClear = LevelConfig::GetClearMapNumber();
        LevelConfig::EnsureGameplayMapCount(oldClear - static_cast<int>(EMapId::GameplayMin) + 1);
        newMapNumber = oldClear;	//옛 클리어 번호가 이제 새 게임맵 자리
    }

    m_object_manager.ResetObstacle();
    m_map.SetMapNumber(newMapNumber);
    m_map.SetBlackTime(0);
    m_map.CreateMap(m_hinstance);

    m_ocount = m_object_manager.InitObject(newMapNumber, m_hinstance);

    //플레이어 상태를 리셋해두지 않으면 편집 모드를 오가는 동안 속도/상태 값이 꼬인다
    m_player->Initialzie();
    m_player->SetCurrentHP(100);

    m_camera->SetX(0);
    m_camera->SetY(0);
    m_editSelected.reset();
    m_editRopePending = false;
    m_editPreviewValid = false;

    std::cout << "편집 모드: 맵 " << newMapNumber << "로 이동 (오브젝트 " << m_ocount << "개)" << std::endl;
}

void GameWorld::OnEditMouseDown(LPARAM mouse)
{
    const int worldX = m_camera->GetX() + LOWORD(mouse);
    const int worldY = m_camera->GetY() + HIWORD(mouse);

    if (m_editSelectMode)
    {
        //선택모드: 클릭은 선택/해제만 함, 절대 배치 안 함
        for (const auto& obs : m_object_manager.GetObjects())
        {
            const RECT box = GetEditHighlightBox(obs);
            if (worldX >= box.left && worldX <= box.right && worldY >= box.top && worldY <= box.bottom)
            {
                m_editSelected = obs;
                return;
            }
        }
        m_editSelected.reset();
        return;
    }

    //배치모드
    if (m_editPlaceType == EObstacleType::Rope)
    {
        //로프는 시작점/끝점을 2번 클릭해서 길이를 정한다
        if (!m_editRopePending)
        {
            m_editRopePending = true;
            m_editRopeStartX = worldX;
            m_editRopeStartY = worldY;
            return;
        }

        constexpr int kRopeWidth = 24;
        constexpr int kMinRopeHeight = 30;
        const int topY = std::min(m_editRopeStartY, worldY);
        const int height = std::max(std::abs(worldY - m_editRopeStartY), kMinRopeHeight);
        m_editSelected = m_object_manager.CreateAndRegisterObstacle(EObstacleType::Rope, m_editRopeStartX, topY, kRopeWidth, height, m_hinstance);
        m_editRopePending = false;
        return;
    }

    //그 외 타입: 클릭 한 번으로 기본 크기만큼 즉시 배치
    int w, h;
    GetDefaultObstacleSize(m_editPlaceType, w, h);
    m_editSelected = m_object_manager.CreateAndRegisterObstacle(m_editPlaceType, worldX, worldY, w, h, m_hinstance);
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

    if (m_editMode)
        RenderEditOverlay(mem1dc);

    BitBlt(hdc, 0, 0, GameConst::kViewportWidth, GameConst::kViewportHeight, mem1dc, m_camera->GetX(), m_camera->GetY(), SRCCOPY);

    DeleteDC(mem1dc);
    DeleteDC(mem2dc);
}

void GameWorld::UpdateGameplay(float dt)
{
    if (InputManager::IsRegistered())
    {
        m_player->ProcessInput();

        if (m_player->GetGameMode() && !m_player->IsDead())
            m_camera->ProcessInput();
    }

    m_player->Update(dt);
    m_object_manager.AdjustPlayer(m_player, m_map, m_ocount, m_hinstance);
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
    m_map.DrawUi(mem1dc, m_camera);
    m_map.DrawHP(mem1dc, m_camera, m_player);
    if (m_player->IsDead() == 1)
        m_deadUI.Render(mem1dc, m_camera);
}

void GameWorld::RenderEditOverlay(HDC mem1dc)
{
    if (m_editSelected)
    {
        const RECT box = GetEditHighlightBox(m_editSelected);
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
        HPEN oldPen = (HPEN)SelectObject(mem1dc, pen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(mem1dc, GetStockObject(NULL_BRUSH));
        Rectangle(mem1dc, box.left, box.top, box.right, box.bottom);
        SelectObject(mem1dc, oldBrush);
        SelectObject(mem1dc, oldPen);
        DeleteObject(pen);
    }

    if (m_editRopePending)
    {
        //로프 시작점 -> 현재 마우스 위치까지 미리보기 선
        HPEN pen = CreatePen(PS_DOT, 2, RGB(255, 255, 0));
        HPEN oldPen = (HPEN)SelectObject(mem1dc, pen);
        MoveToEx(mem1dc, m_editRopeStartX, m_editRopeStartY, nullptr);
        LineTo(mem1dc, m_editRopeStartX, m_editMouseWorldY);
        SelectObject(mem1dc, oldPen);
        DeleteObject(pen);
    }

    //배치모드: 커서 위치에 실제로 배치될 오브젝트 모양을 미리 그려줌 (선택모드/로프 대기 중엔 숨김)
    if (!m_editSelectMode && !m_editRopePending && m_editPreviewValid)
    {
        m_editPreview->DrawObj(mem1dc);
    }

    HFONT hfont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));
    HFONT oldFont = (HFONT)SelectObject(mem1dc, hfont);
    SetBkMode(mem1dc, TRANSPARENT);
    SetTextColor(mem1dc, RGB(255, 255, 0));

    wchar_t buf[280];
    if (m_editRopePending)
        swprintf_s(buf, L"[EDIT MODE - 배치] 맵 %d | 로프 끝점을 클릭하세요 (시작점 지정됨)", m_map.GetMapNumber());
    else
        swprintf_s(buf, L"[EDIT MODE - %s] 맵 %d (파일:%s) | 타입 번호: %d | 오브젝트: %d개 | Space 모드전환  Tab 타입변경  Del 삭제  F5 저장  PgUp/PgDn 맵이동  F1 종료",
            m_editSelectMode ? L"선택" : L"배치", m_map.GetMapNumber(),
            ObjectManager::MapFileExists(m_map.GetMapNumber()) ? L"있음" : L"없음",
            static_cast<int>(m_editPlaceType), static_cast<int>(m_object_manager.GetObjects().size()));
    TextOut(mem1dc, m_camera->GetX() + 10, m_camera->GetY() + 10, buf, lstrlenW(buf));

    //저장 안 된 맵에서 F1 종료를 시도했을 때 뜨는 경고
    if (m_editUnsavedWarnTimer > 0)
    {
        SetTextColor(mem1dc, RGB(255, 80, 80));
        const wchar_t* warn = L"저장되지 않은 맵입니다! F5로 저장한 뒤 종료하세요 (PgDn으로 이전 맵 복귀 가능)";
        TextOut(mem1dc, m_camera->GetX() + 10, m_camera->GetY() + 32, warn, lstrlenW(warn));
    }

    SelectObject(mem1dc, oldFont);
    DeleteObject(hfont);
}

void GameWorld::Shutdown()
{
    m_titleUI.Unload();
    m_deadUI.Unload();

    m_sound->Shutdown();
    SoundManager::Unregister();

	// GDI 비트맵
	if (m_hbit1) { DeleteObject(m_hbit1); m_hbit1 = nullptr; }
	if (m_clickCursor) { DestroyCursor(m_clickCursor); m_clickCursor = nullptr; }

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
    if (m_editMode)
    {
        m_editMouseWorldX = m_camera->GetX() + LOWORD(mouse);
        m_editMouseWorldY = m_camera->GetY() + HIWORD(mouse);
        return;
    }

    if (m_player->IsDead() == 1)
    {
        m_deadUI.OnMouseMove(mouse);
        return;
    }
    if (m_map.GetMapNumber() == static_cast<int>(EMapId::Title))
        m_titleUI.OnMouseMove(mouse);
}

void GameWorld::OnMouseDown(LPARAM mouse)
{
    if (m_clickCursor)
        SetCursor(m_clickCursor);

    if (m_editMode)
    {
        OnEditMouseDown(mouse);
        return;
    }

    if (m_player->IsDead() == 1)
    {
        m_deadUI.OnMouseDown(mouse);
        return;
    }
    if (m_map.GetMapNumber() == static_cast<int>(EMapId::Title))
        m_titleUI.OnMouseDown(mouse);
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
            if (m_hbit1) { DeleteObject(m_hbit1); m_hbit1 = nullptr; }
            m_hbit1 = (HBITMAP)LoadImage(m_hinstance, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            m_sound->SetIndex(m_sound->GetIndex() + 1);

            m_sound->PlayEffect(EEffect::Portal);
            m_sound->PlayBgm(EBgm::Level1);

            m_player->SetCurrentHP(100);
            m_camera->SetX(0);
            m_camera->SetY(GameConst::kDefaultCameraY);
        }
    }
}
