#include "Global.h"
#include "world/obstacle/TransparentObstacle.h"

void TransparentObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit.get());
	TransparentBlt(mem1dc, x, y, width, height, odc, 0, 9, 0, 0, RGB(255, 255, 255));
	DeleteDC(odc);
}
