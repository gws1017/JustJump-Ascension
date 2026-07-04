#include "Global.h"
#include "world/obstacle/AnimatedBgObstacle.h"

void AnimatedBgObstacle::IndexChange()
{
	index += 1;
	if (index >= 7) index = 0;
}

float AnimatedBgObstacle::GetAnimPeriodSec() const
{
	return AnimPeriod::kAnimatedBg * GameConst::kAnimTickSeconds;
}

void AnimatedBgObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit);
	TransparentBlt(mem1dc, x, y, width, height, odc, 0, 0 + index * 768, 1024, 768, RGB(142, 203, 162));
	DeleteDC(odc);
}
