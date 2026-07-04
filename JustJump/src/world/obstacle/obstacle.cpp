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

void Obstacle::Update(float dt)
{
	Move();	//가상함수, 타입별로 움직임이 있으면 오버라이드

	const float period = GetAnimPeriodSec();
	if (period <= 0.f) return;
	m_animAccum += dt;
	if (m_animAccum >= period)
	{
		m_animAccum -= period;
		IndexChange();
	}
}
