#include "Global.h"
#include "world/obstacle/core/ObjectManager.h"
#include <fstream>


//오브젝트와 플레이어 충돌체크 1이면 부닥침
bool collp2w(PLAYER player, OBJECT object)
//----------------------------------------
{
	int adjust = 10;
	//왜 101이 먼저오냐면 발판보다는 장애물이 우선순위기때문임
	if (101 <= object.getType() && object.getType() < 301) { //장애물일때는 플레이어 네모빡스가 히트박스가된다
		if (object.getType() == 106 || object.getType() == 107)
		{
			if (player.GetPositionX() + player.GetHalfWidth() < object.getX() + object.getmx() || player.GetPositionX() - player.GetHalfWidth() > object.getX() + object.getmx() + object.getW()) return 0;
			if (player.GetPositionY() + player.GetHalfHeight() < object.getY() + object.getmy() || player.GetPositionY() - player.GetHalfHeight() > object.getY() + object.getmy() + object.getH()) return 0;
		}
		else
		{
			if (player.GetPositionX() + player.GetHalfWidth() < object.getX() || player.GetPositionX() - player.GetHalfWidth() > object.getX() + object.getW()) return 0;
			if (player.GetPositionY() + player.GetHalfHeight() < object.getY() || player.GetPositionY() - player.GetHalfHeight() > object.getY() + object.getH()) return 0;
		}


		return 1;
	}
	else if (301 <= object.getType() && object.getType() < 401)	//로프,밧줄같은 딱코 맞춰야하는 오브젝 위로는 플레이어 발까지 닿아야하고 아래로는 플레이어 중점에서 끝난다 하지만 내려갈수도 있어야하므로 조금 후하게 준다
	{

		if (player.GetPositionY() + player.GetHalfHeight() < object.getY() || player.GetPositionY() - player.GetHalfHeight() > object.getY() + object.getH()) return 0;	//일단먼저 닿았으면 들어와

		if (object.getX() < player.GetPositionX() && player.GetPositionX() < object.getX() + object.getW())	//파이프가 그래도 좀 두꺼우니 이안에들어오면 cehck
		{
			if (UPkey == true)//여기는 특이하게 올라가면 올라가는쪽 체크는 끝이나야한다.
			{

				if (player.GetPositionY() + player.GetHalfHeight() <= object.getY())	//올라갔을때 아랫키를 만족하면 충돌체크 x 안그러면 반응해서 계속 줄에매달리는 오류
					return 0;
				if (player.GetPositionY() < object.getY() + object.getH())
					return 1;
			}
			else if (DOWNkey == true)
			{

				if (player.GetPositionY() + player.GetHalfHeight() <= object.getY())
					return 1;
			}

			if (player.GetPositionY() + player.GetHalfHeight() <= object.getY() || player.GetPositionY() < object.getY() + object.getH())
				return 1;
		}

		return 0;
	}
	else if (object.getType() == 1)	//땅바닥일때
	{
		if (object.getX() <= player.GetPositionX() && player.GetPositionX() <= object.getX() + object.getW())
		{
			if (object.getY() <= player.GetPositionY() + player.GetHalfHeight())
			{
				return 1;
			}
		}
	}
	else if (object.getType() <= 100) {	//플랫폼일때는 플레이어 중점이 히트박스가된다
		if (object.getX() <= player.GetPositionX() && player.GetPositionX() <= object.getX() + object.getW())
		{
			if (object.getY() <= player.GetPositionY() + player.GetHalfHeight() && player.GetPositionY() + player.GetHalfHeight() <= object.getY() + adjust)
			{
				return 1;
				//ㅇㅇ
			}
		}
	}

	return 0;
}


//플레이어와 오브젝트간 상호작용 판단하고 그에맞게 바꿔줌
void adjustPlayer(PLAYER& player, OBJECT* obj, MAP& m, int& ocount, HINSTANCE g_hinst, Sound& sound)
{
	int check_coll = 0;	//하나라도 부딪혔는지 판별하기위함
	if (player.GetPositionX() - player.GetHalfWidth() < 0)
	{
		player.SetPositionX(player.GetHalfWidth());
		player.SetMoveCommand(EMoveCommand::None);
	}
	else if (player.GetPositionX() + player.GetHalfWidth() > 1023)
	{
		player.SetPositionX(1023 - player.GetHalfWidth());
		player.SetMoveCommand(EMoveCommand::None);
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
			if (obj[i].getType() < 101 && obj[i].getType() > 0)			//근데 그게 땅바닥이였다?
			{

				if (player.GetState() == EPlayerState::Airborne) //떨어지는 중일때 부딪혔다 ?
				{
					if (abs(player.GetSaveY() - player.GetPositionY()) > 200)	//낙뎀을받아야한다면
					{
						if (player.GetInvincibleTime() == 0)	//무적이 아니라면
						{
							player.SetMoveCommand(static_cast<EMoveCommand>(player.GetDirection()));	//보고있는방향으로 앞으로 나가게, 떨어졌는데 가만히있진 않지요
							player.SetState(EPlayerState::Hurt);		//피격으로감
							player.TakeDamage(sound);
							return;
						}
					}
					player.SetPositionY(obj[i].getY() - player.GetHalfHeight());//발판위로 y좌표 세팅해주고

					if (LEFTkey == 0 && RIGHTkey == 0)	//근데 그와중에도 아무키도 안누르고있었다 ? 
						player.SetMoveCommand(EMoveCommand::None);	//그럼 진행방향으로 가는걸 멈추도록해준다.
					else if (LEFTkey == 1 && RIGHTkey == 1)
						player.SetMoveCommand(EMoveCommand::None);	//동시에 누르고있었어도 멈춰준다
					else if (LEFTkey == 1)	//하지만 뭔가를 누르고있었다?
						player.SetMoveCommand(EMoveCommand::Left);
					else if (RIGHTkey == 1)			//그에맞춰바꿔준다
						player.SetMoveCommand(EMoveCommand::Right);

					if (DOWNkey == true) {
						player.SetState(EPlayerState::Crouch);	//숙이고있던 상태였다면 계속 숙이고있어줌
						player.SetPositionY(player.GetPositionY() + 12);
						player.SetHalfHeight(player.GetHalfHeight() - 12);	//계산이 끝났다면 다시 숙이기상태로 돌려줌
					}
					else player.SetState(EPlayerState::Idle);				//숙이던게 아니였으면 땅에부딪혔으니 정지상태해줌
					player.SetFallSpeed(0);			//떨어질때가속도를 위한거니 이것도 정지해줌
					player.SetHurt(0);			//땅에 닿았으면 피격아님
					player.SetRopeHurt(0);		//땅에 닿았으면 피격아님

					if (ROWSPEED != 3)		//ROWSPEED를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
						ROWSPEED = 3;
				}

				if (obj[i].getType() == 4)
				{
					player.SetPositionX(player.GetPositionX() + beltspeed);
				}
				if (obj[i].getType() == 6)
				{
					player.SetPositionX(player.GetPositionX() - beltspeed);
				}
			}
			else if (obj[i].getType() >= 101 && obj[i].getType() <= 200)	//장애물에 부딪히면
			{

				if (obj[i].getType() == 101)	//까시라면
				{
					if (player.GetInvincibleTime() == 0)	//무적이 아니라면
					{
						if (player.GetState() == EPlayerState::RopeIdle || player.GetState() == EPlayerState::RopeMove)
						{
							player.SetRopeHurt(1);
						}
						if (player.GetState() == EPlayerState::Crouch) //숙이고있었다면
						{
							player.SetPositionY(player.GetPositionY() - 12);
							player.SetHalfHeight(player.GetHalfHeight() + 12);	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
						}
						if (player.GetState() == EPlayerState::Airborne)//일반일때는 살짝 점프 뛰듯이 가는데 떨어지는중이면 살짝만 이동한다
						{
							if (player.GetMoveCommand() == EMoveCommand::Left)
							{
								player.SetSpikeHurt(-8);	//8번 왼쪽으로 감
							}
							else if (player.GetMoveCommand() == EMoveCommand::Right)
							{
								player.SetSpikeHurt(8);	//8번 오른쪽으로감
							}

							player.SetInvicible(100);	//무적시간 넣어줌 (이동하는로직은 state==7 일때 알아서 다뤄줌
						}
						else {
							player.SetState(EPlayerState::Hurt);		//피격으로감
						}
						player.TakeDamage(sound);
					}
				}
				else if (obj[i].getType() == 102) //Break Pipe Left
				{
					//Copy and Paste is very good (Y Collapse)
					if (player.GetState() == EPlayerState::Airborne) //떨어지는 중일때 부딪혔다 ?
					{
						player.SetPositionY(obj[i].getY() - player.GetHalfHeight());//발판위로 y좌표 세팅해주고

						if (LEFTkey == 0 && RIGHTkey == 0)	//근데 그와중에도 아무키도 안누르고있었다 ? 
							player.SetMoveCommand(EMoveCommand::None);	//그럼 진행방향으로 가는걸 멈추도록해준다.
						else if (LEFTkey == 1 && RIGHTkey == 1)
							player.SetMoveCommand(EMoveCommand::None);	//동시에 누르고있었어도 멈춰준다
						else if (LEFTkey == 1)	//하지만 뭔가를 누르고있었다?
							player.SetMoveCommand(EMoveCommand::Left);
						else if (RIGHTkey == 1)			//그에맞춰바꿔준다
							player.SetMoveCommand(EMoveCommand::Right);

						player.SetState(EPlayerState::Idle);				//그리고 땅에부딪혔으니 정지상태해줌
						player.SetFallSpeed(0);			//떨어질때가속도를 위한거니 이것도 정지해줌
						player.SetHurt(0);			//땅에 닿았으면 피격아님
						player.SetRopeHurt(0);		//땅에 닿았으면 피격아님

						if (ROWSPEED != 3)		//ROWSPEED를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
							ROWSPEED = 3;
					}
					//X Collapse
					if (player.GetState() == EPlayerState::Idle || player.GetState() == EPlayerState::Move) //Walking Collpse
					{
						if (obj[i].getY() < player.GetPositionY() - player.GetHalfHeight())
						{
							if (obj[i].getX() < player.GetPositionX() + player.GetHalfWidth()) //Left Collpse
							{
								player.SetPositionX(obj[i].getX() - player.GetHalfWidth());// x좌표 세팅해주고
								player.SetMoveCommand(EMoveCommand::None);
							}
						}
					}
				}
				else if (obj[i].getType() == 103) //왼쪽 증기, 가시와 비슷함 대신 증기가 완전히 뿜어져  나왔을때 피격판정이 있다.
				{
					if (obj[i].getindex() == 2) //증기가 완전히 뿜어졌을때만 피격이 발생한다
					{
						if (player.GetInvincibleTime() == 0)
						{
							if (player.GetState() == EPlayerState::RopeIdle || player.GetState() == EPlayerState::RopeMove)
							{
								player.SetRopeHurt(1);
							}
							if (player.GetState() == EPlayerState::Crouch) //숙이고있었다면
							{
								player.SetPositionY(player.GetPositionY() - 12);
								player.SetHalfHeight(player.GetHalfHeight() + 12);	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
							}
							if (player.GetState() == EPlayerState::Airborne)
							{
								if (player.GetDirection() == EPlayerDirection::Left || player.GetDirection() == EPlayerDirection::Right) //무조건 왼쪽으로감
								{
									player.SetSpikeHurt(-8);
								}

								player.SetInvicible(100);
							}
							else {
								player.SetMoveCommand(EMoveCommand::Left); //무조건 왼쪽임
								player.SetState(EPlayerState::Hurt);
							}
							player.TakeDamage(sound);
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
					if (player.GetInvincibleTime() == 0)	//무적이 아니라면
					{
						if (player.GetState() == EPlayerState::RopeIdle || player.GetState() == EPlayerState::RopeMove)
						{
							player.SetRopeHurt(1);
						}
						if (player.GetState() == EPlayerState::Crouch) //숙이고있었다면
						{
							player.SetPositionY(player.GetPositionY() - 12);
							player.SetHalfHeight(player.GetHalfHeight() + 12);	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
						}
						if (player.GetState() == EPlayerState::Airborne)//일반일때는 살짝 점프 뛰듯이 가는데 떨어지는중이면 살짝만 이동한다
						{
							if (player.GetDirection() == EPlayerDirection::Left)
							{
								player.SetSpikeHurt(-8);	//8번 왼쪽으로 감
							}
							else if (player.GetDirection() == EPlayerDirection::Right)
							{
								player.SetSpikeHurt(8);	//8번 오른쪽으로감
							}

							player.SetInvicible(100);	//무적시간 넣어줌 (이동하는로직은 state==7 일때 알아서 다뤄줌
						}
						else {
							player.SetMoveCommand(static_cast<EMoveCommand>(player.GetDirection()));
							player.SetState(EPlayerState::Hurt);		//피격으로감
						}
						player.TakeDamage(sound);
					}
				}
				else if (obj[i].getType() == 107)
				{
					if (player.GetInvincibleTime() == 0)	//무적이 아니라면
					{
						if (player.GetState() == EPlayerState::RopeIdle || player.GetState() == EPlayerState::RopeMove)
						{
							player.SetRopeHurt(1);
						}
						if (player.GetState() == EPlayerState::Crouch) //숙이고있었다면
						{
							player.SetPositionY(player.GetPositionY() - 12);
							player.SetHalfHeight(player.GetHalfHeight() + 12);	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
						}
						if (player.GetState() == EPlayerState::Airborne)//일반일때는 살짝 점프 뛰듯이 가는데 떨어지는중이면 살짝만 이동한다
						{
							if (player.GetDirection() == EPlayerDirection::Left)
							{
								player.SetSpikeHurt(-8);	//8번 왼쪽으로 감
							}
							else if (player.GetDirection() == EPlayerDirection::Right)
							{
								player.SetSpikeHurt(8);	//8번 오른쪽으로감
							}

							player.SetInvicible(100);	//무적시간 넣어줌 (이동하는로직은 state==7 일때 알아서 다뤄줌
						}
						else {
							player.SetMoveCommand(static_cast<EMoveCommand>(player.GetDirection()));
							player.SetState(EPlayerState::Hurt);		//피격으로감
						}
						player.TakeDamage(sound);
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
						player.Initialzie();
						if (m.getmapnum() == 13) m.CreateMap(g_hinst);
						for (int j = 0; j < ocount; j++)
							obj[j].ResetObject();
						ocount = initObject(obj, m.getmapnum(), g_hinst);
						m.CreateMap(g_hinst);
						sound.setindex(m.getmapnum() - 9);
						sound.System->update();
						sound.Channel[0]->stop();
						sound.System->playSound(
							sound.bgmSound[sound.getindex()],
							nullptr,
							false,
							&sound.Channel[0]
						);
						sound.Channel[1]->stop();
						sound.System->playSound(
							sound.effectSound[1],
							nullptr,
							false,
							&sound.Channel[1]
						);


						return;
					}
				}
			}
			else if (obj[i].getType() == 301) //Rope
			{
				if (player.GetJumpCooldown() <= 0)
				{
					if (player.IsRopeHurt() == 0)	//로프에서 맞으면 다시 로프 못탐
					{
						if (UPkey == true || DOWNkey == true)
						{
							if (DOWNkey == true && (player.GetState() == EPlayerState::Jump || player.GetState() == EPlayerState::Airborne))	//공중에있거나 점프중일때 아랫키로는 줄에 붙을수없다
								return;

							if (player.GetState() != EPlayerState::RopeIdle && player.GetState() != EPlayerState::RopeMove)	//줄에 매달려있지 않았다면 줄에 매달리는 상태를 만들어준다. 이미붙어있다면 해줄필요없음
							{
								player.SetState(EPlayerState::RopeIdle);
								if (UPkey == true)
									player.SetMoveCommand(EMoveCommand::Up);
								if (DOWNkey == true)
									player.SetMoveCommand(EMoveCommand::Down);
								player.SetPositionX(obj[i].getX() + (obj[i].getW() / 2));
								if (DOWNkey == true)	//이때는 수그리기아니라 밧줄 아래로 내려가는것이므로 수그리기로 깍인거 돌려준다
								{
									player.SetPositionY(player.GetPositionY() - 12);
									player.SetHalfHeight(25);
								}
							}
							player.PlayAnim();
							//player.BitMove();
						}
					}
				}
			}
			else if (obj[i].getType() == 0)
			{

			}
			//if (ROWSPEED != 3)		//ROWSPEED를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
			//	ROWSPEED = 3; 잠깐 위로 올려줬음 주석처리하고 ㅇㅇ 근데 이게 맞을거같긴해

			//return;			//하나라도 부딪혔다면 그대로 탈출한다
		}
	}
	if (check_coll != 0)
		return;	//하나라도 부딪혔다면 그대로 탈출
	if ((player.GetState() == EPlayerState::Move || player.GetState() == EPlayerState::Idle) 
		|| (player.GetState() == EPlayerState::RopeIdle || player.GetState() == EPlayerState::RopeMove))	//하나도 못부딪혔으면 공중에있는거니까 떨어져야한다
	{
		player.SetState(EPlayerState::Airborne);
		player.SavePositionX();		//떨어지는 순간의 x좌표점 기억
	}


}


//int(맵 번호) 에 따라 장애물 위치값 넣어주고 몇개의 오브젝트가 들어갔는지 알려주는 함수
int initObject(OBJECT* obj, int mapnum, HINSTANCE g_hinst)
{
	int x, y, w, h, type;
	int objcount = 0;

	ifstream in;
	if (mapnum == 9)
	{
		in.open("map/map_0.txt", ios::in);
	}
	else if (mapnum == 10)
	{
		in.open("map/map_1.txt", ios::in);
	}
	else if (mapnum == 11)
	{
		in.open("map/map_2.txt", ios::in);
	}
	else if (mapnum == 12)
	{
		in.open("map/map_3.txt", ios::in);
	}
	else if (mapnum == 13)
	{
		in.open("map/map_4.txt", ios::in);
	}
	else {
		return 0;		//맵 값이 잘못입력되었으면 그대로 탈출
	}




	if (in.is_open())
	{
		for (int i = 0; i < 150; ++i)
		{
			if (in.eof())
			{
				objcount = i;
				break;
			}
			in >> x >> y >> w >> h >> type;
			obj[i].create(x, y, w, h, type);
			obj[i].setHbit(g_hinst);

		}
	}
	return objcount;
}

//카메라 무빙워크
void adjustCamera(CAMERA& camera, PLAYER player)
{
	//플레이어의 머리부분이 카메라의 꼭대기점을 넘어가면 바로 따라붙게한다
	if (player.GetPositionY() - player.GetHalfHeight() < camera.GetY())
	{
		if (camera.GetY() <= 0)	//최상점일땐 이동해주지않음
		{

		}
		else {
			camera.SetY(player.GetPositionY() - player.GetHalfHeight());		//384는 맵 크기 768의 절반
		}
	}
	else if (player.GetPositionY() + player.GetHalfHeight() > camera.GetY() + 768)	//캐릭터의 발바닥이 카메라밖을 넘어서면
	{
		if (camera.GetY() >= 3232)	//최하점일땐 이동해주지않음
		{

		}
		else {
			camera.SetY(player.GetPositionY() + player.GetHalfHeight() - 768);	//따라가준다
		}
	}
	else if (camera.GetY() + 600 != player.GetPositionY())		//카메라가 정해진 위치에 있지않다면
	{
		if (player.GetState() != EPlayerState::Airborne)							//그리고 떨어질때까지 카메라를 바꿔주면 너무 흔들려서 이때는 무시함
		{
			if (camera.GetY() <= 0)	//최상점일땐 이동해주지않음
			{
				camera.SetY(0);
			}
			else if (camera.GetY() + 540 > player.GetPositionY())		//얼마나 멀리있느냐에 따라 속도비를 다르게해서 카메라를 따라오게한다
			{
				camera.SetY(camera.GetY() - 4);
			}
			else if (camera.GetY() + 580 > player.GetPositionY())
			{
				camera.SetY(camera.GetY() - 2);
			}
			else if (camera.GetY() + 600 > player.GetPositionY())
			{
				camera.SetY(camera.GetY() - 1);
			}
			else if (camera.GetY() + 700 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 10);
			}
			else if (camera.GetY() + 680 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 8);
			}
			else if (camera.GetY() + 660 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 6);
			}
			else if (camera.GetY() + 640 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 4);
			}
			else if (camera.GetY() + 620 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 2);
			}
			else if (camera.GetY() + 600 < player.GetPositionY())
			{
				camera.SetY(camera.GetY() + 1);
			}
		}
	}


}