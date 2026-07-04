#include "Global.h"
#include "world/obstacle/PlatformObstacle.h"

void PlatformObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit);
	const auto& p = ObstacleSprite::kPlatform;
	TransparentBlt(mem1dc, x, y, width, height + p.dst_h_offset, odc, p.x, p.y, p.w, p.h, RGB(255, 255, 255));
	DeleteDC(odc);
}
