#pragma once
#include "world/obstacle/obstacle.h"

class LongPlatObstacle : public Obstacle
{
public:
	void DrawObj(HDC& mem1dc) override;
};
