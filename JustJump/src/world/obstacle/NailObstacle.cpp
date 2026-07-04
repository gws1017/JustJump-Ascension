#include "Global.h"
#include "world/obstacle/NailObstacle.h"

void NailObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit.get());
	const auto& n = ObstacleSprite::kNail;
	TransparentBlt(mem1dc, x, y, width + n.dst_w_offset, height, odc, n.x, n.y, n.w, n.h, RGB(255, 255, 255));
	DeleteDC(odc);
}
