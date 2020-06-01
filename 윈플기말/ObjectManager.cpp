#include "ObjectManager.h"


//땅바닥과 플레이어 충돌체크 1이면 부닥침
bool collp2w(PLAYER player, OBJECT object)
//----------------------------------------
{
	int adjust = 10 ;
	if (object.getType() == 1)
	{
		if (object.getX() <= player.getx() && player.getx() <= object.getX() + object.getW())
		{
			if (object.getY() <= player.gety() + player.geth())
			{
				return 1;
			}
		}
	}
	else {
		if (object.getX() <= player.getx() && player.getx() <= object.getX() + object.getW())
		{
			if (object.getY() <= player.gety() + player.geth() && player.gety() + player.geth() <= object.getY() + adjust)
			{
				return 1;
				//ㅇㅇ
			}
		}
	}
	
	return 0;
}


//플레이어와 오브젝트간 상호작용 판단하고 그에맞게 바꿔줌
void adjustPlayer(PLAYER& player, OBJECT* obj,int ocount)
{
	for (int i = 0; i < ocount; ++i)
	{
		if (player.getstate() == 2)		//점프중일땐 적어도 바닥이랑은 충돌 없게한다
			break;
		if (player.getstate() == 3)		//어차피 7번 아니면 아래로 안떨어지니까는 ㄱㅊ 숙이기,점프중일때는 충돌처리 안해쥼
			break;
		if (collp2w(player, obj[i]))
		{
			player.sety(obj[i].getY() - player.geth());
			player.setstate(1);
			if (ROWSPEED == 1)
				ROWSPEED = 5;
			break;
		}
		else {
			if (player.getstate() == 4 || player.getstate() == 1)
				player.setstate(7);
		}
	}
}
//int(맵 번호) 에 따라 장애물 위치값 넣어주고 몇개의 오브젝트가 들어갔는지 알려주는 함수
int initObject(OBJECT* obj, int mapnum)
{
	int x, y, w, h, type;
	int objcount = 0;
	
	ifstream in;
	if (mapnum == 10)
	{
		in.open("map/map_1.txt", ios::in);
	}
	else if (mapnum == 11)
	{
		in.open("map/map_2.txt", ios::in);
	}
	else {
		return 0;		//맵 값이 잘못입력되었으면 그대로 탈출
	}




	if (in.is_open())
	{
		for (int i = 0; i < 100; ++i)
		{
			if (in.eof())
			{
				objcount = i;
				break;
			}
			in >> x >> y >> w >> h >> type;
			obj[i].setX(x);
			obj[i].setY(y);
			obj[i].setW(w);
			obj[i].setH(h);
			obj[i].setType(type);

		}
	}
	return objcount;
}

//카메라 무빙워크
void adjustCamera(CAMERA& camera,PLAYER player)
{
	//플레이어가 카메라의 중간지점을 넘어가면 카메라도 올려준다
	if (player.gety() < camera.gety() + 384)
	{
		if (camera.gety() <= 0)	//최상점일땐 이동해주지않음
		{

		}
		else {
			camera.sety(player.gety() - 384);
		}
	}
	else if (player.gety() > camera.gety() + 384)
	{
		if (camera.gety() >= 3232)	//최하점일땐 이동해주지않음
		{

		}
		else {
			camera.sety(player.gety() - 384);
		}
	}



}