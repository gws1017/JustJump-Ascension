#include "Global.h"
#include "world/obstacle/BeltObstacle.h"

void BeltObstacle::IndexChange()
{
	index += 1;
	if (index >= 4) index = 0;
}

float BeltObstacle::GetAnimPeriodSec() const
{
	return AnimPeriod::kBelt * GameConst::kAnimTickSeconds;
}

void BeltObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit.get());
	const auto& b = ObstacleSprite::kBelt;
	TransparentBlt(mem1dc, x, y, width, height + b.dst_h_offset, odc, b.x0 + index * b.stride, b.y, b.w, b.h, RGB(0, 0, 0));
	DeleteDC(odc);
}
