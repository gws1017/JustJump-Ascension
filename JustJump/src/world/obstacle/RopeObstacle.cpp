#include "Global.h"
#include "world/obstacle/RopeObstacle.h"

void RopeObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit);

	const auto& rh = ObstacleSprite::Rope::kHead;
	const auto& rb = ObstacleSprite::Rope::kBody;
	const auto& rt = ObstacleSprite::Rope::kTail;
	int body = (height - (rh.h + rt.h)) / rb.h;

	TransparentBlt(mem1dc, x, y, width, rh.h, odc, rh.x, rh.y, rh.w, rh.h, RGB(255, 255, 255));
	for (int i = 0; i < body; i++)
	{
		TransparentBlt(mem1dc, x, y + rh.h + i * rb.h, width, rb.h, odc, rb.x, rb.y, rb.w, rb.h, RGB(255, 255, 255));
	}
	TransparentBlt(mem1dc, x, y + rh.h + body * rb.h, width, rt.h, odc, rt.x, rt.y, rt.w, rt.h, RGB(255, 255, 255));

	DeleteDC(odc);
}
