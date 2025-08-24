#pragma once
#include "object/core/Object.h"

#define beltspeed 2 //belt speed
#define gearrowspeed 5 //row gear speed
#define gearcolspeed 3 // col gear speed
class Obstacle : public Object
{

public:
	//오브젝트의 x y 는 왼쪽상단이 기준이다.

	int GetMX() { return mx; }
	int GetMY() { return my; }
	int GetSpriteIndex() { return index; }

	//1.바닥 2.기본 발판 3.작은발판 4.컨베이어 벨트 5.투명바닥 101.나사못 102.깨진파이프 103.증기 106,107.톱니바퀴 201.포탈 301.로프
	int GetType() { return type; }
	
	//오브젝트가 몇개들어갔는지
	//int getocount();

	//1~100 플랫폼 101~장애물
	void SetType(int _type) { type = _type; }
	void SetHbit(HINSTANCE g_hinst);

public:

	void Create(int x, int y, int width, int height, int type);
	// All objects reset
	void ResetObject();
	//Move gear object
	void Move();
	//오브젝트 애니메이션 쓸때 사용하는함수
	void IndexChange();
	//오브젝트를 그려줌
	void DrawObj(HDC& mem1dc);

private:

	int mx, my;					// x,y 축기준 증감량

	int dir;					//오브젝트 이동방향n 0 == left , right / 1 == up , down
	int s;						//부호

	int type;					//1.바닥 2.기본 발판 3.작은발판 4.컨베이어 벨트 5.투명바닥 101.나사못 102.깨진파이프 103.증기 106,107.톱니바퀴 201.포탈 301.로프
	int index;					//애니메이션 돌릴때 배열의 인덱스를 바꿔주어 이미지를 바꾼다

	HBITMAP hbit;
	HDC odc;
};

//bool collp2w(PLAYER, OBJECT);