#pragma once
#include "world/obstacle/obstacle.h"

class AnimatedBgObstacle : public Obstacle
{
public:
	void DrawObj(HDC& mem1dc) override;
	EHitboxKind GetHitboxKind() const override { return EHitboxKind::None; }

protected:
	void IndexChange() override;
	float GetAnimPeriodSec() const override;
};
