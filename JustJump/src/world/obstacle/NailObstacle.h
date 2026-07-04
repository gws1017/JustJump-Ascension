#pragma once
#include "world/obstacle/obstacle.h"

class NailObstacle : public Obstacle
{
public:
	void DrawObj(HDC& mem1dc) override;
	EHitboxKind GetHitboxKind() const override { return EHitboxKind::Box; }
};
