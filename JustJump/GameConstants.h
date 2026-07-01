#pragma once
#include <cstdint>

namespace GameConst {
    constexpr int kViewportWidth = 1024;
    constexpr int kViewportHeight = 768;

    constexpr float kMaxDeltaTime = 0.05f;
    constexpr float kAnimTickSeconds = 1.0f / 60.0f;
    constexpr int   kAnimCounterMax = 27000;

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
}

// 맵 ID
enum class EMapId : int {
    Title = 9,
    GameplayMin = 10,
    Clear = 13,
};

// 오브젝트 타입
enum class EObstacleType : int {
    AnimatedBg0 = 0,
    Ground = 1,
    // ... 플랫폼 2~100
    HazardMin = 101,
    Gas = 103,
    GearA = 106,
    GearB = 107,
    Portal = 201,
    Rope = 301,
};

// UI 히트박스
struct UiRect {
    int left, top, right, bottom;
};
namespace UiHitbox {
    constexpr UiRect kRevive{ 584, 338, 620, 352 };
    constexpr UiRect kStart{ 290, 490, 430, 572 };
    constexpr UiRect kHelp{ 290, 345, 428, 427 };
}