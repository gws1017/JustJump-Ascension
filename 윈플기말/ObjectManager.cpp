#include "ObjectManager.h"


//오브젝트와 플레이어 충돌체크 1이면 부닥침
bool collp2w(PLAYER player, OBJECT object)
//----------------------------------------
{
	int adjust = 10 ;
	if (object.getType() == 1)	//땅바닥일때
	{
		if (object.getX() <= player.getx() && player.getx() <= object.getX() + object.getW())
		{
			if (object.getY() <= player.gety() + player.geth())
			{
				return 1;
			}
		}
	}
	else if(object.getType()<=100) {	//플랫폼일때
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
	if (player.getx() -player.getw()< 0)
	{
		player.setx(player.getw());
		player.setCMD_move(0);
	}
	else if (player.getx() + player.getw() > 1023)
	{
		player.setx(1023 - player.getw());
		player.setCMD_move(0);
	}
	for (int i = 0; i < ocount; ++i)
	{
		if (player.getstate() == 2)		//점프중일땐 적어도 바닥이랑은 충돌 없게한다
			break;
		if (player.getstate() == 3)		//어차피 7번 아니면 아래로 안떨어지니까는 ㄱㅊ 숙이기,점프중일때는 충돌처리 안해쥼
			break;
		if (collp2w(player, obj[i]))
		{
			player.sety(obj[i].getY() - player.geth());

			if (player.getstate() == 7) //떨어지는 중일때 부딪혔다 ?
			{
				if (obj[i].getType() <101)			//근데 그게 땅바닥이였다?
				{
					if(LEFTkey==0&&RIGHTkey==0)	//근데 그와중에도 아무키도 안누르고있었다 ? 
						player.setCMD_move(false);	//그럼 진행방향으로 가는걸 멈추도록해준다.
				}
			}
			player.setstate(1);
			if (ROWSPEED == 1)
				ROWSPEED = 3;
			return;			//하나라도 부딪혔다면 그대로 탈출한다
		}
	}
	
	if (player.getstate() == 4 || player.getstate() == 1)	//하나도 못부딪혔으면 공중에있는거니까 떨어져야한다
	{
		player.setstate(7);
		player.fall2save();		//떨어지는 순간의 x좌표점 기억
	}

}
//int(맵 번호) 에 따라 장애물 위치값 넣어주고 몇개의 오브젝트가 들어갔는지 알려주는 함수
int initObject(OBJECT* obj, int mapnum, HINSTANCE g_hinst)
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
			obj[i].setHbit(g_hinst);

		}
	}
	return objcount;
}

//카메라 무빙워크
void adjustCamera(CAMERA& camera,PLAYER player)
{
	//플레이어의 머리부분이 카메라의 꼭대기점을 넘어가면 바로 따라붙게한다
	if (player.gety()-player.geth() < camera.gety())
	{
		if (camera.gety() <= 0)	//최상점일땐 이동해주지않음
		{

		}
		else {
			camera.sety(player.gety() - player.geth());		//384는 맵 크기 768의 절반
		}
	}
	else if (player.gety()+player.geth() > camera.gety() + 768)	//캐릭터의 발바닥이 카메라밖을 넘어서면
	{
		if (camera.gety() >= 3232)	//최하점일땐 이동해주지않음
		{

		}
		else {
			camera.sety(player.gety()+player.geth() - 768);	//따라가준다
		}
	}else if (camera.gety()+600 != player.gety())		//카메라가 정해진 위치에 있지않다면
	{
		if (player.getstate() != 7)							//그리고 떨어질때까지 카메라를 바꿔주면 너무 흔들려서 이때는 무시함
		{
			if (camera.gety() + 540 > player.gety())		//얼마나 멀리있느냐에 따라 속도비를 다르게해서 카메라를 따라오게한다
			{
				camera.sety(camera.gety() - 4);
			}
			else if (camera.gety() + 580 > player.gety())
			{
				camera.sety(camera.gety() - 2);
			}
			else if (camera.gety() + 600 > player.gety())
			{
				camera.sety(camera.gety() - 1);
			}
			else if (camera.gety() + 700 < player.gety())
			{
				camera.sety(camera.gety() + 10);
			}
			else if (camera.gety() + 680 < player.gety())
			{
				camera.sety(camera.gety() + 8);
			}
			else if (camera.gety() + 660 < player.gety())
			{
				camera.sety(camera.gety() + 6);
			}
			else if (camera.gety() + 640 < player.gety())
			{
				camera.sety(camera.gety() + 4);
			}
			else if (camera.gety() + 620 < player.gety())
			{
				camera.sety(camera.gety() + 2);
			}
			else if (camera.gety() + 600 < player.gety())
			{
				camera.sety(camera.gety() + 1);
			}
		}
	}


}