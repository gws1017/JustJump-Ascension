#pragma once
#include "world/obstacle/obstacle.h"

class TransparentObstacle : public Obstacle
{
public:
	void DrawObj(HDC& mem1dc) override;
};
