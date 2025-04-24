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
	CurrentHP(100), state(1), dir(2),
	FallAdjustSpeed(0), COMMAND_move(0),
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
	state = 7;
	dir = 2;
	FallAdjustSpeed = 0;
	InvincibleTime = 0;
	SpikeKnockback = 0;
	COMMAND_move = false;
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

//�÷��̾� ���� ����
void PLAYER::OnKeyPressed(WPARAM key, Sound& sound)
{
	if (key == VK_LEFT)
	{
		
		LEFTkey = true;				//Ű ��������
		if (RIGHTkey == true)		//�¿츦 ���ÿ� �������ִٸ� ������������
		{
			LRkey = true;
			if (state == 4)		//�����̰� �������� 1�� �ؼ� ���߰��ϴ°��� �ٸ��� 1�ιٲٸ� ������
				state = 1;
			return;
		}

		if (state == 7)
		{
			dir = 1;	//������ �ٲ������� ���������´� ���� (�Ʒ� move���� �����̻� ������ ���ݾ��� �ٲ������)
		}
		else if (state == 1 || state == 4)
		{

			if (state == 1)//�����ִٰ� �����϶� �ѹ� �ٷ� �����������
			{

				state = 4;
				dir = 1;
				
			}
			else {
				state = 4;
				dir = 1;
			}
			COMMAND_move = 1;	//1�̵� 4�� ������ �ϴ��� ���������¸� �ٲ���
			std::cout << "LEFT����" << std::endl;
		}
		else if (state == 2)
		{
			if (bIsHurt != 1)	//�ĸ°��������� �� ���� �����ؿ�~
				ROWSPEED = 1;	//���������� ������ �ٲٷ��ϸ� ���ƽ�ϰ� �ٽ� ���°��� ������ �׷��� ���ߴ��ͺ��� ���� ����
			dir = 1;	//������ �ٲ������� ���������´� ����
		}
		else if (state == 3)
		{

			HalfHeight += 12;
			PositionY -= 12;
			state = 4;
			dir = 1;
			COMMAND_move = 1;
		}
		else if (state == 5 || state == 8)
		{
			dir = 1;
			//COMMAND_move = 1;
		}
		return;
	}
	if (key == VK_RIGHT)
	{
		RIGHTkey = true;	//Ű ��������
		if (LEFTkey == true)
		{
			LRkey = true;
			if (state == 4)		//�����̰� �������� 1�� �ؼ� ���߰��ϴ°��� �ٸ��� 1�ιٲٸ� ������
				state = 1;
			return;
		}
		if (state == 7)
		{
			dir = 2;	//������ �ٲ������� ���������´� ���� (�Ʒ� move���� �����̻� ������ ���ݾ��� �ٲ������)
		}
		else if (state == 1 || state == 4)
		{
			if (state == 1)//�����ִٰ� �����϶� �ѹ� �ٷ� �����������
			{

				state = 4;
				dir = 2;
			}
			else {
				state = 4;
				dir = 2;
				
			}
			COMMAND_move = 2;	//1�̵� 4�� ������ �ϴ��� ���������¸� �ٲ���
			std::cout << "RIGHT ����" << std::endl;
		}
		else if (state == 2)
		{
			if (bIsHurt != 1)	//�ĸ°��������� �� ���� �����ؿ�~
				ROWSPEED = 1;	//���������� ������ �ٲٷ��ϸ� ���ƽ�ϰ� �ٽ� ���°��� ������ �׷��� ���ߴ��ͺ��� ���� ����
			dir = 2;	//������ �ٲ������� ���������´� ����
		}
		else if (state == 3)
		{
			HalfHeight += 12;
			PositionY -= 12;
			state = 4;
			dir = 2;
			COMMAND_move = 2;
		}
		else if (state == 5 || state == 8)
		{
			dir = 2;
			//COMMAND_move = 2;
		}

		return;
	}
	if (key == VK_UP)
	{
		UPkey = true;

		if (DOWNkey == true)
		{
			UDkey = true;
			if (state == 8)
				state = 5;
			return;
		}
		if (state == 5 || state == 8)
		{
			state = 8;
			COMMAND_move = 3;

		}

		return;
	}
	if (key == VK_DOWN)
	{
		if (bIsHurt == 1)
			return;
		DOWNkey = true;
		if (UPkey == true)
		{
			UDkey = true;
			if (state == 8)
				state = 5;
			return;
		}
		if (state == 5 || state == 8)
		{
			
			if (state == 5)//�����ִٰ� �����϶� �ѹ� �ٷ� �����������
			{

				state = 8;
			}
			else {
				//BitMove();
				state = 8;
			}
			COMMAND_move = 4;

		}
		else if (state == 1) {

			state = 3;	//���̴°Ŵ� ������ �������� �����ϴ�
			HalfHeight -= 12;		//���̸� Ű�� �پ�����Ѵ�.
			PositionY += 12;
		}

		return;
	}
	if (key == VK_SPACE)
	{
		if (DOWNkey == true)//���׸��������� ��������
		{
			return;	//�ƹ��͵��������ʴ´� ����������
		}
		if (state == 5 || state == 8)//�ٿ� �Ŵ޷�����
		{
			if (LRkey == 0)		//���ÿ� �¿�Ű�� ������ �ʾ����鼭
			{
				if (LEFTkey == 1 || RIGHTkey == 1)	//���߿� �ϳ��� Ű�� �������־��ٸ� ������ ������ �ƴϸ� ����
				{
					COMMAND_move = dir;	//�׸��� �̶� ���ζ۰��� ������ ����
					RopeJumpCooldown = 2;	//������ �ٽø���Ե� �ٲ���
				}
				else return;//�ƴϸ����
			}else return;//�ƴϸ� ����
		}
		if (state != 2 && state != 7)	//������ �����̾ƴ϶�� �����ۼ��ִ�. ������ �ٿ��Ŵ޷������� �ȵǱ� ��������
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
			state = 2;
			SavedY = PositionY;
		}
		return;
	}
}
//�÷��̾� ������
void PLAYER::OnKeyReleased(WPARAM key)
{
	if (key == VK_DOWN)
	{
		if (UPkey == true)
		{
			if (state == 5)			//�Ѵٴ��������� ������ state==1�϶����� �ߵ��� �ȴ�. 
				COMMAND_move = 3;
		}
		else if (UPkey == false)	//������Ű�� ���������� �ʾҴٸ� �����̴»��¿����� ��������Ѵ�.
		{
			if (state == 8)
			{
				state = 5;
				COMMAND_move = 0;	//�����̴� ������ �״������ ���������� �ʴ´�.
			}
			else if (state == 5)	//�������׼� �÷��̷����ؼ� (����Ű�� ���ÿ� Ű�� ������ �ٴڿ� ������ ���ÿ� ����) �̰�찡�ִµ�, �̶��� �����ֵ����Ѵ�.
			{
				COMMAND_move = 0;
			}
		}
		if (DOWNkey == true)
		{
			if (state == 3)
			{
				HalfHeight += 12;
				PositionY -= 12;	//�ٽ� Ű �÷���
				state = 1;
			}
		}

		UDkey = false;
		DOWNkey = false;
		return;
	}
	if (key == VK_LEFT)
	{
		if (RIGHTkey == true)		//������Ű�� �������־��ٸ� ����Ű�� ������ ���������� ����Ʋ����Ѵ�
		{
			dir = 2;
			if (state == 1)			//�Ѵٴ��������� ������ state==1�϶����� �ߵ��� �ȴ�. 
				COMMAND_move = 2;
		}
		else if (RIGHTkey == false)	//������Ű�� ���������� �ʾҴٸ� �����̴»��¿����� ��������Ѵ�.
		{
			if (state == 4)
			{
				state = 1;
				COMMAND_move = 0;	//�����̴� ������ �״������ ���������� �ʴ´�.
			}
			else if (state == 1)	//�������׼� �÷��̷����ؼ� (����Ű�� ���ÿ� Ű�� ������ �ٴڿ� ������ ���ÿ� ����) �̰�찡�ִµ�, �̶��� �����ֵ����Ѵ�.
			{
				COMMAND_move = 0;
			}
			if (DOWNkey == true)//���� ���׸����־��ٸ�
			{
				if (state == 1)	//���׸����� ���� �˰����� state==1 �϶��� ���׸����� �Ѵ�
				{
					state = 3;
					HalfHeight -= 12;
					PositionY += 12;//������� ����������
				}
			}
		}


		LRkey = false;				//�Ѱ��� ������ false
		LEFTkey = false;			//LEFTkey ������ false

		//std::cout << "LEFT �E" << std::endl;

		return;
	}
	if (key == VK_RIGHT)
	{
		if (LEFTkey == true)		//����Ű�� �������־��ٸ� ������Ű�� ������ �������� ����Ʋ����Ѵ�
		{
			dir = 1;
			if (state == 1)			//�Ѵٴ��������� ������ state==1�϶����� �ߵ��� �ȴ�. 
				COMMAND_move = 1;
		}
		else if (LEFTkey == false)	//����Ű�� ���������� �ʾҴٸ� �����̴»��¿����� ��������Ѵ�.
		{
			if (state == 4)
			{
				state = 1;
				COMMAND_move = 0;	//�����̴� ������ �״������ ���������� �ʴ´�.
			}
			else if (state == 1)	//�������׼� �÷��̷����ؼ� (����Ű�� ���ÿ� Ű�� ������ �ٴڿ� ������ ���ÿ� ����) �̰�찡�ִµ�, �̶��� �����ֵ����Ѵ�.
			{
				COMMAND_move = 0;
			}
			if (DOWNkey == true)//���� ���׸����־��ٸ�
			{
				if (state == 1)	//���׸����� ���� �˰����� state==1 �϶��� ���׸����� �Ѵ�
				{
					state = 3;
					HalfHeight -= 12;
					PositionY += 12;//������� ����������
				}
			}
		}



		LRkey = false;				//�Ѱ��� ������ false
		RIGHTkey = false;			//RIGHTkey ������ false
		//std::cout << "RIGTH ��" << std::endl;

		return;
	}
	if (key == VK_UP)
	{
		if (DOWNkey == true)
		{
			if (state == 5)			//�Ѵٴ��������� ������ state==5�϶����� �ߵ��� �ȴ�. 
				COMMAND_move = 4;
		}
		else if (DOWNkey == false)	
		{
			if (state == 8)
			{
				state = 5;
				COMMAND_move = 0;	//�����̴� ������ �״������ ���������� �ʴ´�.
			}
		}

		UPkey = false;
		UDkey = false;
		return;
	}
	return;
}


//�÷��̾� ������
void PLAYER::UpdateMovement(int delta_time)
{

	if (state == 1)
	{
		FallAdjustSpeed = 0;		//�������� �� �Ĵ� 1���°� �ǹǷ� ���⼭ �ʱ�ȭ������
		if (LRkey == true)
		{

		}
		else if (RIGHTkey == true)
		{
			dir = 2;
			state = 4;
		}
		else if (LEFTkey == true)
		{
			dir = 1;
			state = 4;
		}

		//else {
		//	if (COMMAND_move == 1)
		//	{
		//		x -= ROWSPEED;
		//	}
		//	else if (COMMAND_move == 2)
		//	{
		//		x += ROWSPEED;
		//	}
		//}
		//�������������� �����ϼ� ���µ� �� �������� ����
	}
	else if (state == 2 )	//���������϶��� ����������� �̵����ؾ��� 
	{
		if (bIsHurt == true)	//�ǰݴ��Ѱ��
		{
			if (COMMAND_move == 1)
			{

				PositionX -= ROWSPEED;
			}
			else if (COMMAND_move == 2)
			{
				PositionX += ROWSPEED;
			}
			//y -= 1;
			if (abs(PositionY - SavedY) > 40) {
				PositionY -= 3;
			}
			else {
				PositionY -= COLSPEED / 2;
			}
			if (abs((PositionY - SavedY)) >= 40)	//40�ȼ���ŭ �ǰݴ��ؼ� ���� ��¦��
			{
				state = 7;			//�ٽ� ������ ����������
				SavedX = PositionX;			//�̼����� x��ǥ�� �����(���ӵ��� �޴ٰ� �����ó�� ���ٿ���)
			}
		}
		else if (bIsHurt == false)	//�Ϲݻ���
		{
			if (COMMAND_move == 1)
			{

				PositionX -= ROWSPEED;
			}
			else if (COMMAND_move == 2)
			{
				PositionX += ROWSPEED;
			}
			//y -= 1;
			if (abs(PositionY - SavedY) > 80) {
				PositionY -= 3;
			}
			else {
				PositionY -= COLSPEED;
			}
			if (abs((PositionY - SavedY)) >= 100)	//30�ȼ���ŭ �����ߴٸ�
			{
				state = 7;			//�ٽ� ������ ����������
				SavedX = PositionX;			//�̼����� x��ǥ�� �����(���ӵ��� �޴ٰ� �����ó�� ���ٿ���)
			}
		}


	}
	else if (state == 3)
	{

	}
	else if (state == 4)
	{

		if (LRkey == true)
		{

		}
		else {
			if (delta_time % 5 == 0)
			{
				BitMove();
			}

			if (COMMAND_move == 1)
			{
				PositionX -= ROWSPEED;
			}
			else if (COMMAND_move == 2)
			{
				PositionX += ROWSPEED;
			}
		}
	}
	else if (state == 5)
	{

	SavedY = PositionY;	//�ٿ��Ŵ޷������� ���ڸ��� ���������̴�
		if (UDkey == true)
		{

		}
		else if (UPkey == true)
		{
			
			state = 8;

		}
		else if (DOWNkey == true)
		{
			
			state = 8;

		}
	}
	else if (state == 6)
	{
		ROWSPEED *= 3;
		InvincibleTime = 100;		//�����ð� 2��
		SavedY = PositionY;			//�ǰݰ� ���ÿ� y��ǥ����(������ �������� ����)
		bIsHurt = true;	//�ǰ��Լ� on
		state = 2;				//�ǰ��ϸ� �������� �ѹ� �� ���

	}
	else if (state == 7)
	{
		PositionY += COLSPEED;					//�Ʒ��� ������
		if (FallAdjustSpeed < 1000)			//1000������ �������� ������
			FallAdjustSpeed++;
		if (LEFTkey == true)			//�������� �� ���� �ڴ�����������
			if (FallAdjustSpeed % 30 == 0)	//Ÿ�̸Ӱ� 30�� ���ư������� �ѹ��� �Ȱ���
				PositionX -= ROWSPEED;
		if (RIGHTkey == true)
			if (FallAdjustSpeed % 30 == 0)
				PositionX += ROWSPEED;
		if (COMMAND_move == 1)		//�������� �����̰��ִٸ�
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
					COMMAND_move = 0;

		}
		else if (COMMAND_move == 2)
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
					COMMAND_move = 0;
		}
	}
	else if (state == 8)
	{
		SavedY = PositionY;	//�ٿ��Ŵ޷������� ���ڸ��� ���������̴�
		if(delta_time%10==0)	//10�� Ÿ�̸� ���ư��� �ѹ� �����̰����ش�
			BitMove();
		if (UDkey == true)
		{

		}
		else {
			

			if (COMMAND_move == 3)
			{
				PositionY -= ROPESPEED;
			}
			else if (COMMAND_move == 4)
			{
				PositionY += ROPESPEED;
			}
		}
	}
}

//���¿����� ��Ʈ���� �����ϴ� �Լ�
void PLAYER::selectBit()
{
	if (state == 1 || state == 4)
	{
		CurrentBitmap = WalkBitmap;
		AnimWidth = 62;
		AnimHeight = 50;
	}

	else
		CurrentBitmap = ActionBitmap;
}

//��Ʈ���� �ٲ��ִ��Լ� (�ִϸ��̼�)
void PLAYER::BitMove()
{
	AnimX += 1;//�ε��� �������� �ٲ�
	if (state == 4)
	{
		if (AnimX >= 5) AnimX = 1;
	}
	if (state == 5||state==8)
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
	if (state == 1) // �������� 
	{

		if (dir == 1)//����
		{
			//TransparentBlt(gdidc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 0, 62, 50, RGB(255, 255, 255));
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
		else if (dir == 2)//������
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 50, 62, 50, RGB(255, 255, 255));
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

	}
	else if (state == 4) //�̵�����
	{
		if (dir == 1)//����
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by, bw, bh, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, AnimX * 68, AnimY, AnimWidth, AnimHeight, RGB(255, 255, 255));
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
		else if (dir == 2)//������
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by + 50, bw, bh, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, AnimX * 68, AnimY + 50, AnimWidth, AnimHeight, RGB(255, 255, 255));
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


	}
	else if (state == 2 || state == 7) //�����ϰų� ��������
	{
		if (dir == 1)//����
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 107, 62, 48, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, 0, 107, 62, 50, RGB(255, 255, 255));
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
		else if (dir == 2)//������
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 107, 62, 48, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, 77, 107, 62, 48, RGB(255, 255, 255));
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
	}
	else if (state == 3) //���̱�
	{
		//h�� �ٰ� y�� �ð� 

		BitBlt(gdidc, 0, 0, SpriteWidth * 2, 26, backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SRCCOPY);
		if (dir == 1)//����
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 161, 62, 26, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 26, playerDC, 0, 161, 62, 26, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - 12 - HalfHeight + 12, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - 12 - HalfHeight + 12, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 26, bf);
		}
		else if (dir == 2)//������
		{
			//	TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 161, 62, 26, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 26, playerDC, 77, 161, 62, 26, RGB(255, 255, 255));
			if (InvincibleTime > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 26, bf);
		}
	}
	else if (state == 5 || state == 8)	//�ٿ� �Ŵ޸�����
	{
		TransparentBlt(gdidc, 0, 0, 62, 50, playerDC, AnimX * 77, 54, 62, 50, RGB(255, 255, 255));

		if (InvincibleTime > 0)
		{

			bf.SourceConstantAlpha = 155;
			GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);
			bf.SourceConstantAlpha = 255;

		}
		else GdiAlphaBlend(backDC, PositionX - SpriteWidth, PositionY - HalfHeight, SpriteWidth * 2, HalfHeight * 2, gdidc, 0, 0, 62, 50, bf);

	}

	SelectObject(gdidc, oldtmpdc);
	DeleteObject(tmpdc);
	DeleteObject(gdidc);
	DeleteObject(playerDC);

}

void PLAYER::UpdateInvincibilityTimer()
{
	if(bIsDead==0)	//������ ������Ǯ��
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
		CurrentHP = 0;
		bIsDead = true;
		COMMAND_move = false;
		state = 3;
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
}