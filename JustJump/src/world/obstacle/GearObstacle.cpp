#include "Global.h"
#include "world/obstacle/GearObstacle.h"

void GearObstacle::Move()
{
	if (dir == 0) //Left or Right
	{
		if (mx > ObstacleConst::kGearRangeX) s = -1;
		else if (mx < -ObstacleConst::kGearRangeX) s = 1;
		mx += s * ObstacleConst::kGearRowSpeed;
	}
	else if (dir == 1) //Up or Down
	{
		if (my > ObstacleConst::kGearRangeY) s = -1;
		else if (my < -ObstacleConst::kGearRangeY) s = 1;
		my += s * ObstacleConst::kGearColSpeed;
	}
}

void GearObstacle::IndexChange()
{
	index += 1;
	if (index >= 2) index = 0;	//톱니바퀴 회전하는 이미지 2개
}

float GearObstacle::GetAnimPeriodSec() const
{
	return AnimPeriod::kGear * GameConst::kAnimTickSeconds;
}

void GearObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit.get());
	const auto& g = ObstacleSprite::kGear;
	TransparentBlt(mem1dc, x + mx, y + my, width, height, odc, g.x0 + index * g.stride, g.y, g.w, g.h, RGB(255, 255, 255));
	DeleteDC(odc);
}
