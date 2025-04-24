#include "Global.h"
#include "object/character/player.h"
#pragma comment (lib, "Msimg32.lib")

#include "system/Load.h"

int ROWSPEED = 3; 	//���� �̵��ӵ�
int COLSPEED = 10;	//���� �̵��ӵ�
int ROPESPEED = 2;
bool LEFTkey = 0;//����Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool RIGHTkey = 0;//������Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool UPkey = 0;	//����Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool DOWNkey = 0;//�Ʒ���Ű �������� 1�̸� ���� 0�̸� �ȴ���
bool LRkey = 0;//���ʿ�����Ű ���ÿ� �������� 1�̸鴭�� 0�̸� �ȴ���
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
	LEFTkey = true;				//Ű ��������

	if (RIGHTkey == true)		//�¿츦 ���ÿ� �������ִٸ� ������������
	{
		LRkey = true;
		if (PlayerState == EPlayerState::Move)		//�����̰� �������� 1�� �ؼ� ���߰��ϴ°��� �ٸ��� 1�ιٲٸ� ������
			PlayerState = EPlayerState::Idle;
		return;
	}
	PlayerDirection = EPlayerDirection::Left;
	MoveCommand = EMoveCommand::Left;

	switch (PlayerState)
	{
	case EPlayerState::Idle: //�����ִٰ� �����϶� �ѹ� �ٷ� �����������
		PlayerState = EPlayerState::Move;
		break;
	case EPlayerState::Jump:
		if (bIsHurt != 1)	//�ĸ°��������� �� ���� �����ؿ�~
			ROWSPEED = 1;	//���������� ������ �ٲٷ��ϸ� ���ƽ�ϰ� �ٽ� ���°��� ������ �׷��� ���ߴ��ͺ��� ���� ����
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
	RIGHTkey = true;	//Ű ��������
	if (LEFTkey == true)
	{
		LRkey = true;
		if (PlayerState == EPlayerState::Move)		//�����̰� �������� 1�� �ؼ� ���߰��ϴ°��� �ٸ��� 1�ιٲٸ� ������
			PlayerState = EPlayerState::Idle;
		return;
	}
	PlayerDirection = EPlayerDirection::Right;
	MoveCommand = EMoveCommand::Right;

	switch (PlayerState)
	{
	case EPlayerState::Idle:
		PlayerState = EPlayerState::Move;
		std::cout << "RIGHT ����" << std::endl;
		break;
	case EPlayerState::Jump:
		if (bIsHurt != 1)	//�ĸ°��������� �� ���� �����ؿ�~
			ROWSPEED = 1;	//���������� ������ �ٲٷ��ϸ� ���ƽ�ϰ� �ٽ� ���°��� ������ �׷��� ���ߴ��ͺ��� ���� ����
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

		PlayerState = EPlayerState::Crouch;	//���̴°Ŵ� ������ �������� �����ϴ�
		HalfHeight -= 12;		//���̸� Ű�� �پ�����Ѵ�.
		PositionY += 12;
	}
}

void PLAYER::HandleSpacePressed(Sound& sound)
{
	if (DOWNkey == true)//���׸��������� ��������
	{
		return;	//�ƹ��͵��������ʴ´� ����������
	}
	if (PlayerState == EPlayerState::RopeIdle || PlayerState == EPlayerState::RopeMove)//�ٿ� �Ŵ޷�����
	{
		if (LRkey == 0)		//���ÿ� �¿�Ű�� ������ �ʾ����鼭
		{
			if (LEFTkey == 1 || RIGHTkey == 1)	//���߿� �ϳ��� Ű�� �������־��ٸ� ������ ������ �ƴϸ� ����
			{
				MoveCommand = static_cast<EMoveCommand>(PlayerDirection);	//�׸��� �̶� ���ζ۰��� ������ ����
				RopeJumpCooldown = 2;	//������ �ٽø���Ե� �ٲ���
			}
			else return;//�ƴϸ����
		}
		else return;//�ƴϸ� ����
	}
	if (PlayerState != EPlayerState::Jump && PlayerState != EPlayerState::Airborne)	//������ �����̾ƴ϶�� �����ۼ��ִ�. ������ �ٿ��Ŵ޷������� �ȵǱ� ��������
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
	if (RIGHTkey) //������Ű�� �������־��ٸ� ����Ű�� ������ ���������� ����Ʋ����Ѵ�
	{
		PlayerDirection = EPlayerDirection::Right;
		if (PlayerState == EPlayerState::Idle)			//�Ѵٴ��������� ������ state==1�϶����� �ߵ��� �ȴ�. 
			MoveCommand = EMoveCommand::Right;
	}
	else //������Ű�� ���������� �ʾҴٸ� �����̴»��¿����� ��������Ѵ�.
	{
		if (PlayerState == EPlayerState::Move)
		{
			PlayerState = EPlayerState::Idle;
			MoveCommand = EMoveCommand::None;	//�����̴� ������ �״������ ���������� �ʴ´�.
		}
		else if (PlayerState == EPlayerState::Idle)	//�������׼� �÷��̷����ؼ� (����Ű�� ���ÿ� Ű�� ������ �ٴڿ� ������ ���ÿ� ����) �̰�찡�ִµ�, �̶��� �����ֵ����Ѵ�.
		{
			MoveCommand = EMoveCommand::None;
		}
		if (DOWNkey == true)//���� ���׸����־��ٸ�
		{
			if (PlayerState == EPlayerState::Idle)	//���׸����� ���� �˰����� state==1 �϶��� ���׸����� �Ѵ�
			{
				PlayerState = EPlayerState::Crouch;
				HalfHeight -= 12;
				PositionY += 12;//������� ����������
			}
		}
	}

	LRkey = false;				//�Ѱ��� ������ false
	LEFTkey = false;			//LEFTkey ������ false
}

void PLAYER::HandleRightReleased()
{
	if (LEFTkey == true)		//����Ű�� �������־��ٸ� ������Ű�� ������ �������� ����Ʋ����Ѵ�
	{
		PlayerDirection = EPlayerDirection::Left;
		if (PlayerState == EPlayerState::Idle)			//�Ѵٴ��������� ������ state==1�϶����� �ߵ��� �ȴ�. 
			MoveCommand = EMoveCommand::Left;
	}
	else if (LEFTkey == false)	//����Ű�� ���������� �ʾҴٸ� �����̴»��¿����� ��������Ѵ�.
	{
		if (PlayerState == EPlayerState::Move)
		{
			PlayerState = EPlayerState::Idle;
			MoveCommand = EMoveCommand::None;	//�����̴� ������ �״������ ���������� �ʴ´�.
		}
		else if (PlayerState == EPlayerState::Idle)	//�������׼� �÷��̷����ؼ� (����Ű�� ���ÿ� Ű�� ������ �ٴڿ� ������ ���ÿ� ����) �̰�찡�ִµ�, �̶��� �����ֵ����Ѵ�.
		{
			MoveCommand = EMoveCommand::None;
		}
		if (DOWNkey == true)//���� ���׸����־��ٸ�
		{
			if (PlayerState == EPlayerState::Idle)	//���׸����� ���� �˰����� state==1 �϶��� ���׸����� �Ѵ�
			{
				PlayerState = EPlayerState::Crouch;
				HalfHeight -= 12;
				PositionY += 12;//������� ����������
			}
		}
	}

	LRkey = false;				//�Ѱ��� ������ false
	RIGHTkey = false;			//RIGHTkey ������ false
}

void PLAYER::HandleUpReleased()
{
	if (DOWNkey == true)
	{
		if (PlayerState == EPlayerState::RopeIdle)			//�Ѵٴ��������� ������ state==5�϶����� �ߵ��� �ȴ�. 
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
		if (PlayerState == EPlayerState::RopeIdle)			//�Ѵٴ��������� ������ state==1�϶����� �ߵ��� �ȴ�. 
			MoveCommand = EMoveCommand::Up;
	}
	else if (UPkey == false)	//������Ű�� ���������� �ʾҴٸ� �����̴»��¿����� ��������Ѵ�.
	{
		if (PlayerState == EPlayerState::RopeMove)
		{
			PlayerState = EPlayerState::RopeIdle;
			MoveCommand = EMoveCommand::None;	//�����̴� ������ �״������ ���������� �ʴ´�.
		}
		else if (PlayerState == EPlayerState::Move)	//�������׼� �÷��̷����ؼ� (����Ű�� ���ÿ� Ű�� ������ �ٴڿ� ������ ���ÿ� ����) �̰�찡�ִµ�, �̶��� �����ֵ����Ѵ�.
		{
			MoveCommand = EMoveCommand::None;
		}
	}
	if (DOWNkey == true)
	{
		if (PlayerState == EPlayerState::Crouch)
		{
			HalfHeight += 12;
			PositionY -= 12;	//�ٽ� Ű �÷���
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

//�÷��̾� ������
void PLAYER::UpdateMovement(int delta_time)
{
	switch(PlayerState)
	{
	case EPlayerState::Idle:
		FallAdjustSpeed = 0;		//�������� �� �Ĵ� 1���°� �ǹǷ� ���⼭ �ʱ�ȭ������
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

		if (bIsHurt == true)	//�ǰݴ��Ѱ��
		{
			if (abs(PositionY - SavedY) > 40) {
				PositionY -= 3;
			}
			else {
				PositionY -= COLSPEED / 2;
			}
			if (abs((PositionY - SavedY)) >= 40)	//40�ȼ���ŭ �ǰݴ��ؼ� ���� ��¦��
			{
				PlayerState = EPlayerState::Airborne;			//�ٽ� ������ ����������
				SavedX = PositionX;			//�̼����� x��ǥ�� �����(���ӵ��� �޴ٰ� �����ó�� ���ٿ���)
			}
		}
		else if (bIsHurt == false)	//�Ϲݻ���
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
		SavedY = PositionY;	//�ٿ��Ŵ޷������� ���ڸ��� ���������̴�
		if (UDkey == false)
		{
			if (UPkey || DOWNkey)
				PlayerState = EPlayerState::RopeMove;
		}
		break;

	case EPlayerState::Hurt:
		ROWSPEED *= 3;
		InvincibleTime = 100;		//�����ð� 2��
		SavedY = PositionY;			//�ǰݰ� ���ÿ� y��ǥ����(������ �������� ����)
		bIsHurt = true;	//�ǰ��Լ� on
		PlayerState = EPlayerState::Jump;				//�ǰ��ϸ� �������� �ѹ� �� ���
		break;

	case EPlayerState::Airborne:
		PositionY += COLSPEED;					//�Ʒ��� ������
		if (FallAdjustSpeed < 1000)			//1000������ �������� ������
			FallAdjustSpeed++;
		if (LEFTkey == true)			//�������� �� ���� �ڴ�����������
			if (FallAdjustSpeed % 30 == 0)	//Ÿ�̸Ӱ� 30�� ���ư������� �ѹ��� �Ȱ���
				PositionX -= ROWSPEED;
		if (RIGHTkey == true)
			if (FallAdjustSpeed % 30 == 0)
				PositionX += ROWSPEED;
		if (MoveCommand == EMoveCommand::Left)		//�������� �����̰��ִٸ�
		{
			if (FallAdjustSpeed <= 10)	//�������� �� ���ٰ�
			{
				PositionX -= ROWSPEED;
			}
			if (FallAdjustSpeed > 10)		//10�� ���� ������ 2���� �ѹ��� ����
			{
				if (FallAdjustSpeed % 2 == 0)
					PositionX -= ROWSPEED;
			}
			else if (FallAdjustSpeed > 30)	//2���� 10�� �� ������ ���� 5���� 1���� ��� ���� �̰� �����ʵ� �Ȱ��� ����
			{
				if (FallAdjustSpeed % 5 == 0)
					PositionX -= ROWSPEED;
			}

			if (LEFTkey == 0)				//50ĭ������ ��ó�������� ���� , �����Ŀ� ����Ű�� ���������� ������ϰ� �ƴϸ� �������� �� ����
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
		SavedY = PositionY;	//�ٿ��Ŵ޷������� ���ڸ��� ���������̴�
		if (delta_time % 10 == 0)	//10�� Ÿ�̸� ���ư��� �ѹ� �����̰����ش�
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

//���¿����� ��Ʈ���� �����ϴ� �Լ�
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

//��Ʈ���� �ٲ��ִ��Լ� (�ִϸ��̼�)
void PLAYER::PlayAnim()
{
	AnimX += 1;//�ε��� �������� �ٲ�
	if (PlayerState == EPlayerState::Move)
	{
		if (AnimX >= 5) AnimX = 1;
	}
	if (PlayerState == EPlayerState::RopeIdle || PlayerState == EPlayerState::RopeMove)
	{
		if (AnimX >= 2) AnimX = 0;
	}
}

//�÷��̾ �׷���
void PLAYER::Render(HDC& backDC, HDC& playerDC)
{
	BLENDFUNCTION bf;
	bf.AlphaFormat = 0;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;



	playerDC = CreateCompatibleDC(backDC);
	//�ǰݴ������ÿ� ����ó�� ���� dc�� mem1dc�� ����
	HDC gdidc = CreateCompatibleDC(backDC);
	//mem1dc�� ĳ���ͱ׸�������ŭ�� ���´�(���� mem1dc���� ����������Ƿ� 0,0 ���� 62,50 ������ ��Ʈ���� ��)
	HBITMAP tmpdc = CreateCompatibleBitmap(backDC, 62, 50);
	HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
	//���⼭ 0,0 ~62,50 ������ ��Ʈ���� ĳ���ͱ������� �ٲ��ش� (�÷��̾ �ִ� ��ġ�� ��Ʈ���� ������)
	BitBlt(gdidc, 0, 0, SpriteWidth * 2, HalfHeight * 2, backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SRCCOPY);
	//�⺻ ������
	SelectObject(playerDC, CurrentBitmap);

	switch (PlayerState)
	{
	case EPlayerState::Idle:
		if (PlayerDirection == EPlayerDirection::Left)//����
		{
			//gdidc�� 0,0~ 62,50 �̴ϱ� �� ��ġ�� ������ ĳ���͸� ��������ְ� GdialphaBlend �� ���� ����ȭó�� ���ش�.
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
		else if (PlayerDirection == EPlayerDirection::Right)//������
		{
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, 0, 50, 62, 50, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		}
		break;
	case EPlayerState::Move:
		if (PlayerDirection == EPlayerDirection::Left)//����
		{
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, AnimX * 68, AnimY, AnimWidth, AnimHeight, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (PlayerDirection == EPlayerDirection::Right)//������
		{
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, AnimX * 68, AnimY + 50, AnimWidth, AnimHeight, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		}
		break;
	case EPlayerState::Jump:
	case EPlayerState::Airborne:
		if (PlayerDirection == EPlayerDirection::Left)//����
		{
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, 0, 107, 62, 50, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (PlayerDirection == EPlayerDirection::Right)//������
		{
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, 77, 107, 62, 48, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
		}
		break;
	case EPlayerState::Crouch:
		//h�� �ٰ� y�� �ð� 
		BitBlt(gdidc, 0, 0, SpriteWidth * 2, 26, backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SRCCOPY);
		if (PlayerDirection == EPlayerDirection::Left)//����
		{
			TransparentBlt(gdidc, 0, 0, 62, 26, playerDC, 0, 161, 62, 26, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - 12 - HalfHeight + 12, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - 12 - HalfHeight + 12, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 26, bf);
		}
		else if (PlayerDirection == EPlayerDirection::Right)//������
		{
			TransparentBlt(gdidc, 0, 0, 62, 26, playerDC, 77, 161, 62, 26, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//����
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
	if (bIsDead == 0)	//������ ������Ǯ��
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
		PositionX -= 4;			//�������ΰ�
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
	if (CurrentHP <= 0)	//0 ���϶��
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
