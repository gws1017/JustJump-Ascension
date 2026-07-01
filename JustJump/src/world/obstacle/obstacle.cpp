#pragma comment (lib, "Msimg32.lib")
#include "Global.h"

#include "obstacle.h"
#include "object/character/player.h"


void Obstacle::Create(int _x, int _y, int _w, int _h, int _type)
{
	x = _x;
	y = _y;
	width = _w;
	height = _h;
	type = static_cast<EObstacleType>(_type);
	index = 0;

	mx = 0;
	my = 0;

	//row gear
	if (type == EObstacleType::GearRow)
	{
		s = 1;
		dir = 0;
	}
	//col gear
	else if (type == EObstacleType::GearCol)
	{
		s = 1;
		dir = 1;
	}
}


void Obstacle::SetHbit(HINSTANCE g_hinst)
{
	hbit = LoadObj(hbit, g_hinst, static_cast<int>(type));
}

// All objects reset
void Obstacle::ResetObject()
{
	if(hbit)
	{
		DeleteObject(hbit);
		hbit = NULL;
	}
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	type = EObstacleType::AnimatedBg;
	index = 0;
	hbit = NULL;
}
//인덱스를 바꿔주는함수
void Obstacle::IndexChange()
{
	index += 1;
	if (type == EObstacleType::AnimatedBg)
	{
		if (index >= 7) index = 0;
	}
	else if (type == EObstacleType::BeltRight || type == EObstacleType::BeltLeft)
	{
		if (index >= 4) index = 0;
	}
	else if (type == EObstacleType::Gas) //가스 공백포함 이미지 4개
	{
		if (index >= 4) index = 0;
	}
	else if (type == EObstacleType::GearRow || type == EObstacleType::GearCol) //톱니바퀴 회전하는 이미지 2개
	{
		if (index >= 2) index = 0;
	}
	else if (type == EObstacleType::Portal) //포탈 이미지 7개
	{
		if (index >= 7) index = 0;
	}
}

void Obstacle::DrawObj(HDC& mem1dc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit);
	if (type == EObstacleType::Ground)
	{
		TransparentBlt(mem1dc, x, y, width, height, odc, 0, 0, 1023, 62, RGB(255, 255, 255));
	}
	else if (type == EObstacleType::AnimatedBg)
		TransparentBlt(mem1dc, x, y, width, height, odc, 0, 0 + index * 768, 1024, 768, RGB(142, 203, 162));
	else if (type == EObstacleType::Platform)
		TransparentBlt(mem1dc, x, y, width, height + ObstacleSprite::kPlatform.dst_h_offset, odc, ObstacleSprite::kPlatform.x, ObstacleSprite::kPlatform.y, ObstacleSprite::kPlatform.w, ObstacleSprite::kPlatform.h, RGB(255, 255, 255));	// 원본그림에서 x 11~88 y 15 33 만큼 잘라내서 투명처리후 출력
	else if (type == EObstacleType::SmallPlat)
		TransparentBlt(mem1dc, x, y, width, height + ObstacleSprite::kSmall.dst_h_offset, odc, ObstacleSprite::kSmall.x, ObstacleSprite::kSmall.y, ObstacleSprite::kSmall.w, ObstacleSprite::kSmall.h, RGB(255, 255, 255));
	else if (type == EObstacleType::BeltRight || type == EObstacleType::BeltLeft)
		TransparentBlt(mem1dc, x, y, width, height + ObstacleSprite::kBelt.dst_h_offset, odc, ObstacleSprite::kBelt.x0 + index * ObstacleSprite::kBelt.stride, ObstacleSprite::kBelt.y, ObstacleSprite::kBelt.w, ObstacleSprite::kBelt.h, RGB(0, 0, 0));
	else if (type == EObstacleType::LongPlat)
	{
		const auto& ph = ObstacleSprite::Platform7::kHead;
		const auto& pb = ObstacleSprite::Platform7::kBody;
		const auto& pt = ObstacleSprite::Platform7::kTail;
		const int dh = ObstacleSprite::Platform7::kDstHOffset;
		TransparentBlt(mem1dc, x, y, ph.w, height + dh, odc, ph.x, ph.y, ph.w, ph.h, RGB(255, 255, 255));//head
		for (int i = 0; i < ObstacleSprite::Platform7::kBodyCount; i++)
		{
			TransparentBlt(mem1dc, x + ph.w + pb.w * i, y, pb.w, height + dh, odc, pb.x, pb.y, pb.w, pb.h, RGB(255, 255, 255));//body
		}
		TransparentBlt(mem1dc, x + ph.w + pb.w * ObstacleSprite::Platform7::kBodyCount, y, pt.w, height + dh, odc, pt.x, pt.y, pt.w, pt.h, RGB(255, 255, 255));//tail
	}
	else if (type == EObstacleType::Transparent)
		TransparentBlt(mem1dc, x, y, width, height, odc, 0, 9, 0, 0, RGB(255, 255, 255));
	else if (type == EObstacleType::Nail)
		TransparentBlt(mem1dc, x, y, width + ObstacleSprite::kNail.dst_w_offset, height, odc, ObstacleSprite::kNail.x, ObstacleSprite::kNail.y, ObstacleSprite::kNail.w, ObstacleSprite::kNail.h, RGB(255, 255, 255));
	else if (type == EObstacleType::BrokenPipe)
		TransparentBlt(mem1dc, x, y, width, height, odc, ObstacleSprite::kBrokenPipe.x, ObstacleSprite::kBrokenPipe.y, ObstacleSprite::kBrokenPipe.w, ObstacleSprite::kBrokenPipe.h, RGB(255, 255, 255));
	else if (type == EObstacleType::Gas) // 103번의 경우 102번의 y값에서 51을 뺀 위치가 파이프 깨진부분이다.
	{
		//그림의 크기가 각각 다르기때문에 임시변수를 만들어  값을 저장하고 인덱스에따라 바꿔주는 형식이다.
		RECT gas[4] = { {0,2,7,7},{10,2,18,7},{31,0,33,9},{0,0,0,0} }; //1번째인자는 아무것도 안그리는것
		int tx;
		if (index == 0) tx = x + 26;
		else if (index == 1) tx = x + 15;
		else if (index == 2) tx = x;
		else tx = 0;
		TransparentBlt(mem1dc, tx, y, gas[index].right, gas[index].bottom, odc, gas[index].left, gas[index].top, gas[index].right, gas[index].bottom, RGB(255, 255, 255));
	}
	else if (type == EObstacleType::GearRow || type == EObstacleType::GearCol)
	{
		const auto& g = ObstacleSprite::kGear;
		TransparentBlt(mem1dc, x+mx, y+my, width, height, odc, g.x0 + index * g.stride, g.y, g.w, g.h, RGB(255, 255, 255));
	}
	else if (type == EObstacleType::Portal)
	{
		const auto& p = ObstacleSprite::kPortal;
		TransparentBlt(mem1dc, x, y, width, height, odc, p.x0 + index * p.stride, p.y, p.w, p.h, RGB(0, 0, 0));
	}
	else if (type == EObstacleType::Rope)
	{
		//머리+ 꼬리 53 몸통 41 전체 94 기본 길이 94 더긴거는 135 176 217 258 299 340 381 422 463
		const auto& rh = ObstacleSprite::Rope::kHead;
		const auto& rb = ObstacleSprite::Rope::kBody;
		const auto& rt = ObstacleSprite::Rope::kTail;
		int body = (height - (rh.h + rt.h)) / rb.h;
		TransparentBlt(mem1dc, x, y, width, rh.h, odc, rh.x, rh.y, rh.w, rh.h, RGB(255, 255, 255));//head
		for (int i = 0; i < body; i++)
		{
			TransparentBlt(mem1dc, x, y + rh.h + i * rb.h, width, rb.h, odc, rb.x, rb.y, rb.w, rb.h, RGB(255, 255, 255));//body
		}
		TransparentBlt(mem1dc, x, y + rh.h + body * rb.h, width, rt.h, odc, rt.x, rt.y, rt.w, rt.h, RGB(255, 255, 255));//tail
	}
	DeleteDC(odc);
}

void Obstacle::Move()
{
	
	if (dir == 0) //Left or Down
	{
		if (mx > ObstacleConst::kGearRangeX) s = -1;
		else if (mx < -ObstacleConst::kGearRangeX) s = 1;
		mx += s * ObstacleConst::kGearRowSpeed;

	}
	else if (dir == 1) //Up or Down
	{
		if (my > ObstacleConst::kGearRangeY) s = -1;
		else if (my < -ObstacleConst::kGearRangeY) s = 1;
		my += s * ObstacleConst::kGearColSpeed;

	}
	
}
//땅바닥과 플레이어 충돌체크 1이면 부닥침
//
//bool collp2w(PLAYER player, OBJECT obj)
//{
//	if (obj.getX() <= player.getx() && player.getx() + player.getw() <= obj.getX() + obj.getW())
//	{
//		if (obj.getY() <= player.gety()+player.geth())
//		{
//			return 1;
//		}
//	}
//	return 0;
//	/*if (player.getx() + player.getw() < obj.getX() || obj.getX() + obj.getW() < player.getx()) return 0;
//	if (player.gety() + player.geth() < obj.getY() || obj.getY() + obj.getH() < player.gety())return 0;
//
//	return 1;
//*/
//
//}