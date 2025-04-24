#include "Global.h"
#include "object/character/player.h"
#pragma comment (lib, "Msimg32.lib")

#include "system/Load.h"

int ROWSPEED = 3; 	//가로 이동속도
int COLSPEED = 10;	//세로 이동속도
int ROPESPEED = 2;
bool LEFTkey = 0;//왼쪽키 눌렀는지 1이면 누름 0이면 안누름
bool RIGHTkey = 0;//오른쪽키 눌렀는지 1이면 누름 0이면 안누름
bool UPkey = 0;	//위쪽키 눌렀는지 1이면 누름 0이면 안누름
bool DOWNkey = 0;//아래쪽키 눌렀는지 1이면 누름 0이면 안누름
bool LRkey = 0;//왼쪽오른쪽키 동시에 눌렀는지 1이면눌림 0이면 안눌림
bool UDkey = 0;
int jumpcount = 0;
int diecount = 0;

PLAYER::PLAYER()
	: PositionX(80), PositionY(655), SavedY(3700),
	HalfWidth(14), HalfHeight(25),
	SpriteWidth(31), SpriteHeight(25),
	CurrentHP(100), PlayerState(EPlayerState::Idle), PlayerDirection(EPlayerDirection::Right),
	FallAdjustSpeed(0), MoveCommand(EMoveCommand::Left),
	bIsHurt(false), bIsRopHurt(false), bIsDead(false),
	IsGameMode(false), InvincibleTime(0), RopeJumpCooldown(0), SpikeKnockback(0),
	CurrentBitmap(nullptr), WalkBitmap(nullptr), ActionBitmap(nullptr),
	AnimX(0), AnimY(0), AnimWidth(0), AnimHeight(0)
{

}

void PLAYER::Initialzie()
{
	PositionX = 80;
	PositionY = 3700;
	SavedY = 3700;
	HalfWidth = 14;
	HalfHeight = 25;
	PlayerState = EPlayerState::Airborne;
	PlayerDirection = EPlayerDirection::Right;
	FallAdjustSpeed = 0;
	InvincibleTime = 0;
	SpikeKnockback = 0;
	MoveCommand = EMoveCommand::None;
	bIsHurt = false;
	bIsDead = false;


}

void PLAYER::SetBitMap(HINSTANCE hInstance)
{
	WalkBitmap = LoadWalk(hInstance);
	ActionBitmap = LoadEx(hInstance);
}

void PLAYER::InitializeAnimPosition()
{
	AnimX = 0;
	AnimY = 0;
	AnimWidth = 0;
	AnimHeight = 0;
}
void PLAYER::HandleLeftPressed()
{
	LEFTkey = true;				//키 누름상태

	if (RIGHTkey == true)		//좌우를 동시에 누르고있다면 움직이지않음
	{
		LRkey = true;
		if (PlayerState == EPlayerState::Move)		//움직이고 있을때나 1로 해서 멈추게하는거지 다를때 1로바꾸면 난리남
			PlayerState = EPlayerState::Idle;
		return;
	}
	PlayerDirection = EPlayerDirection::Left;
	MoveCommand = EMoveCommand::Left;

	switch (PlayerState)
	{
	case EPlayerState::Idle: //멈춰있다가 움직일때 한번 바로 움직여줘야함
		PlayerState = EPlayerState::Move;
		break;
	case EPlayerState::Jump:
		if (bIsHurt != 1)	//쳐맞고있을때는 이 로직 안통해요~
			ROWSPEED = 1;	//점프했을때 방향을 바꾸려하면 드라마틱하게 다시 오는경우는 없지만 그래도 원했던것보단 조금 나감
		break;
	case EPlayerState::Crouch:
		HalfHeight += 12;
		PositionY -= 12;
		PlayerState = EPlayerState::Move;
		break;
	default:
		break;
	}
}

void PLAYER::HandleRightPressed()
{
	RIGHTkey = true;	//키 누름상태
	if (LEFTkey == true)
	{
		LRkey = true;
		if (PlayerState == EPlayerState::Move)		//움직이고 있을때나 1로 해서 멈추게하는거지 다를때 1로바꾸면 난리남
			PlayerState = EPlayerState::Idle;
		return;
	}
	PlayerDirection = EPlayerDirection::Right;
	MoveCommand = EMoveCommand::Right;

	switch (PlayerState)
	{
	case EPlayerState::Idle:
		PlayerState = EPlayerState::Move;
		std::cout << "RIGHT 눌림" << std::endl;
		break;
	case EPlayerState::Jump:
		if (bIsHurt != 1)	//쳐맞고있을때는 이 로직 안통해요~
			ROWSPEED = 1;	//점프했을때 방향을 바꾸려하면 드라마틱하게 다시 오는경우는 없지만 그래도 원했던것보단 조금 나감
		break;
	case EPlayerState::Crouch:
		HalfHeight += 12;
		PositionY -= 12;
		PlayerState = EPlayerState::Move;
		break;
	}
}

void PLAYER::HandleUpPressed()
{
	UPkey = true;

	if (DOWNkey == true)
	{
		UDkey = true;
		if (PlayerState == EPlayerState::RopeMove)
			PlayerState = EPlayerState::RopeIdle;
		return;
	}
	if (PlayerState == EPlayerState::RopeIdle || PlayerState == EPlayerState::RopeMove)
	{
		PlayerState = EPlayerState::RopeMove;
		MoveCommand = EMoveCommand::Up;
	}
}

void PLAYER::HandleDownPressed()
{
	if (bIsHurt == 1)
		return;
	DOWNkey = true;
	if (UPkey == true)
	{
		UDkey = true;
		if (PlayerState == EPlayerState::RopeMove)
			PlayerState = EPlayerState::RopeIdle;
		return;
	}
	if (PlayerState == EPlayerState::RopeIdle || PlayerState == EPlayerState::RopeMove)
	{

		PlayerState = EPlayerState::RopeMove;
		MoveCommand = EMoveCommand::Down;

	}
	else if (PlayerState == EPlayerState::Idle) {

		PlayerState = EPlayerState::Crouch;	//숙이는거는 가만히 있을때만 가능하다
		HalfHeight -= 12;		//숙이면 키도 줄어들어야한다.
		PositionY += 12;
	}
}

void PLAYER::HandleSpacePressed(Sound& sound)
{
	if (DOWNkey == true)//수그리고있을땐 점프못함
	{
		return;	//아무것도해주지않는다 현상태유지
	}
	if (PlayerState == EPlayerState::RopeIdle || PlayerState == EPlayerState::RopeMove)//줄에 매달렸을때
	{
		if (LRkey == 0)		//동시에 좌우키가 눌리지 않았으면서
		{
			if (LEFTkey == 1 || RIGHTkey == 1)	//둘중에 하나의 키라도 누르고있었다면 점프뜀 하지만 아니면 못뜀
			{
				MoveCommand = static_cast<EMoveCommand>(PlayerDirection);	//그리고 이때 어디로뛸건지 강제로 정함
				RopeJumpCooldown = 2;	//점프시 다시못잡게도 바꿔줌
			}
			else return;//아니면못뜀
		}
		else return;//아니면 못뜀
	}
	if (PlayerState != EPlayerState::Jump && PlayerState != EPlayerState::Airborne)	//점프나 공중이아니라면 점프뛸수있다. 하지만 줄에매달렸을때도 안되긴 마찬가지
	{
		if (sound.Channel[1]) {
			sound.Channel[1]->stop();
		}

		sound.System->playSound(
			sound.effectSound[0],
			nullptr,
			false,
			&sound.Channel[1]
		);
		jumpcount++;
		PlayerState = EPlayerState::Jump;
		SavedY = PositionY;
	}
}

void PLAYER::HandleLeftReleased()
{
	if (RIGHTkey) //오른쪽키도 누르고있었다면 왼쪽키를 땟을때 오른쪽으로 몸을틀어야한다
	{
		PlayerDirection = EPlayerDirection::Right;
		if (PlayerState == EPlayerState::Idle)			//둘다눌럿을때의 로직은 state==1일때에만 발동이 된다. 
			MoveCommand = EMoveCommand::Right;
	}
	else //오른쪽키를 누르고있지 않았다면 움직이는상태였을땐 멈춰줘야한다.
	{
		if (PlayerState == EPlayerState::Move)
		{
			PlayerState = EPlayerState::Idle;
			MoveCommand = EMoveCommand::None;	//움직이는 방향은 그대로지만 움직이지는 않는다.
		}
		else if (PlayerState == EPlayerState::Idle)	//종종버그성 플레이로인해서 (점프키와 동시에 키를 누른후 바닥에 닿음과 동시에 땔때) 이경우가있는데, 이때도 멈춰주도록한다.
		{
			MoveCommand = EMoveCommand::None;
		}
		if (DOWNkey == true)//만약 수그리고있었다면
		{
			if (PlayerState == EPlayerState::Idle)	//수그리기의 원래 알고리즘인 state==1 일때만 수그리도록 한다
			{
				PlayerState = EPlayerState::Crouch;
				HalfHeight -= 12;
				PositionY += 12;//원래대로 돌려놔주자
			}
		}
	}

	LRkey = false;				//한개를 땠으니 false
	LEFTkey = false;			//LEFTkey 땠으니 false
}

void PLAYER::HandleRightReleased()
{
	if (LEFTkey == true)		//왼쪽키도 누르고있었다면 오른쪽키를 땟을때 왼쪽으로 몸을틀어야한다
	{
		PlayerDirection = EPlayerDirection::Left;
		if (PlayerState == EPlayerState::Idle)			//둘다눌럿을때의 로직은 state==1일때에만 발동이 된다. 
			MoveCommand = EMoveCommand::Left;
	}
	else if (LEFTkey == false)	//왼쪽키를 누르고있지 않았다면 움직이는상태였을땐 멈춰줘야한다.
	{
		if (PlayerState == EPlayerState::Move)
		{
			PlayerState = EPlayerState::Idle;
			MoveCommand = EMoveCommand::None;	//움직이는 방향은 그대로지만 움직이지는 않는다.
		}
		else if (PlayerState == EPlayerState::Idle)	//종종버그성 플레이로인해서 (점프키와 동시에 키를 누른후 바닥에 닿음과 동시에 땔때) 이경우가있는데, 이때도 멈춰주도록한다.
		{
			MoveCommand = EMoveCommand::None;
		}
		if (DOWNkey == true)//만약 수그리고있었다면
		{
			if (PlayerState == EPlayerState::Idle)	//수그리기의 원래 알고리즘인 state==1 일때만 수그리도록 한다
			{
				PlayerState = EPlayerState::Crouch;
				HalfHeight -= 12;
				PositionY += 12;//원래대로 돌려놔주자
			}
		}
	}

	LRkey = false;				//한개를 땠으니 false
	RIGHTkey = false;			//RIGHTkey 땠으니 false
}

void PLAYER::HandleUpReleased()
{
	if (DOWNkey == true)
	{
		if (PlayerState == EPlayerState::RopeIdle)			//둘다눌럿을때의 로직은 state==5일때에만 발동이 된다. 
			MoveCommand = EMoveCommand::Down;
	}
	else if (DOWNkey == false)
	{
		if (PlayerState == EPlayerState::RopeMove)
		{
			PlayerState = EPlayerState::RopeIdle;
			MoveCommand = EMoveCommand::None;
		}
	}

	UPkey = false;
	UDkey = false;
}

void PLAYER::HandleDownReleased()
{
	if (UPkey == true)
	{
		if (PlayerState == EPlayerState::RopeIdle)			//둘다눌럿을때의 로직은 state==1일때에만 발동이 된다. 
			MoveCommand = EMoveCommand::Up;
	}
	else if (UPkey == false)	//오른쪽키를 누르고있지 않았다면 움직이는상태였을땐 멈춰줘야한다.
	{
		if (PlayerState == EPlayerState::RopeMove)
		{
			PlayerState = EPlayerState::RopeIdle;
			MoveCommand = EMoveCommand::None;	//움직이는 방향은 그대로지만 움직이지는 않는다.
		}
		else if (PlayerState == EPlayerState::Move)	//종종버그성 플레이로인해서 (점프키와 동시에 키를 누른후 바닥에 닿음과 동시에 땔때) 이경우가있는데, 이때도 멈춰주도록한다.
		{
			MoveCommand = EMoveCommand::None;
		}
	}
	if (DOWNkey == true)
	{
		if (PlayerState == EPlayerState::Crouch)
		{
			HalfHeight += 12;
			PositionY -= 12;	//다시 키 늘려줌
			PlayerState = EPlayerState::Idle;
		}
	}

	UDkey = false;
	DOWNkey = false;
}

void PLAYER::OnKeyPressed(WPARAM key, Sound& sound)
{
	switch (key)
	{
	case VK_LEFT:
		HandleLeftPressed();
		break;
	case VK_RIGHT:
		HandleRightPressed();
		break;
	case VK_UP:
		HandleUpPressed();
		break;
	case VK_DOWN:
		HandleDownPressed();
		break;
	case VK_SPACE:
		HandleSpacePressed(sound);
		break;
	}
}

void PLAYER::OnKeyReleased(WPARAM key)
{
	switch (key)
	{
	case VK_LEFT:
		HandleLeftReleased();
		break;
	case VK_RIGHT:
		HandleRightReleased();
		break;
	case VK_UP:
		HandleUpReleased();
		break;
	case VK_DOWN:
		HandleDownReleased();
		break;
	}
}

//플레이어 움직임
void PLAYER::UpdateMovement(int delta_time)
{
	switch(PlayerState)
	{
	case EPlayerState::Idle:
		FallAdjustSpeed = 0;		//수직낙하 한 후는 1상태가 되므로 여기서 초기화시켜줌
		if (LRkey == false)
		{
			if (RIGHTkey == true)
			{
				PlayerDirection = EPlayerDirection::Right;
				PlayerState = EPlayerState::Move;
			}
			if (LEFTkey == true)
			{
				PlayerDirection = EPlayerDirection::Left;
				PlayerState = EPlayerState::Move;
			}
		}

		break;
	case EPlayerState::Jump:

		if (MoveCommand == EMoveCommand::Left)
		{
			PositionX -= ROWSPEED;
		}
		else if (MoveCommand == EMoveCommand::Right)
		{
			PositionX += ROWSPEED;
		}

		if (bIsHurt == true)	//피격당한경우
		{
			if (abs(PositionY - SavedY) > 40) {
				PositionY -= 3;
			}
			else {
				PositionY -= COLSPEED / 2;
			}
			if (abs((PositionY - SavedY)) >= 40)	//40픽셀만큼 피격당해서 위로 살짝뜸
			{
				PlayerState = EPlayerState::Airborne;			//다시 땅으로 떨어지게함
				SavedX = PositionX;			//이순간의 x좌표를 기억함(가속도를 받다가 멈춘것처럼 해줄예정)
			}
		}
		else if (bIsHurt == false)	//일반상태
		{
			if (abs(PositionY - SavedY) > 80) {
				PositionY -= 3;
			}
			else {
				PositionY -= COLSPEED;
			}
			if (abs((PositionY - SavedY)) >= 100)
			{
				PlayerState = EPlayerState::Airborne;
				SavedX = PositionX;
			}
		}
		break;

	case EPlayerState::Move:
		if (LRkey == false)
		{
			if (delta_time % 5 == 0)
				PlayAnim();
			if (MoveCommand == EMoveCommand::Left)
				PositionX -= ROWSPEED;
			else if (MoveCommand == EMoveCommand::Right)
				PositionX += ROWSPEED;
		}
		break;

	case EPlayerState::RopeIdle:
		SavedY = PositionY;	//줄에매달렸을때는 그자리가 저장지점이다
		if (UDkey == false)
		{
			if (UPkey || DOWNkey)
				PlayerState = EPlayerState::RopeMove;
		}
		break;

	case EPlayerState::Hurt:
		ROWSPEED *= 3;
		InvincibleTime = 100;		//무적시간 2초
		SavedY = PositionY;			//피격과 동시에 y좌표저장(적당히 내려오기 위해)
		bIsHurt = true;	//피격함수 on
		PlayerState = EPlayerState::Jump;				//피격하면 공중으로 한번 붕 뜬다
		break;

	case EPlayerState::Airborne:
		PositionY += COLSPEED;					//아래로 떨어짐
		if (FallAdjustSpeed < 1000)			//1000까지만 왼쪽으로 움직임
			FallAdjustSpeed++;
		if (LEFTkey == true)			//떨어질ㄸ ㅐ 왼쪽 꾹누르고있으면
			if (FallAdjustSpeed % 30 == 0)	//타이머가 30번 돌아갈때마다 한번씩 옴겨줌
				PositionX -= ROWSPEED;
		if (RIGHTkey == true)
			if (FallAdjustSpeed % 30 == 0)
				PositionX += ROWSPEED;
		if (MoveCommand == EMoveCommand::Left)		//왼쪽으로 움직이고있다면
		{
			if (FallAdjustSpeed <= 10)	//왼쪽으로 슥 갔다가
			{
				PositionX -= ROWSPEED;
			}
			if (FallAdjustSpeed > 10)		//10번 왼쪽 갔으면 2번에 한번씩 가줌
			{
				if (FallAdjustSpeed % 2 == 0)
					PositionX -= ROWSPEED;
			}
			else if (FallAdjustSpeed > 30)	//2번씩 10번 또 갔으면 이젠 5번에 1번씩 찔끔 가줌 이건 오른쪽도 똑같이 적용
			{
				if (FallAdjustSpeed % 5 == 0)
					PositionX -= ROWSPEED;
			}

			if (LEFTkey == 0)				//50칸까지는 맨처음방향대로 가고 , 그이후에 왼쪽키를 때고있으면 멈춤당하고 아니면 왼쪽으로 쭉 날라감
				if (abs(PositionX - SavedX) > 50)
					MoveCommand = EMoveCommand::None;
		}
		else if (MoveCommand == EMoveCommand::Right)
		{
			if (FallAdjustSpeed <= 10)
			{
				PositionX += ROWSPEED;
			}
			if (FallAdjustSpeed > 10)
			{
				if (FallAdjustSpeed % 2 == 0)
					PositionX += ROWSPEED;
			}
			else if (FallAdjustSpeed > 30)
			{
				if (FallAdjustSpeed % 5 == 0)
					PositionX += ROWSPEED;
			}
			if (RIGHTkey == 0)
				if (abs(PositionX - SavedX) > 50)
					MoveCommand = EMoveCommand::None;
		}
		break;

	case EPlayerState::RopeMove:
		SavedY = PositionY;	//줄에매달렸을때는 그자리가 저장지점이다
		if (delta_time % 10 == 0)	//10번 타이머 돌아갈때 한번 움직이게해준다
			PlayAnim();
		if (UDkey == false)
		{
			if (MoveCommand == EMoveCommand::Up)
			{
				PositionY -= ROPESPEED;
			}
			else if (MoveCommand == EMoveCommand::Down)
			{
				PositionY += ROPESPEED;
			}
		}
		break;

	}

}

//상태에따라 비트맵을 선택하는 함수
void PLAYER::SelectBitmap()
{
	if (PlayerState == EPlayerState::Idle || PlayerState == EPlayerState::Move)
	{
		CurrentBitmap = WalkBitmap;
		AnimWidth = 62;
		AnimHeight = 50;
	}
	else
		CurrentBitmap = ActionBitmap;
}

//비트맵을 바꿔주는함수 (애니메이션)
void PLAYER::PlayAnim()
{
	AnimX += 1;//인덱스 형식으로 바꿈
	if (PlayerState == EPlayerState::Move)
	{
		if (AnimX >= 5) AnimX = 1;
	}
	if (PlayerState == EPlayerState::RopeIdle || PlayerState == EPlayerState::RopeMove)
	{
		if (AnimX >= 2) AnimX = 0;
	}
}

//플레이어를 그려줌
void PLAYER::Render(HDC& backDC, HDC& playerDC)
{
	BLENDFUNCTION bf;
	bf.AlphaFormat = 0;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;



	playerDC = CreateCompatibleDC(backDC);
	//피격당했을시에 투명처리 해줄 dc를 mem1dc와 연결
	HDC gdidc = CreateCompatibleDC(backDC);
	//mem1dc의 캐릭터그릴공간만큼만 얻어온다(실제 mem1dc에는 배경이있으므로 0,0 부터 62,50 까지의 비트맵이 들어감)
	HBITMAP tmpdc = CreateCompatibleBitmap(backDC, 62, 50);
	HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
	//여기서 0,0 ~62,50 까지의 비트맵을 캐릭터기준으로 바꿔준다 (플레이어가 있는 위치의 비트맵을 복사함)
	BitBlt(gdidc, 0, 0, SpriteWidth * 2, HalfHeight * 2, backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SRCCOPY);
	//기본 움직임
	SelectObject(playerDC, CurrentBitmap);

	switch (PlayerState)
	{
	case EPlayerState::Idle:
		if (PlayerDirection == EPlayerDirection::Left)//왼쪽
		{
			//gdidc는 0,0~ 62,50 이니까 이 위치에 투명한 캐릭터를 복사시켜주고 GdialphaBlend 를 통해 투명화처리 해준다.
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, 0, 0, 62, 50, RGB(255, 255, 255));

			if (InvincibleTime > 0)
			{
				bf.SourceConstantAlpha = 155;
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (PlayerDirection == EPlayerDirection::Right)//오른쪽
		{
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, 0, 50, 62, 50, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				//이 함수는 일반 stretchblt 와 비슷하다 gdidc 는 최대가 0,0 ~62,50 이므로 뒷 인자는 0 0 62 50
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		}
		break;
	case EPlayerState::Move:
		if (PlayerDirection == EPlayerDirection::Left)//왼쪽
		{
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, AnimX * 68, AnimY, AnimWidth, AnimHeight, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (PlayerDirection == EPlayerDirection::Right)//오른쪽
		{
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, AnimX * 68, AnimY + 50, AnimWidth, AnimHeight, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		}
		break;
	case EPlayerState::Jump:
	case EPlayerState::Airborne:
		if (PlayerDirection == EPlayerDirection::Left)//왼쪽
		{
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, 0, 107, 62, 50, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (PlayerDirection == EPlayerDirection::Right)//오른쪽
		{
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, 77, 107, 62, 48, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		}
		break;
	case EPlayerState::Crouch:
		//h는 줄고 y는 늘고 
		BitBlt(gdidc, 0, 0, SpriteWidth * 2, 26, backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SRCCOPY);
		if (PlayerDirection == EPlayerDirection::Left)//왼쪽
		{
			TransparentBlt(gdidc, 0, 0, 62, 26, playerDC, 0, 161, 62, 26, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - 12 - HalfHeight + 12, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - 12 - HalfHeight + 12, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 26, bf);
		}
		else if (PlayerDirection == EPlayerDirection::Right)//오른쪽
		{
			TransparentBlt(gdidc, 0, 0, 62, 26, playerDC, 77, 161, 62, 26, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//투명도
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 26, bf);
		}
		break;
	case EPlayerState::RopeIdle:
	case EPlayerState::RopeMove:
		TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, AnimX * 77, 54, 62, 50, RGB(255, 255, 255));

		if (InvincibleTime > 0)
		{

			bf.SourceConstantAlpha = 155;
			GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
			bf.SourceConstantAlpha = 255;

		}
		else GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		break;
	}

	SelectObject(gdidc, oldtmpdc);
	DeleteObject(tmpdc);
	DeleteObject(gdidc);
	DeleteObject(playerDC);

}

void PLAYER::UpdateInvincibilityTimer()
{
	if (bIsDead == 0)	//죽으면 무적안풀림
		if (InvincibleTime > 0)
		{
			InvincibleTime--;
			if (InvincibleTime == 0)
				bIsHurt = 0;
		}
	if (RopeJumpCooldown > 0)
		RopeJumpCooldown--;

}
void PLAYER::UpdateSpikeKnockback()
{
	if (SpikeKnockback < 0)
	{
		SpikeKnockback++;
		PositionX -= 4;			//왼쪽으로감
	}
	else if (SpikeKnockback > 0)
	{
		SpikeKnockback--;
		PositionX += 4;
	}
}

void PLAYER::TakeDamage(Sound& sound)
{
	if (bIsDead == false)
		CurrentHP -= 5;
	if (CurrentHP <= 0)	//0 이하라면
	{
		Die(sound);
	}
}

void PLAYER::Die(Sound& sound)
{
	CurrentHP = 0;
	bIsDead = true;
	MoveCommand = EMoveCommand::None;
	PlayerState = EPlayerState::Crouch;
	PositionY += 12;
	HalfHeight = 13;
	InvincibleTime = 1;
	LEFTkey = 0;
	RIGHTkey = 0;
	UPkey = 0;
	DOWNkey = 0;

	if (sound.Channel[1]) {
		sound.Channel[1]->stop();
	}

	sound.System->playSound(
		sound.effectSound[2],
		nullptr,
		false,
		&sound.Channel[1]
	);
	diecount++;
}
