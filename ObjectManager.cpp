#include "ObjectManager.h"


//땅바닥과 플레이어 충돌체크 1이면 부닥침
bool collp2w(PLAYER player, OBJECT object)
//----------------------------------------
{
	int adjust = 5 ;
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