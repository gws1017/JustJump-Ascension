#include "Global.h"
#include "player.h"
#pragma comment (lib, "Msimg32.lib")

#include "Load.h"

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
{
	// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�. 
	x = 80; //100 ĳ������ �߽�x��ǥ
	y = 655; //3800 ĳ������ �߽�y��ǥ
	savey = 3700;	//ó�� ĳ������ �߽���ǥ
	w = 14; //ĳ���� width�� ����
	h = 25;	//ĳ���� hegiht�� ����
	charw = 31;	//ĳ��������Ʈ width�� ����
	charh = 25;	//ĳ��������Ʈ height�� ����
	hp = 100;	//ĳ���� hp�� 100
	state = 1; //ĳ������ state
	dir = 2;
	adjustspd = 0;
	stealth = 0;
	spike_hurt = 0;
	COMMAND_move = false;
	COMMAND_hurt = false;
	COMMAND_die = false;
	COMMAND_ropehurt = false;
	
}
void PLAYER::initPos()
{
	x = 80;
	y = 3700;
	savey = 3700;
	w = 14;
	h = 25;
	state = 7;
	dir = 2;
	adjustspd = 0;
	stealth = 0;
	spike_hurt = 0;
	COMMAND_move = false;
	COMMAND_hurt = false;
	COMMAND_die = false;
}
void PLAYER::setx(int i)
{
	x = i;
}

void PLAYER::sety(int i)
{
	y = i;
}

void PLAYER::setsavey(int i)
{
	savey = i;
}

void PLAYER::setw(int i)
{
	w = i;
}

void PLAYER::seth(int i)
{
	h = i;
}

void PLAYER::setcharw(int i)
{
	charw = i;
}

void PLAYER::setcharh(int i)
{
	charh = i;
}

void PLAYER::sethp(int i)
{
	hp = i;
}

void PLAYER::setadjspd(int i)
{
	adjustspd = i;
}

void PLAYER::setstate(int i)
{
	state = i;
}

void PLAYER::setdir(int i)
{
	dir = i;
}
void PLAYER::setCMD_move(int i)
{
	COMMAND_move = i;
}
void PLAYER::setCMD_hurt(bool i)
{
	COMMAND_hurt = i;
}
void PLAYER::setCMD_ropehurt(bool i)
{
	COMMAND_ropehurt = i;
}
void PLAYER::setCMD_die(bool i)
{
	COMMAND_die = i;
}
void PLAYER::setGamemode(bool i)
{
	Gamemode = i;
}
void PLAYER::setstealth(int i)
{
	stealth = i;
}
void PLAYER::setspike_hurt(int i)
{
	spike_hurt = i;
}
void PLAYER::setBit(HINSTANCE g_hinst)
{
	hbitwalk = LoadWalk(g_hinst);
	hbitex = LoadEx(g_hinst);
}

void PLAYER::initBitPos()
{
	bx = 0;
	by = 0;
	bw = 0;
	bh = 0;
}

//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѼ���
int PLAYER::getx()
{
	return x;
}

int PLAYER::gety()
{
	return y;
}

int PLAYER::getsavey()
{
	return savey;
}

int PLAYER::getw()
{
	return w;
}

int PLAYER::geth()
{
	return h;
}

int PLAYER::getcharw()
{
	return charw;
}

int PLAYER::getcharh()
{
	return charh;
}

int PLAYER::gethp()
{
	return hp;
}

int PLAYER::getadjspd()
{
	return adjustspd;
}

int PLAYER::getstate()
{
	return state;
}
int PLAYER::getdir()
{
	return dir;
}
int PLAYER::getCMD_move()
{
	return COMMAND_move;
}
bool PLAYER::getCMD_hurt()
{
	return COMMAND_hurt;
}
bool PLAYER::getCMD_ropehurt()
{
	return COMMAND_ropehurt;
}
bool PLAYER::getCMD_die()
{
	return COMMAND_die;
}
bool PLAYER::getGamemode()
{
	return Gamemode;
}
int PLAYER::getstealth()
{
	return stealth;
}
int PLAYER::getjumpignore()
{
	return jumpignore;
}
int PLAYER::getspike_hurt()
{
	return spike_hurt;
}
//�÷��̾� ���� ����
void PLAYER::PlayerSetting(WPARAM wParam, Sound& sound)
{
	if (wParam == VK_LEFT)
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
			if (COMMAND_hurt != 1)	//�ĸ°��������� �� ���� �����ؿ�~
				ROWSPEED = 1;	//���������� ������ �ٲٷ��ϸ� ���ƽ�ϰ� �ٽ� ���°��� ������ �׷��� ���ߴ��ͺ��� ���� ����
			dir = 1;	//������ �ٲ������� ���������´� ����
		}
		else if (state == 3)
		{

			h += 12;
			y -= 12;
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
	if (wParam == VK_RIGHT)
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
			if (COMMAND_hurt != 1)	//�ĸ°��������� �� ���� �����ؿ�~
				ROWSPEED = 1;	//���������� ������ �ٲٷ��ϸ� ���ƽ�ϰ� �ٽ� ���°��� ������ �׷��� ���ߴ��ͺ��� ���� ����
			dir = 2;	//������ �ٲ������� ���������´� ����
		}
		else if (state == 3)
		{
			h += 12;
			y -= 12;
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
	if (wParam == VK_UP)
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
	if (wParam == VK_DOWN)
	{
		if (COMMAND_hurt == 1)
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
			h -= 12;		//���̸� Ű�� �پ�����Ѵ�.
			y += 12;
		}

		return;
	}
	if (wParam == VK_SPACE)
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
					jumpignore = 2;	//������ �ٽø���Ե� �ٲ���
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
			savey = y;
		}
		return;
	}
}
//�÷��̾� ������
void PLAYER::PlayerWaiting(WPARAM wParam)
{
	if (wParam == VK_DOWN)
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
				h += 12;
				y -= 12;	//�ٽ� Ű �÷���
				state = 1;
			}
		}

		UDkey = false;
		DOWNkey = false;
		return;
	}
	if (wParam == VK_LEFT)
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
					h -= 12;
					y += 12;//������� ����������
				}
			}
		}


		LRkey = false;				//�Ѱ��� ������ false
		LEFTkey = false;			//LEFTkey ������ false

		//std::cout << "LEFT �E" << std::endl;

		return;
	}
	if (wParam == VK_RIGHT)
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
					h -= 12;
					y += 12;//������� ����������
				}
			}
		}



		LRkey = false;				//�Ѱ��� ������ false
		RIGHTkey = false;			//RIGHTkey ������ false
		//std::cout << "RIGTH ��" << std::endl;

		return;
	}
	if (wParam == VK_UP)
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
void PLAYER::move(int obj_t)
{

	if (state == 1)
	{
		adjustspd = 0;		//�������� �� �Ĵ� 1���°� �ǹǷ� ���⼭ �ʱ�ȭ������
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
		if (COMMAND_hurt == true)	//�ǰݴ��Ѱ��
		{
			if (COMMAND_move == 1)
			{

				x -= ROWSPEED;
			}
			else if (COMMAND_move == 2)
			{
				x += ROWSPEED;
			}
			//y -= 1;
			if (abs(y - savey) > 40) {
				y -= 3;
			}
			else {
				y -= COLSPEED / 2;
			}
			if (abs((y - savey)) >= 40)	//40�ȼ���ŭ �ǰݴ��ؼ� ���� ��¦��
			{
				state = 7;			//�ٽ� ������ ����������
				savex = x;			//�̼����� x��ǥ�� �����(���ӵ��� �޴ٰ� �����ó�� ���ٿ���)
			}
		}
		else if (COMMAND_hurt == false)	//�Ϲݻ���
		{
			if (COMMAND_move == 1)
			{

				x -= ROWSPEED;
			}
			else if (COMMAND_move == 2)
			{
				x += ROWSPEED;
			}
			//y -= 1;
			if (abs(y - savey) > 80) {
				y -= 3;
			}
			else {
				y -= COLSPEED;
			}
			if (abs((y - savey)) >= 100)	//30�ȼ���ŭ �����ߴٸ�
			{
				state = 7;			//�ٽ� ������ ����������
				savex = x;			//�̼����� x��ǥ�� �����(���ӵ��� �޴ٰ� �����ó�� ���ٿ���)
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
			if (obj_t % 5 == 0)
			{
				BitMove();
			}

			if (COMMAND_move == 1)
			{
				x -= ROWSPEED;
			}
			else if (COMMAND_move == 2)
			{
				x += ROWSPEED;
			}
		}
	}
	else if (state == 5)
	{

	savey = y;	//�ٿ��Ŵ޷������� ���ڸ��� ���������̴�
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
		stealth = 100;		//�����ð� 2��
		savey = y;			//�ǰݰ� ���ÿ� y��ǥ����(������ �������� ����)
		COMMAND_hurt = true;	//�ǰ��Լ� on
		state = 2;				//�ǰ��ϸ� �������� �ѹ� �� ���

	}
	else if (state == 7)
	{
		y += COLSPEED;					//�Ʒ��� ������
		if (adjustspd < 1000)			//1000������ �������� ������
			adjustspd++;
		if (LEFTkey == true)			//�������� �� ���� �ڴ�����������
			if (adjustspd % 30 == 0)	//Ÿ�̸Ӱ� 30�� ���ư������� �ѹ��� �Ȱ���
				x -= ROWSPEED;
		if (RIGHTkey == true)
			if (adjustspd % 30 == 0)
				x += ROWSPEED;
		if (COMMAND_move == 1)		//�������� �����̰��ִٸ�
		{
			if (adjustspd <= 10)	//�������� �� ���ٰ�
			{
				x -= ROWSPEED;
			}
			if (adjustspd > 10)		//10�� ���� ������ 2���� �ѹ��� ����
			{
				if (adjustspd % 2 == 0)
					x -= ROWSPEED;
			}
			else if (adjustspd > 30)	//2���� 10�� �� ������ ���� 5���� 1���� ��� ���� �̰� �����ʵ� �Ȱ��� ����
			{
				if (adjustspd % 5 == 0)
					x -= ROWSPEED;
			}

			if (LEFTkey == 0)				//50ĭ������ ��ó�������� ���� , �����Ŀ� ����Ű�� ���������� ������ϰ� �ƴϸ� �������� �� ����
				if (abs(x - savex) > 50)
					COMMAND_move = 0;

		}
		else if (COMMAND_move == 2)
		{
			if (adjustspd <= 10)
			{
				x += ROWSPEED;
			}
			if (adjustspd > 10)
			{
				if (adjustspd % 2 == 0)
					x += ROWSPEED;
			}
			else if (adjustspd > 30)
			{
				if (adjustspd % 5 == 0)
					x += ROWSPEED;
			}
			if (RIGHTkey == 0)
				if (abs(x - savex) > 50)
					COMMAND_move = 0;
		}
	}
	else if (state == 8)
	{
		savey = y;	//�ٿ��Ŵ޷������� ���ڸ��� ���������̴�
		if(obj_t%10==0)	//10�� Ÿ�̸� ���ư��� �ѹ� �����̰����ش�
			BitMove();
		if (UDkey == true)
		{

		}
		else {
			

			if (COMMAND_move == 3)
			{
				y -= ROPESPEED;
			}
			else if (COMMAND_move == 4)
			{
				y += ROPESPEED;
			}
		}
	}
}

//���¿����� ��Ʈ���� �����ϴ� �Լ�
void PLAYER::selectBit()
{
	if (state == 1 || state == 4)
	{
		hbitcur = hbitwalk;
		bw = 62;
		bh = 50;
	}

	else
		hbitcur = hbitex;
}

//��Ʈ���� �ٲ��ִ��Լ� (�ִϸ��̼�)
void PLAYER::BitMove()
{
	bx += 1;//�ε��� �������� �ٲ�
	if (state == 4)
	{
		if (bx >= 5) bx = 1;
	}
	if (state == 5||state==8)
	{

		if (bx >= 2) bx = 0;
	}
}

//�÷��̾ �׷���
void PLAYER::draw(HDC& mem1dc, HDC& pdc)
{
	BLENDFUNCTION bf;
	bf.AlphaFormat = 0;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;



	pdc = CreateCompatibleDC(mem1dc);
	//�ǰݴ������ÿ� ����ó�� ���� dc�� mem1dc�� ����
	HDC gdidc = CreateCompatibleDC(mem1dc);
	//mem1dc�� ĳ���ͱ׸�������ŭ�� ���´�(���� mem1dc���� ����������Ƿ� 0,0 ���� 62,50 ������ ��Ʈ���� ��)
	HBITMAP tmpdc = CreateCompatibleBitmap(mem1dc, 62, 50);
	HBITMAP oldtmpdc = (HBITMAP)SelectObject(gdidc, tmpdc);
	//���⼭ 0,0 ~62,50 ������ ��Ʈ���� ĳ���ͱ������� �ٲ��ش� (�÷��̾ �ִ� ��ġ�� ��Ʈ���� ������)
	BitBlt(gdidc, 0, 0, charw * 2, h * 2, mem1dc, x - charw, y - h, SRCCOPY);
	//�⺻ ������
	SelectObject(pdc, hbitcur);
	if (state == 1) // �������� 
	{

		if (dir == 1)//����
		{
			//TransparentBlt(gdidc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 0, 62, 50, RGB(255, 255, 255));
			//gdidc�� 0,0~ 62,50 �̴ϱ� �� ��ġ�� ������ ĳ���͸� ��������ְ� GdialphaBlend �� ���� ����ȭó�� ���ش�.
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 0, 62, 50, RGB(255, 255, 255));

			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//������
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 50, 62, 50, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 50, 62, 50, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}

	}
	else if (state == 4) //�̵�����
	{
		if (dir == 1)//����
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by, bw, bh, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, bx * 68, by, bw, bh, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//������
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, bx, by + 50, bw, bh, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, bx * 68, by + 50, bw, bh, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}


	}
	else if (state == 2 || state == 7) //�����ϰų� ��������
	{
		if (dir == 1)//����
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 107, 62, 48, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 0, 107, 62, 50, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
		else if (dir == 2)//������
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 107, 62, 48, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 50, pdc, 77, 107, 62, 48, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
		}
	}
	else if (state == 3) //���̱�
	{
		//h�� �ٰ� y�� �ð� 

		BitBlt(gdidc, 0, 0, charw * 2, 26, mem1dc, x - charw, y - h, SRCCOPY);
		if (dir == 1)//����
		{
			//TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 0, 161, 62, 26, RGB(255, 255, 255)); //68 0 130 50
			TransparentBlt(gdidc, 0, 0, 62, 26, pdc, 0, 161, 62, 26, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - 12 - h + 12, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - 12 - h + 12, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
		}
		else if (dir == 2)//������
		{
			//	TransparentBlt(mem1dc, x - charw, y - h, charw * 2, h * 2, pdc, 77, 161, 62, 26, RGB(255, 255, 255));
			TransparentBlt(gdidc, 0, 0, 62, 26, pdc, 77, 161, 62, 26, RGB(255, 255, 255));
			if (stealth > 0)
			{

				bf.SourceConstantAlpha = 155;//����
				//�� �Լ��� �Ϲ� stretchblt �� ����ϴ� gdidc �� �ִ밡 0,0 ~62,50 �̹Ƿ� �� ���ڴ� 0 0 62 50
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
				bf.SourceConstantAlpha = 255;

			}
			else
				GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 26, bf);
		}
	}
	else if (state == 5 || state == 8)	//�ٿ� �Ŵ޸�����
	{
		TransparentBlt(gdidc, 0, 0, 62, 50, pdc, bx * 77, 54, 62, 50, RGB(255, 255, 255));

		if (stealth > 0)
		{

			bf.SourceConstantAlpha = 155;
			GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);
			bf.SourceConstantAlpha = 255;

		}
		else GdiAlphaBlend(mem1dc, x - charw, y - h, charw * 2, h * 2, gdidc, 0, 0, 62, 50, bf);

	}

	SelectObject(gdidc, oldtmpdc);
	DeleteObject(tmpdc);
	DeleteObject(gdidc);
	DeleteObject(pdc);

}

void PLAYER::fall2save()
{
	savex = x;
}

void PLAYER::stealthtime()
{
	if(COMMAND_die==0)	//������ ������Ǯ��
		if (stealth > 0)
		{
			stealth--;
			if (stealth == 0)
				COMMAND_hurt = 0;
		}
	if (jumpignore > 0)
		jumpignore--;
	
}
void PLAYER::spike_hurttime()
{
	if (spike_hurt < 0)
	{
		spike_hurt++;
		x -= 4;			//�������ΰ�
	}
	else if (spike_hurt > 0)
	{
		spike_hurt--;
		x += 4;
	}
}

void PLAYER::hurt(Sound& sound)
{
	if (COMMAND_die == false)
		hp -= 5;
	if (hp <= 0)	//0 ���϶��
	{
		hp = 0;
		COMMAND_die = true;
		COMMAND_move = false;
		state = 3;
		y += 12;
		h = 13;
		stealth = 1;
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