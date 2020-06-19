#include "ObjectManager.h"


//오브젝트와 플레이어 충돌체크 1이면 부닥침
bool collp2w(PLAYER player, OBJECT object)
//----------------------------------------
{
	int adjust = 10;
	//왜 101이 먼저오냐면 발판보다는 장애물이 우선순위기때문임
	if (object.getType() >= 101) { //장애물일때는 플레이어 네모빡스가 히트박스가된다
		if (object.getType() == 106 || object.getType() == 107)
		{
			if (player.getx() + player.getw() < object.getX()+object.getmx() || player.getx() - player.getw() > object.getX() + object.getmx() + object.getW()) return 0;
			if (player.gety() + player.geth() < object.getY() + object.getmy() || player.gety() - player.geth() > object.getY() + object.getmy() + object.getH()) return 0;
		}
		else
		{
			if (player.getx() + player.getw() < object.getX() || player.getx() - player.getw() > object.getX() + object.getW()) return 0;
			if (player.gety() + player.geth() < object.getY() || player.gety() - player.geth() > object.getY() + object.getH()) return 0;
		}
		

		return 1;
	}
	else if (object.getType() == 1)	//땅바닥일때
	{
		if (object.getX() <= player.getx() && player.getx() <= object.getX() + object.getW())
		{
			if (object.getY() <= player.gety() + player.geth())
			{
				return 1;
			}
		}
	}
	else if (object.getType() <= 100) {	//플랫폼일때는 플레이어 중점이 히트박스가된다
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
void adjustPlayer(PLAYER& player, OBJECT* obj, MAP& m, int ocount, HINSTANCE g_hinst, Sound sound)
{
	int check_coll = 0;	//하나라도 부딪혔는지 판별하기위함
	if (player.getx() - player.getw() < 0)
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
		//if (player.getstate() == 2)		//점프중일땐 적어도 바닥이랑은 충돌 없게한다
		//	break;
		//if (player.getstate() == 3)		//어차피 7번 아니면 아래로 안떨어지니까는 ㄱㅊ 숙이기,점프중일때는 충돌처리 안해쥼
		//	break;
		if (collp2w(player, obj[i]))
		{
			check_coll++;	//하나라도 부딪혔으면 coll이 올라감
			if (obj[i].getType() < 101)			//근데 그게 땅바닥이였다?
			{

				if (player.getstate() == 7) //떨어지는 중일때 부딪혔다 ?
				{
					if (abs(player.getsavey() - player.gety()) > 200)	//낙뎀을받아야한다면
					{
						if (player.getstealth() == 0)	//무적이 아니라면
						{
							player.setCMD_move(player.getdir());	//보고있는방향으로 앞으로 나가게, 떨어졌는데 가만히있진 않지요
							player.setstate(6);		//피격으로감
							player.hurt();
							return;
						}
					}
					player.sety(obj[i].getY() - player.geth());//발판위로 y좌표 세팅해주고

					if (LEFTkey == 0 && RIGHTkey == 0)	//근데 그와중에도 아무키도 안누르고있었다 ? 
						player.setCMD_move(false);	//그럼 진행방향으로 가는걸 멈추도록해준다.
					else if (LEFTkey == 1 && RIGHTkey == 1)
						player.setCMD_move(false);	//동시에 누르고있었어도 멈춰준다
					else if (LEFTkey == 1)	//하지만 뭔가를 누르고있었다?
						player.setCMD_move(1);
					else if (RIGHTkey == 1)			//그에맞춰바꿔준다
						player.setCMD_move(2);

					if (DOWNkey == true) {
						player.setstate(3);	//숙이고있던 상태였다면 계속 숙이고있어줌
						player.sety(player.gety() + 12);
						player.seth(player.geth() - 12);	//계산이 끝났다면 다시 숙이기상태로 돌려줌
					}
					else player.setstate(1);				//숙이던게 아니였으면 땅에부딪혔으니 정지상태해줌
					player.setadjspd(0);			//떨어질때가속도를 위한거니 이것도 정지해줌
					player.setCMD_hurt(0);			//땅에 닿았으면 피격아님

					if (ROWSPEED != 3)		//ROWSPEED를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
						ROWSPEED = 3;
				}

				if (obj[i].getType() == 4)
				{
					player.setx(player.getx() + beltspeed);
				}
			}
			else if (obj[i].getType() >= 101 && obj[i].getType() <= 200)	//장애물에 부딪히면
			{

				if (obj[i].getType() == 101)	//까시라면
				{
					if (player.getstealth() == 0)	//무적이 아니라면
					{
						if (player.getstate()==3) //숙이고있었다면
						{
							player.sety(player.gety() - 12);
							player.seth(player.geth() + 12);	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
						}
						if (player.getstate() == 7)//일반일때는 살짝 점프 뛰듯이 가는데 떨어지는중이면 살짝만 이동한다
						{
							if (player.getCMD_move() == 1)
							{
								player.setspike_hurt(-8);	//8번 왼쪽으로 감
							}
							else if (player.getCMD_move() == 2)
							{
								player.setspike_hurt(8);	//8번 오른쪽으로감
							}

							player.setstealth(100);	//무적시간 넣어줌 (이동하는로직은 state==7 일때 알아서 다뤄줌
						}
						else {
							player.setstate(6);		//피격으로감
						}
						player.hurt();
					}
				}
				else if (obj[i].getType() == 102) //Break Pipe Left
				{
					//Copy and Paste is very good (Y Collapse)
					if (player.getstate() == 7) //떨어지는 중일때 부딪혔다 ?
					{
						player.sety(obj[i].getY() - player.geth());//발판위로 y좌표 세팅해주고

						if (LEFTkey == 0 && RIGHTkey == 0)	//근데 그와중에도 아무키도 안누르고있었다 ? 
							player.setCMD_move(false);	//그럼 진행방향으로 가는걸 멈추도록해준다.
						else if (LEFTkey == 1 && RIGHTkey == 1)
							player.setCMD_move(false);	//동시에 누르고있었어도 멈춰준다
						else if (LEFTkey == 1)	//하지만 뭔가를 누르고있었다?
							player.setCMD_move(1);
						else if (RIGHTkey == 1)			//그에맞춰바꿔준다
							player.setCMD_move(2);

						player.setstate(1);				//그리고 땅에부딪혔으니 정지상태해줌
						player.setadjspd(0);			//떨어질때가속도를 위한거니 이것도 정지해줌
						player.setCMD_hurt(0);			//땅에 닿았으면 피격아님

						if (ROWSPEED != 3)		//ROWSPEED를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
							ROWSPEED = 3;
					}
					//X Collapse
					if (player.getstate() == 1 || player.getstate() == 4) //Walking Collpse
					{
						if (obj[i].getY() < player.gety() - player.geth())
						{
							if (obj[i].getX() < player.getx() + player.getw()) //Left Collpse
							{
								player.setx(obj[i].getX() - player.getw());// x좌표 세팅해주고
								player.setCMD_move(0);//Don't Move
							}
						}
					}
				}
				else if (obj[i].getType() == 103) //왼쪽 증기, 가시와 비슷함 대신 증기가 완전히 뿜어져  나왔을때 피격판정이 있다.
				{
					if (obj[i].getindex() == 2) //증기가 완전히 뿜어졌을때만 피격이 발생한다
					{
						if (player.getstealth() == 0)
						{
							if (player.getstate() == 3) //숙이고있었다면
							{
								player.sety(player.gety() - 12);
								player.seth(player.geth() + 12);	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
							}
							if (player.getstate() == 7)
							{
								if (player.getCMD_move() == 1 || player.getCMD_move() == 2) //무조건 왼쪽으로감
								{
									player.setspike_hurt(-8);
								}

								player.setstealth(100);
							}
							else {
								player.setCMD_move(1); //무조건 왼쪽임
								player.setstate(6);
							}
							player.hurt();
						}
					}
				}
				else if (obj[i].getType() == 104) //Break Pipe Right
				{
					//Update Cooming Soon
				}
				else if (obj[i].getType() == 105) //Gas Right
				{
					//Update Cooming Soon
				}
				else if (obj[i].getType() == 106)
				{
					if (player.getstealth() == 0)	//무적이 아니라면
					{
						if (player.getstate() == 3) //숙이고있었다면
						{
							player.sety(player.gety() - 12);
							player.seth(player.geth() + 12);	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
						}
						if (player.getstate() == 7)//일반일때는 살짝 점프 뛰듯이 가는데 떨어지는중이면 살짝만 이동한다
						{
							if (player.getCMD_move() == 1)
							{
								player.setspike_hurt(-8);	//8번 왼쪽으로 감
							}
							else if (player.getCMD_move() == 2)
							{
								player.setspike_hurt(8);	//8번 오른쪽으로감
							}

							player.setstealth(100);	//무적시간 넣어줌 (이동하는로직은 state==7 일때 알아서 다뤄줌
						}
						else {
							player.setstate(6);		//피격으로감
						}
						player.hurt();
					}
				}
				else if (obj[i].getType() == 107)
				{
					if (player.getstealth() == 0)	//무적이 아니라면
					{
						if (player.getstate() == 3) //숙이고있었다면
						{
							player.sety(player.gety() - 12);
							player.seth(player.geth() + 12);	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
						}
						if (player.getstate() == 7)//일반일때는 살짝 점프 뛰듯이 가는데 떨어지는중이면 살짝만 이동한다
						{
							if (player.getCMD_move() == 1)
							{
								player.setspike_hurt(-8);	//8번 왼쪽으로 감
							}
							else if (player.getCMD_move() == 2)
							{
								player.setspike_hurt(8);	//8번 오른쪽으로감
							}

							player.setstealth(100);	//무적시간 넣어줌 (이동하는로직은 state==7 일때 알아서 다뤄줌
						}
						else {
							player.setstate(6);		//피격으로감
						}
						player.hurt();
					}
				}
			}
			else if (obj[i].getType() >= 201 && obj[i].getType() <= 300) //플레이어와 상호작용하는 오브젝트 ex)포탈
			{
			if (obj[i].getType() == 201) //Portal
			{
				if (UPkey == true)
				{
					m.setblack_t(50);
					/*m.CreateBlack(g_hinst);*/
					m.setmapnum(m.getmapnum() + 1);
					player.initPos();
					for (int j = 0; j < ocount; j++)
						obj[j].ResetObject();
					initObject(obj, m.getmapnum(), g_hinst);
					FMOD_Channel_Stop(sound.Channel[1]);
					FMOD_System_PlaySound(sound.System, sound.effectSound[1], NULL, 0, &sound.Channel[1]);
					FMOD_Channel_Stop(sound.Channel[0]);
					FMOD_System_PlaySound(sound.System, sound.bgmSound[1], NULL, 0, &sound.Channel[0]);
					return;
				}
			}
			else if (obj[i].getType() == 202) //Rope
			{
				if (UPkey == true)
				{
					player.setstate(5);
					//player.BitMove();
				}
					
				
			}
			}
			//if (ROWSPEED != 3)		//ROWSPEED를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
			//	ROWSPEED = 3; 잠깐 위로 올려줬음 주석처리하고 ㅇㅇ 근데 이게 맞을거같긴해

			//return;			//하나라도 부딪혔다면 그대로 탈출한다
		}
	}
	if (check_coll != 0)
		return;	//하나라도 부딪혔다면 그대로 탈출
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
			obj[i].create(x, y, w, h, type);
			/*obj[i].setX(x);
			obj[i].setY(y);
			obj[i].setW(w);
			obj[i].setH(h);
			obj[i].setType(type);*/
			obj[i].setHbit(g_hinst);

		}
	}
	return objcount;
}

//카메라 무빙워크
void adjustCamera(CAMERA& camera, PLAYER player)
{
	//플레이어의 머리부분이 카메라의 꼭대기점을 넘어가면 바로 따라붙게한다
	if (player.gety() - player.geth() < camera.gety())
	{
		if (camera.gety() <= 0)	//최상점일땐 이동해주지않음
		{

		}
		else {
			camera.sety(player.gety() - player.geth());		//384는 맵 크기 768의 절반
		}
	}
	else if (player.gety() + player.geth() > camera.gety() + 768)	//캐릭터의 발바닥이 카메라밖을 넘어서면
	{
		if (camera.gety() >= 3232)	//최하점일땐 이동해주지않음
		{

		}
		else {
			camera.sety(player.gety() + player.geth() - 768);	//따라가준다
		}
	}
	else if (camera.gety() + 600 != player.gety())		//카메라가 정해진 위치에 있지않다면
	{
		if (player.getstate() != 7)							//그리고 떨어질때까지 카메라를 바꿔주면 너무 흔들려서 이때는 무시함
		{
			if (camera.gety() <= 0)	//최상점일땐 이동해주지않음
			{
				camera.sety(0);
			}
			else if (camera.gety() + 540 > player.gety())		//얼마나 멀리있느냐에 따라 속도비를 다르게해서 카메라를 따라오게한다
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