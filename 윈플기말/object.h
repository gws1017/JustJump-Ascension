#pragma once
#include <windows.h>
#include "player.h"

#define gearrowspeed 5 //row gear speed
#define gearcolspeed 3 // col gear speed
class OBJECT {
	int x, y;					//오브젝트의 x y 는 왼쪽상단이 기준이다.
	int w, h;
	int type;					//1~100 플랫폼 101~장애물 201~상호작용 오브젝트
	int index;					//애니메이션 돌릴때 배열의 인덱스를 바꿔주어 이미지를 바꾼다
	int mx, my;					//Variables that increase/decrease based on the object's x,y coordinates
	int s;						//sign bit
	int dir;					//object's moving direction 0 == left , right / 1 == up , down
	HBITMAP hbit;				
public:
	//오브젝트의 x y 는 왼쪽상단이 기준이다.
	int getX();
	int getY();
	int getW();
	int getH();
	int getindex();

	//1~100 플랫폼 101~장애물 201~상호작용 오브젝트
	int getType();
	//오브젝트가 몇개들어갔는지
	int getocount();

	int getmx();
	int getmy();

	void create(int, int, int, int, int);
	//오브젝트의 x y 는 왼쪽상단이 기준이다.
	void setX(int);
	//오브젝트의 x y 는 왼쪽상단이 기준이다.
	void setY(int);
	void setW(int);
	void setH(int);
	//1~100 플랫폼 101~장애물
	void setType(int);
	void setHbit(HINSTANCE);

	// All objects reset
	void ResetObject();
	//Move gear object
	void move();
	//오브젝트 애니메이션 쓸때 사용하는함수
	void IndexChange();
	//오브젝트를 그려줌
	void DrawObj(HDC&, HDC&);
};

//bool collp2w(PLAYER, OBJECT);