#pragma once
#include "world/obstacle/obstacle.h"

//다음 맵으로 이동하는 포탈
class PortalObstacle : public Obstacle
{
public:
	void DrawObj(HDC& mem1dc) override;
	EHitboxKind GetHitboxKind() const override { return EHitboxKind::Box; }

protected:
	void IndexChange() override;
	float GetAnimPeriodSec() const override;
};
