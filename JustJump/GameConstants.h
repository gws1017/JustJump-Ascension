#pragma once
#include <cstdint>

namespace GameConst {
    constexpr int kViewportWidth = 1024;
    constexpr int kViewportHeight = 768;

    constexpr float kMaxDeltaTime = 0.05f;
    constexpr float kAnimTickSeconds = 1.0f / 60.0f;

    constexpr int kAlphaMax = 255;
    constexpr int kFadeStep = 40;
    constexpr int kMapFadeDuration = 50;   // SetBlackTime(50)
    constexpr int kDefaultPlayerHp = 100;

    constexpr int kWorldMaxX = 1023;       // ObjectManager 경계
    constexpr int kPortalCheatOffset = 40; // OnChar 'r'

    constexpr int kDefaultCameraY = 3232;  // Camera.h 기본 y, GameWorld SetY(3232)

    constexpr int kWorldHeight = 4096;       // 맵 전체 세로 크기, 카메라 스크롤 하한
    constexpr int kCameraScrollSpeed = 20;   // 카메라 상/하 스크롤 속도(px/프레임)

    constexpr int kMapBitmapHeight = 4098;   // 맵 배경 비트맵 세로 크기 (kWorldHeight와 2px 차이, 원본 에셋 크기)
    constexpr int kMapScrollSpeed = 5;       // 타이틀 화면 배경 스크롤 속도

    // 플레이어
    constexpr int kPlayerSpawnX       = 80;   // 플레이어 스폰 X (타이틀/게임 공통)
    constexpr int kPlayerTitleY       = 655;  // 타이틀 화면 플레이어 Y
    constexpr int kPlayerSpawnY       = 3700; // 게임 스폰 Y
    constexpr int kPlayerWidth        = 14;   // 플레이어 기본 너비
    constexpr int kPlayerHeight       = 25;   // 플레이어 기본 높이
    constexpr int kCrouchHeightDelta  = 12;   // 웅크리기 시 높이/y 변화량
    constexpr int kRopeJumpCooldown   = 2;    // 로프 점프 후 재탑승 방지 프레임
    constexpr int kInvincibleFrames   = 100;  // 피격 무적 시간(프레임)
    constexpr int kFallDamageThreshold = 200; // 낙뎀 발생 최소 낙하 거리(px)

    // 월드/게임플레이
    constexpr int kPlatformCollisionAdjust = 10;  // 발판 충돌 판정 여유값
    constexpr int kMaxObstaclesPerMap      = 150; // 맵당 최대 오브젝트 수
}

// 오브젝트 애니메이션 주기 (obj_t % N == 0)
namespace AnimPeriod {
    constexpr int kAnimatedBg = 10;
    constexpr int kBelt       = 8;
    constexpr int kGas        = 30;
    constexpr int kGear       = 5;
    constexpr int kPortal     = 20;
}

// 맵 ID
enum class EMapId : int {
    Title = 9,
    GameplayMin = 10,
    Clear = 13,
};

// 오브젝트 타입
enum class EObstacleType : int {
    AnimatedBg  =   0,
    Ground      =   1,
    Platform    =   2,  // 기본 발판
    SmallPlat   =   3,  // 작은 발판
    BeltRight   =   4,  // 컨베이어 벨트 우향
    Transparent =   5,  // 투명 바닥
    BeltLeft    =   6,  // 컨베이어 벨트 좌향
    LongPlat    =   7,  // 긴 발판
    // 8~100: 미사용 플랫폼
    Nail        = 101,  // 나사못
    BrokenPipe  = 102,  // 깨진 파이프
    Gas         = 103,  // 증기
    // 104: Break Pipe Right (미구현)
    // 105: Gas Right (미구현)
    GearRow     = 106,  // 가로 톱니바퀴
    GearCol     = 107,  // 세로 톱니바퀴
    // 108~200: 미사용 장애물
    Portal      = 201,
    // 202~300: 미사용 인터랙티브
    Rope        = 301,
};

namespace ObstacleTypeUtil {
    inline bool IsPlatform(EObstacleType t)    { auto v = static_cast<int>(t); return v > 0   && v <= 100; }
    // 피격 반응이 있는 장애물 (101~200)
    inline bool IsHazard(EObstacleType t)      { auto v = static_cast<int>(t); return v >= 101 && v <= 200; }
    inline bool IsInteractive(EObstacleType t) { auto v = static_cast<int>(t); return v >= 201 && v <= 300; }
    inline bool IsRope(EObstacleType t)        { auto v = static_cast<int>(t); return v >= 301 && v <  401; }
}

// UI 히트박스
struct UiRect {
    int left, top, right, bottom;
};
namespace UiHitbox {
    constexpr UiRect kRevive{ 584, 338, 620, 352 };
    constexpr UiRect kStart{ 290, 490, 430, 572 };
    constexpr UiRect kHelp{ 290, 345, 428, 427 };
}