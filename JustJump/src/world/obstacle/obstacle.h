#pragma once
#include "object/core/Object.h"

namespace ObstacleConst {
	constexpr int kBeltSpeed = 2;     // 벨트 이동 속도
	constexpr int kGearRowSpeed = 5;  // 가로 기어 속도
	constexpr int kGearColSpeed = 3;  // 세로 기어 속도
	constexpr int kGearRangeX = 150;  // 가로 기어 진동 범위
	constexpr int kGearRangeY = 100;  // 세로 기어 진동 범위
}

struct AnimStrip {
	int x0, y, w, h, stride; // src_x = x0 + index * stride
	int dst_h_offset = 0;    // 화면 출력 높이 = 오브젝트 height + dst_h_offset
};

struct SpriteRect {
	int x, y, w, h;
	int dst_h_offset = 0; // 화면 출력 높이 = 오브젝트 height + dst_h_offset
	int dst_w_offset = 0; // 화면 출력 너비 = 오브젝트 width  + dst_w_offset
};

namespace ObstacleSprite {
	constexpr AnimStrip kBelt        { 16,  9, 250,  43, 272, 42 };
	constexpr AnimStrip kGear        {  0,  4,  18,  18,  23     };
	constexpr AnimStrip kPortal      {  0, 55,  63, 135,  79     };

	constexpr SpriteRect kPlatform   { 11, 15,  77,  18, 17 };   // type 2
	constexpr SpriteRect kSmall      {  0,  0,  19,  19, 18 };   // type 3
	constexpr SpriteRect kNail       {  1,  0,  26,  15,  0, 11 }; // type 101
	constexpr SpriteRect kBrokenPipe {  0,  1,  17,  75 };        // type 102

	namespace Platform7 {                                          // type 7
		constexpr SpriteRect kHead  { 11, 15, 14, 18 };
		constexpr SpriteRect kBody  { 27, 15, 50, 18 };
		constexpr SpriteRect kTail  { 78, 15, 13, 18 };
		constexpr int kBodyCount    = 6;
		constexpr int kDstHOffset   = 17;
	}

	namespace Rope {
		constexpr SpriteRect kHead {  0,   0, 24, 32 };
		constexpr SpriteRect kBody {  0,  33, 24, 41 };
		constexpr SpriteRect kTail {  0, 148, 24, 21 };
	}
}

class Obstacle : public Object
{

public:
	//오브젝트의 x y 는 왼쪽상단이 기준이다.

	int GetMX() { return mx; }
	int GetMY() { return my; }
	int GetSpriteIndex() { return index; }

	EObstacleType GetType() const { return type; }

	//오브젝트가 몇개들어갔는지
	//int getocount();

	void SetType(EObstacleType _type) { type = _type; }
	void SetHbit(HINSTANCE g_hinst);

public:

	void Update(float dt) override;

	void Create(int x, int y, int width, int height, int type);
	// All objects reset
	void ResetObject();
	//Move gear object (기본은 안 움직임, GearObstacle이 오버라이드)
	virtual void Move() {}
	//오브젝트 애니메이션 쓸때 사용하는함수
	virtual void IndexChange();
	//오브젝트를 그려줌
	virtual void DrawObj(HDC& mem1dc);

protected:

	virtual float GetAnimPeriodSec() const;

	int mx, my;					// x,y 축기준 증감량

	int dir;					//오브젝트 이동방향n 0 == left , right / 1 == up , down
	int s;						//부호

	EObstacleType type;
	int index;					//애니메이션 돌릴때 배열의 인덱스를 바꿔주어 이미지를 바꾼다

	HBITMAP hbit;
	HDC odc;

private:

	float m_animAccum = 0.f;	// 애니메이션 누적 시간
};

//bool collp2w(PLAYER, OBJECT);