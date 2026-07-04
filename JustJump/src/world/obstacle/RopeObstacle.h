#pragma once
#include "world/obstacle/obstacle.h"

class RopeObstacle : public Obstacle
{
public:
	void DrawObj(HDC& mem1dc) override;
};
