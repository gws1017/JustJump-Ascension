#include "Global.h"
#include "world/obstacle/GasObstacle.h"

void GasObstacle::IndexChange()
{
	index += 1;
	if (index >= 4) index = 0;
}

float GasObstacle::GetAnimPeriodSec() const
{
	return AnimPeriod::kGas * GameConst::kAnimTickSeconds;
}

void GasObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit);

	RECT gas[4] = { {0,2,7,7},{10,2,18,7},{31,0,33,9},{0,0,0,0} };
	int tx;
	if (index == 0) tx = x + 26;
	else if (index == 1) tx = x + 15;
	else if (index == 2) tx = x;
	else tx = 0;
	TransparentBlt(mem1dc, tx, y, gas[index].right, gas[index].bottom, odc, gas[index].left, gas[index].top, gas[index].right, gas[index].bottom, RGB(255, 255, 255));

	DeleteDC(odc);
}
