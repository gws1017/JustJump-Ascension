#include "Global.h"
#include "world/obstacle/GroundObstacle.h"

void GroundObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit.get());
	TransparentBlt(mem1dc, x, y, width, height, odc, 0, 0, 1023, 62, RGB(255, 255, 255));
	DeleteDC(odc);
}
