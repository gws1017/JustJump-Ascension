#include "Global.h"
#include "world/obstacle/LongPlatObstacle.h"

void LongPlatObstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit.get());

	const auto& ph = ObstacleSprite::Platform7::kHead;
	const auto& pb = ObstacleSprite::Platform7::kBody;
	const auto& pt = ObstacleSprite::Platform7::kTail;
	const int dh = ObstacleSprite::Platform7::kDstHOffset;

	TransparentBlt(mem1dc, x, y, ph.w, height + dh, odc, ph.x, ph.y, ph.w, ph.h, RGB(255, 255, 255));
	for (int i = 0; i < ObstacleSprite::Platform7::kBodyCount; i++)
	{
		TransparentBlt(mem1dc, x + ph.w + pb.w * i, y, pb.w, height + dh, odc, pb.x, pb.y, pb.w, pb.h, RGB(255, 255, 255));
	}
	TransparentBlt(mem1dc, x + ph.w + pb.w * ObstacleSprite::Platform7::kBodyCount, y, pt.w, height + dh, odc, pt.x, pt.y, pt.w, pt.h, RGB(255, 255, 255));

	DeleteDC(odc);
}
