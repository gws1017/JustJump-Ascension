#include "Global.h"
#include "world/obstacle/PortalObstacle.h"

void PortalObstacle::IndexChange()
{
	index += 1;
	if (index >= 7) index = 0;	//포탈 이미지 7개
}

float PortalObstacle::GetAnimPeriodSec() const
{
	return AnimPeriod::kPortal * GameConst::kAnimTickSeconds;
}

void PortalObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit);
	const auto& p = ObstacleSprite::kPortal;
	TransparentBlt(mem1dc, x, y, width, height, odc, p.x0 + index * p.stride, p.y, p.w, p.h, RGB(0, 0, 0));
	DeleteDC(odc);
}
