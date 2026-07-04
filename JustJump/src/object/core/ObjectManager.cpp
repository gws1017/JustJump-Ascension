#include "Global.h"
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>

#include "ObjectManager.h"
#include "core/LevelConfig.h"
#include "object/character/player.h"
#include "world/obstacle/obstacle.h"
#include "world/obstacle/GroundObstacle.h"
#include "world/obstacle/AnimatedBgObstacle.h"
#include "world/obstacle/PlatformObstacle.h"
#include "world/obstacle/SmallPlatObstacle.h"
#include "world/obstacle/BeltObstacle.h"
#include "world/obstacle/TransparentObstacle.h"
#include "world/obstacle/LongPlatObstacle.h"
#include "world/obstacle/NailObstacle.h"
#include "world/obstacle/BrokenPipeObstacle.h"
#include "world/obstacle/GasObstacle.h"
#include "world/obstacle/GearObstacle.h"
#include "world/obstacle/PortalObstacle.h"
#include "world/obstacle/RopeObstacle.h"
#include "object/view/Camera.h"
#include "core/InputManager.h"
#include "world/Map.h"


//오브젝트와 플레이어 충돌체크 1이면 부닥침
bool ObjectManager::CollP2W(const UPtr<PLAYER>& player, const SPtr<Obstacle>& obstacle)
//----------------------------------------
{
	const int adjust = GameConst::kPlatformCollisionAdjust;
	switch (obstacle->GetHitboxKind())
	{
	case EHitboxKind::Box:	//장애물일때는 플레이어 네모박스가 히트박스가된다 (mx,my는 GearObstacle 아니면 항상 0)
	{
		if (player->GetX() + player->GetWidth() < obstacle->GetX() + obstacle->GetMX() || player->GetX() - player->GetWidth() > obstacle->GetX() + obstacle->GetMX() + obstacle->GetWidth()) return 0;
		if (player->GetY() + player->GetHeight() < obstacle->GetY() + obstacle->GetMY() || player->GetY() - player->GetHeight() > obstacle->GetY() + obstacle->GetMY() + obstacle->GetHeight()) return 0;
		return 1;
	}
	case EHitboxKind::Rope:	//로프,밧줄같은 딱코 맞춰야하는 오브젝 위로는 플레이어 발까지 닿아야하고 아래로는 플레이어 중점에서 끝난다 하지만 내려갈수도 있어야하므로 조금 후하게 준다
	{
		if (player->GetY() + player->GetHeight() < obstacle->GetY() || player->GetY() - player->GetHeight() > obstacle->GetY() + obstacle->GetHeight()) return 0;	//일단먼저 닿았으면 들어와

		if (obstacle->GetX() < player->GetX() && player->GetX() < obstacle->GetX() + obstacle->GetWidth())	//파이프가 그래도 좀 두꺼우니 이안에들어오면 cehck
		{
			if (InputHelper::IsUpDown())//여기는 특이하게 올라가면 올라가는쪽 체크는 끝이나야한다.
			{
				if (player->GetY() + player->GetHeight() <= obstacle->GetY())	//올라갔을때 아랫키를 만족하면 충돌체크 x 안그러면 반응해서 계속 줄에매달리는 오류
					return 0;
				if (player->GetY() < obstacle->GetY() + obstacle->GetHeight())
					return 1;
			}
			else if (InputHelper::IsDownDown())
			{
				if (player->GetY() + player->GetHeight() <= obstacle->GetY())
					return 1;
			}

			if (player->GetY() + player->GetHeight() <= obstacle->GetY() || player->GetY() < obstacle->GetY() + obstacle->GetHeight())
				return 1;
		}
		return 0;
	}
	case EHitboxKind::Ground:	//땅바닥일때
	{
		if (obstacle->GetX() <= player->GetX() && player->GetX() <= obstacle->GetX() + obstacle->GetWidth())
		{
			if (obstacle->GetY() <= player->GetY() + player->GetHeight())
				return 1;
		}
		return 0;
	}
	case EHitboxKind::Platform:	//플랫폼일때는 플레이어 중점이 히트박스가된다
	{
		if (obstacle->GetX() <= player->GetX() && player->GetX() <= obstacle->GetX() + obstacle->GetWidth())
		{
			if (obstacle->GetY() <= player->GetY() + player->GetHeight() && player->GetY() + player->GetHeight() <= obstacle->GetY() + adjust)
				return 1;
		}
		return 0;
	}
	default:	//None: 충돌 자체를 안 함 (장식용 배경 등)
		return 0;
	}
}


//플레이어와 오브젝트간 상호작용 판단하고 그에맞게 바꿔줌
void ObjectManager::AdjustPlayer(const UPtr<PLAYER>& player, MAP& m, int& ocount, HINSTANCE g_hinst)
{
	
	int check_coll = 0;	//하나라도 부딪혔는지 판별하기위함
	if (player->GetX() - player->GetWidth() < 0)
	{
		player->SetX(player->GetWidth());
		player->SetMoveCommand(EMoveCommand::None);
	}
	else if (player->GetX() + player->GetWidth() > GameConst::kWorldMaxX)
	{
		player->SetX(GameConst::kWorldMaxX - player->GetWidth());
		player->SetMoveCommand(EMoveCommand::None);
	}
	for (const auto& obstacle : m_obstacles)
	{
		//if (player->getstate() == 2)		//점프중일땐 적어도 바닥이랑은 충돌 없게한다
		//	break;
		//if (player->getstate() == 3)		//어차피 7번 아니면 아래로 안떨어지니까는 ㄱㅊ 숙이기,점프중일때는 충돌처리 안해쥼
		//	break;
		if (CollP2W(player, obstacle))
		{
			check_coll++;	//하나라도 부딪혔으면 coll이 올라감
			if (ObstacleTypeUtil::IsPlatform(obstacle->GetType()))			//근데 그게 땅바닥이였다?
			{

				if (player->GetState() == EPlayerState::Airborne) //떨어지는 중일때 부딪혔다 ?
				{
					if (abs(player->GetSaveY() - player->GetY()) > GameConst::kFallDamageThreshold)	//낙뎀을받아야한다면
					{
						if (player->GetInvincibleTime() == 0)	//무적이 아니라면
						{
							player->SetMoveCommand(static_cast<EMoveCommand>(player->GetDirection()));	//보고있는방향으로 앞으로 나가게, 떨어졌는데 가만히있진 않지요
							player->SetState(EPlayerState::Hurt);		//피격으로감
							player->TakeDamage();
							return;
						}
					}
					player->SetY(obstacle->GetY() - player->GetHeight());//발판위로 y좌표 세팅해주고

					if (!InputHelper::IsLeftDown() && !InputHelper::IsRightDown())	//근데 그와중에도 아무키도 안누르고있었다 ? 
						player->SetMoveCommand(EMoveCommand::None);	//그럼 진행방향으로 가는걸 멈추도록해준다.
					else if (InputHelper::IsLeftDown() && InputHelper::IsRightDown())
						player->SetMoveCommand(EMoveCommand::None);	//동시에 누르고있었어도 멈춰준다
					else if (InputHelper::IsLeftDown())	//하지만 뭔가를 누르고있었다?
						player->SetMoveCommand(EMoveCommand::Left);
					else if (InputHelper::IsRightDown())			//그에맞춰바꿔준다
						player->SetMoveCommand(EMoveCommand::Right);

					if (InputHelper::IsDownDown()) {
						player->SetState(EPlayerState::Crouch);	//숙이고있던 상태였다면 계속 숙이고있어줌
						player->SetY(player->GetY() + GameConst::kCrouchHeightDelta);
						player->SetHeight(player->GetHeight() - GameConst::kCrouchHeightDelta);	//계산이 끝났다면 다시 숙이기상태로 돌려줌
					}
					else player->SetState(EPlayerState::Idle);				//숙이던게 아니였으면 땅에부딪혔으니 정지상태해줌
					player->SetFallSpeed(0);			//떨어질때가속도를 위한거니 이것도 정지해줌
					player->SetHurt(0);			//땅에 닿았으면 피격아님
					player->SetRopeHurt(0);		//땅에 닿았으면 피격아님

					if (player->GetRowSpeed() != 3)		//m_rowSpeed를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
						player->SetRowSpeed(3);
				}

				if (obstacle->GetType() == EObstacleType::BeltRight)
					player->SetX(player->GetX() + ObstacleConst::kBeltSpeed);
				if (obstacle->GetType() == EObstacleType::BeltLeft)
					player->SetX(player->GetX() - ObstacleConst::kBeltSpeed);
			}
			else if (ObstacleTypeUtil::IsHazard(obstacle->GetType()))	//장애물에 부딪히면
			{

				if (obstacle->GetType() == EObstacleType::Nail)	//까시라면
				{
					if (player->GetInvincibleTime() == 0)	//무적이 아니라면
					{
						if (player->GetState() == EPlayerState::RopeIdle || player->GetState() == EPlayerState::RopeMove)
						{
							player->SetRopeHurt(1);
						}
						if (player->GetState() == EPlayerState::Crouch) //숙이고있었다면
						{
							player->SetY(player->GetY() - GameConst::kCrouchHeightDelta);
							player->SetHeight(player->GetHeight() + GameConst::kCrouchHeightDelta);	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
						}
						if (player->GetState() == EPlayerState::Airborne)//일반일때는 살짝 점프 뛰듯이 가는데 떨어지는중이면 살짝만 이동한다
						{
							if (player->GetMoveCommand() == EMoveCommand::Left)
							{
								player->SetSpikeHurt(-8);	//8번 왼쪽으로 감
							}
							else if (player->GetMoveCommand() == EMoveCommand::Right)
							{
								player->SetSpikeHurt(8);	//8번 오른쪽으로감
							}

							player->SetInvicible(GameConst::kInvincibleFrames);	//무적시간 넣어줌 (이동하는로직은 state==7 일때 알아서 다뤄줌
						}
						else {
							player->SetState(EPlayerState::Hurt);		//피격으로감
						}
						player->TakeDamage();
					}
				}
				else if (obstacle->GetType() == EObstacleType::BrokenPipe) //Break Pipe Left
				{
					//Copy and Paste is very good (Y Collapse)
					if (player->GetState() == EPlayerState::Airborne) //떨어지는 중일때 부딪혔다 ?
					{
						player->SetY(obstacle->GetY() - player->GetHeight());//발판위로 y좌표 세팅해주고

						if (!InputHelper::IsLeftDown() && !InputHelper::IsRightDown())	//근데 그와중에도 아무키도 안누르고있었다 ? 
							player->SetMoveCommand(EMoveCommand::None);	//그럼 진행방향으로 가는걸 멈추도록해준다.
						else if (InputHelper::IsLeftDown() && InputHelper::IsRightDown())
							player->SetMoveCommand(EMoveCommand::None);	//동시에 누르고있었어도 멈춰준다
						else if (InputHelper::IsLeftDown())	//하지만 뭔가를 누르고있었다?
							player->SetMoveCommand(EMoveCommand::Left);
						else if (InputHelper::IsRightDown())			//그에맞춰바꿔준다
							player->SetMoveCommand(EMoveCommand::Right);

						player->SetState(EPlayerState::Idle);				//그리고 땅에부딪혔으니 정지상태해줌
						player->SetFallSpeed(0);			//떨어질때가속도를 위한거니 이것도 정지해줌
						player->SetHurt(0);			//땅에 닿았으면 피격아님
						player->SetRopeHurt(0);		//땅에 닿았으면 피격아님

						if (player->GetRowSpeed() != 3)		//m_rowSpeed를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
							player->SetRowSpeed(3);
					}
					//X Collapse
					if (player->GetState() == EPlayerState::Idle || player->GetState() == EPlayerState::Move) //Walking Collpse
					{
						if (obstacle->GetY() < player->GetY() - player->GetHeight())
						{
							if (obstacle->GetX() < player->GetX() + player->GetWidth()) //Left Collpse
							{
								player->SetX(obstacle->GetX() - player->GetWidth());// x좌표 세팅해주고
								player->SetMoveCommand(EMoveCommand::None);
							}
						}
					}
				}
				else if (obstacle->GetType() == EObstacleType::Gas) //왼쪽 증기, 가시와 비슷함 대신 증기가 완전히 뿜어져  나왔을때 피격판정이 있다.
				{
					if (obstacle->GetSpriteIndex() == 2) //증기가 완전히 뿜어졌을때만 피격이 발생한다
					{
						if (player->GetInvincibleTime() == 0)
						{
							if (player->GetState() == EPlayerState::RopeIdle || player->GetState() == EPlayerState::RopeMove)
							{
								player->SetRopeHurt(1);
							}
							if (player->GetState() == EPlayerState::Crouch) //숙이고있었다면
							{
								player->SetY(player->GetY() - GameConst::kCrouchHeightDelta);
								player->SetHeight(player->GetHeight() + GameConst::kCrouchHeightDelta);	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
							}
							if (player->GetState() == EPlayerState::Airborne)
							{
								if (player->GetDirection() == EPlayerDirection::Left || player->GetDirection() == EPlayerDirection::Right) //무조건 왼쪽으로감
								{
									player->SetSpikeHurt(-8);
								}

								player->SetInvicible(GameConst::kInvincibleFrames);
							}
							else {
								player->SetMoveCommand(EMoveCommand::Left); //무조건 왼쪽임
								player->SetState(EPlayerState::Hurt);
							}
							player->TakeDamage();
						}
					}
				}
				else if (obstacle->GetType() == EObstacleType::GearRow || obstacle->GetType() == EObstacleType::GearCol)
				{
					if (player->GetInvincibleTime() == 0)	//무적이 아니라면
					{
						if (player->GetState() == EPlayerState::RopeIdle || player->GetState() == EPlayerState::RopeMove)
						{
							player->SetRopeHurt(1);
						}
						if (player->GetState() == EPlayerState::Crouch) //숙이고있었다면
						{
							player->SetY(player->GetY() - GameConst::kCrouchHeightDelta);
							player->SetHeight(player->GetHeight() + GameConst::kCrouchHeightDelta);	//계산전에 돌려놓고 시작한다. 이건 땅에 닿을시점에 다시돌려준다
						}
						if (player->GetState() == EPlayerState::Airborne)//일반일때는 살짝 점프 뛰듯이 가는데 떨어지는중이면 살짝만 이동한다
						{
							if (player->GetDirection() == EPlayerDirection::Left)
							{
								player->SetSpikeHurt(-8);	//8번 왼쪽으로 감
							}
							else if (player->GetDirection() == EPlayerDirection::Right)
							{
								player->SetSpikeHurt(8);	//8번 오른쪽으로감
							}

							player->SetInvicible(GameConst::kInvincibleFrames);	//무적시간 넣어줌 (이동하는로직은 state==7 일때 알아서 다뤄줌
						}
						else {
							player->SetMoveCommand(static_cast<EMoveCommand>(player->GetDirection()));
							player->SetState(EPlayerState::Hurt);		//피격으로감
						}
						player->TakeDamage();
					}
				}
			}
			else if (ObstacleTypeUtil::IsInteractive(obstacle->GetType())) //플레이어와 상호작용하는 오브젝트 ex)포탈
			{
				if (obstacle->GetType() == EObstacleType::Portal)
				{
					if (InputHelper::IsUpDown())
					{
						m.SetBlackTime(50);
						/*m.CreateBlack(g_hinst);*/
						m.SetMapNumber(m.GetMapNumber() + 1);
						player->Initialzie();
						if (m.GetMapNumber() == LevelConfig::GetClearMapNumber()) m.CreateMap(g_hinst);
						for (const auto& resetObstacle : m_obstacles)
							resetObstacle->ResetObject();
						ocount = InitObject(m.GetMapNumber(), g_hinst);
						m.CreateMap(g_hinst);
						SoundManager::Get().SetIndex(m.GetMapNumber() - static_cast<int>(EMapId::Title));
						SoundManager::Get().Update();
						SoundManager::Get().PlayBgm(static_cast<EBgm>(SoundManager::Get().GetIndex()));
						SoundManager::Get().PlayEffect(EEffect::Portal);


						return;
					}
				}
			}
			else if (ObstacleTypeUtil::IsRope(obstacle->GetType()))
			{
				if (player->GetJumpCooldown() <= 0)
				{
					if (player->IsRopeHurt() == 0)	//로프에서 맞으면 다시 로프 못탐
					{
						if (InputHelper::IsUpDown() || InputHelper::IsDownDown())
						{
							if (InputHelper::IsDownDown() && (player->GetState() == EPlayerState::Jump || player->GetState() == EPlayerState::Airborne))	//공중에있거나 점프중일때 아랫키로는 줄에 붙을수없다
								return;

							if (player->GetState() != EPlayerState::RopeIdle && player->GetState() != EPlayerState::RopeMove)	//줄에 매달려있지 않았다면 줄에 매달리는 상태를 만들어준다. 이미붙어있다면 해줄필요없음
							{
								player->SetState(EPlayerState::RopeIdle);
								if (InputHelper::IsUpDown())
									player->SetMoveCommand(EMoveCommand::Up);
								if (InputHelper::IsDownDown())
									player->SetMoveCommand(EMoveCommand::Down);
								player->SetX(obstacle->GetX() + (obstacle->GetWidth() / 2));
								if (InputHelper::IsDownDown())	//이때는 수그리기아니라 밧줄 아래로 내려가는것이므로 수그리기로 깍인거 돌려준다
								{
									player->SetY(player->GetY() - GameConst::kCrouchHeightDelta);
									player->SetHeight(25);
								}
							}
							player->PlayAnim();
							//player->BitMove();
						}
					}
				}
			}
			else if (obstacle->GetType() == EObstacleType::AnimatedBg)
			{

			}
			//if (m_rowSpeed != 3)		//m_rowSpeed를 임의로 바꿔주었다면 땅에 닿으면 초기화니 원래대로 돌려준다
			//	m_rowSpeed = 3; 잠깐 위로 올려줬음 주석처리하고 ㅇㅇ 근데 이게 맞을거같긴해

			//return;			//하나라도 부딪혔다면 그대로 탈출한다
		}
	}
	if (check_coll != 0)
		return;	//하나라도 부딪혔다면 그대로 탈출
	if ((player->GetState() == EPlayerState::Move || player->GetState() == EPlayerState::Idle) 
		|| (player->GetState() == EPlayerState::RopeIdle || player->GetState() == EPlayerState::RopeMove))	//하나도 못부딪혔으면 공중에있는거니까 떨어져야한다
	{
		player->SetState(EPlayerState::Airborne);
		player->SavePositionX();		//떨어지는 순간의 x좌표점 기억
	}


}


namespace
{
	//JSON의 문자열 type 필드 <-> EObstacleType 매핑
	EObstacleType ObstacleTypeFromName(const std::string& name)
	{
		static const std::map<std::string, EObstacleType> kNameTable = {
			{ "AnimatedBg",  EObstacleType::AnimatedBg },
			{ "Ground",      EObstacleType::Ground },
			{ "Platform",    EObstacleType::Platform },
			{ "SmallPlat",   EObstacleType::SmallPlat },
			{ "BeltRight",   EObstacleType::BeltRight },
			{ "Transparent", EObstacleType::Transparent },
			{ "BeltLeft",    EObstacleType::BeltLeft },
			{ "LongPlat",    EObstacleType::LongPlat },
			{ "Nail",        EObstacleType::Nail },
			{ "BrokenPipe",  EObstacleType::BrokenPipe },
			{ "Gas",         EObstacleType::Gas },
			{ "GearRow",     EObstacleType::GearRow },
			{ "GearCol",     EObstacleType::GearCol },
			{ "Portal",      EObstacleType::Portal },
			{ "Rope",        EObstacleType::Rope },
		};
		auto it = kNameTable.find(name);
		if (it == kNameTable.end())
			throw std::runtime_error("맵 JSON: 알 수 없는 오브젝트 타입 '" + name + "'");
		return it->second;
	}

	SPtr<Obstacle> CreateObstacleByType(EObstacleType obsType)
	{
		switch (obsType)
		{
		case EObstacleType::Ground:       return CreateSPtr<GroundObstacle>();
		case EObstacleType::AnimatedBg:   return CreateSPtr<AnimatedBgObstacle>();
		case EObstacleType::Platform:     return CreateSPtr<PlatformObstacle>();
		case EObstacleType::SmallPlat:    return CreateSPtr<SmallPlatObstacle>();
		case EObstacleType::BeltRight:
		case EObstacleType::BeltLeft:     return CreateSPtr<BeltObstacle>();
		case EObstacleType::Transparent:  return CreateSPtr<TransparentObstacle>();
		case EObstacleType::LongPlat:     return CreateSPtr<LongPlatObstacle>();
		case EObstacleType::Nail:         return CreateSPtr<NailObstacle>();
		case EObstacleType::BrokenPipe:   return CreateSPtr<BrokenPipeObstacle>();
		case EObstacleType::Gas:          return CreateSPtr<GasObstacle>();
		case EObstacleType::GearRow:
		case EObstacleType::GearCol:      return CreateSPtr<GearObstacle>();
		case EObstacleType::Portal:       return CreateSPtr<PortalObstacle>();
		case EObstacleType::Rope:         return CreateSPtr<RopeObstacle>();
		default:                          return CreateSPtr<GroundObstacle>();
		}
	}
}

//int(맵 번호) 에 따라 장애물 위치값 넣어주고 몇개의 오브젝트가 들어갔는지 알려주는 함수
int ObjectManager::InitObject(int mapnum, HINSTANCE g_hinst)
{
	//맵 파일명은 (Title 기준 오프셋)으로 정해짐: Title=map_0, GameplayMin=map_1, ..., Clear=map_(마지막)
	//맵을 추가할 땐 이 공식을 안 건드리고 map_N.json 파일과 manifest.json의 개수만 늘리면 된다
	if (mapnum < static_cast<int>(EMapId::Title))
		return 0;		//맵 값이 잘못입력되었으면 그대로 탈출
	const std::string path = "map/map_" + std::to_string(mapnum - static_cast<int>(EMapId::Title)) + ".json";

	int objcount = 0;
	try
	{
		std::ifstream in(path);
		if (!in.is_open())
			throw std::runtime_error("파일을 열 수 없음");

		nlohmann::json root;
		in >> root;

		if (!root.contains("obstacles") || !root["obstacles"].is_array())
			return 0;

		for (const auto& item : root["obstacles"])
		{
			if (objcount >= GameConst::kMaxObstaclesPerMap)
				break;

			const int x = item.at("x").get<int>();
			const int y = item.at("y").get<int>();
			const int w = item.at("w").get<int>();
			const int h = item.at("h").get<int>();
			const EObstacleType obsType = ObstacleTypeFromName(item.at("type").get<std::string>());

			SPtr<Obstacle> obs = CreateObstacleByType(obsType);
			obs->Create(x, y, w, h, static_cast<int>(obsType));
			obs->SetHbit(g_hinst);
			RegisterObstacle(obs);
			++objcount;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "맵 로드 실패 (" << path << "): " << e.what() << std::endl;
	}
	return objcount;
}

void ObjectManager::RegisterObstacle(const SPtr<Obstacle>& obstacle)
{
	m_obstacles.push_back(obstacle);
}

//카메라 무빙워크
void ObjectManager::AdjustCamera(const UPtr<CAMERA>& camera, const UPtr<PLAYER>& player)
{
	//플레이어의 머리부분이 카메라의 꼭대기점을 넘어가면 바로 따라붙게한다
	if (player->GetY() - player->GetHeight() < camera->GetY())
	{
		if (camera->GetY() <= 0)	//최상점일땐 이동해주지않음
		{

		}
		else {
			camera->SetY(player->GetY() - player->GetHeight());		//384는 맵 크기 768의 절반
		}
	}
	else if (player->GetY() + player->GetHeight() > camera->GetY() + GameConst::kViewportHeight)	//캐릭터의 발바닥이 카메라밖을 넘어서면
	{
		if (camera->GetY() >= GameConst::kDefaultCameraY)	//최하점일땐 이동해주지않음
		{

		}
		else {
			camera->SetY(player->GetY() + player->GetHeight() - 768);	//따라가준다
		}
	}
	else if (camera->GetY() + 600 != player->GetY())		//카메라가 정해진 위치에 있지않다면
	{
		if (player->GetState() != EPlayerState::Airborne)							//그리고 떨어질때까지 카메라를 바꿔주면 너무 흔들려서 이때는 무시함
		{
			if (camera->GetY() <= 0)	//최상점일땐 이동해주지않음
			{
				camera->SetY(0);
			}
			else if (camera->GetY() + 540 > player->GetY())		//얼마나 멀리있느냐에 따라 속도비를 다르게해서 카메라를 따라오게한다
			{
				camera->SetY(camera->GetY() - 4);
			}
			else if (camera->GetY() + 580 > player->GetY())
			{
				camera->SetY(camera->GetY() - 2);
			}
			else if (camera->GetY() + 600 > player->GetY())
			{
				camera->SetY(camera->GetY() - 1);
			}
			else if (camera->GetY() + 700 < player->GetY())
			{
				camera->SetY(camera->GetY() + 10);
			}
			else if (camera->GetY() + 680 < player->GetY())
			{
				camera->SetY(camera->GetY() + 8);
			}
			else if (camera->GetY() + 660 < player->GetY())
			{
				camera->SetY(camera->GetY() + 6);
			}
			else if (camera->GetY() + 640 < player->GetY())
			{
				camera->SetY(camera->GetY() + 4);
			}
			else if (camera->GetY() + 620 < player->GetY())
			{
				camera->SetY(camera->GetY() + 2);
			}
			else if (camera->GetY() + 600 < player->GetY())
			{
				camera->SetY(camera->GetY() + 1);
			}
		}
	}


}

void ObjectManager::DrawObjects()
{
	for (auto& obstacle : m_obstacles)
	{
		obstacle->DrawObj(*mem1dc);
	}
}

void ObjectManager::UpdateAll(float dt)
{
	for (auto& obstacle : m_obstacles)
		obstacle->Update(dt);
}

void ObjectManager::ResetObstacle()
{
	for (auto& obstacle : m_obstacles)
	{
		obstacle->ResetObject();
	}
}
