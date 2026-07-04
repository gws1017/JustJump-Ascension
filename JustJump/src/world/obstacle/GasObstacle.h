#pragma once
#include "world/obstacle/obstacle.h"

class GasObstacle : public Obstacle
{
public:
	void DrawObj(HDC& mem1dc) override;
	EHitboxKind GetHitboxKind() const override { return EHitboxKind::Box; }

protected:
	void IndexChange() override;
	float GetAnimPeriodSec() const override;
};
