#pragma once
#include "world/obstacle/obstacle.h"

//가로/세로로 왕복하며 회전하는 톱니바퀴 (GearRow, GearCol)
class GearObstacle : public Obstacle
{
public:
	void DrawObj(HDC& mem1dc) override;
	EHitboxKind GetHitboxKind() const override { return EHitboxKind::Box; }

protected:
	void Move() override;
	void IndexChange() override;
	float GetAnimPeriodSec() const override;
};
