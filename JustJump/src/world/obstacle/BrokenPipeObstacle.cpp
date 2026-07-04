#include "Global.h"
#include "world/obstacle/BrokenPipeObstacle.h"

void BrokenPipeObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit.get());
	const auto& b = ObstacleSprite::kBrokenPipe;
	TransparentBlt(mem1dc, x, y, width, height, odc, b.x, b.y, b.w, b.h, RGB(255, 255, 255));
	DeleteDC(odc);
}
