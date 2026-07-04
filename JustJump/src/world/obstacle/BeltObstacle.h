#pragma once
#include "world/obstacle/obstacle.h"

//BeltRight/BeltLeft 공용 (그리기는 동일, 방향은 type 필드로 별도 판정)
class BeltObstacle : public Obstacle
{
public:
	void DrawObj(HDC& mem1dc) override;
	EHitboxKind GetHitboxKind() const override { return EHitboxKind::Platform; }

protected:
	void IndexChange() override;
	float GetAnimPeriodSec() const override;
};
