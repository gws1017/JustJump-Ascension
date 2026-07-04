#pragma once
#include "world/obstacle/obstacle.h"

class SmallPlatObstacle : public Obstacle
{
public:
	void DrawObj(HDC& mem1dc) override;
};
