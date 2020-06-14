#pragma comment (lib, "Msimg32.lib")
#include "player.h"
#include "Load.h"
#include <iostream>
int ROWSPEED = 3; 	//가로 이동속도
int COLSPEED = 10;	//세로 이동속도
bool LEFTkey = 0;//왼쪽키 눌렀는지 1이면 누름 0이면 안누름
bool RIGHTkey = 0;//오른쪽키 눌렀는지 1이면 누름 0이면 안누름
bool UPkey = 0;	//위쪽키 눌렀는지 1이면 누름 0이면 안누름
bool DOWNkey = 0;//아래쪽키 눌렀는지 1이면 누름 0이면 안누름
bool LRkey = 0;//왼쪽오른쪽키 동시에 눌렀는지 1이면눌림 0이면 안눌림
PLAYER::PLAYER()
{
	x = 80; //100
	y = 3800; //3800
	w = 62; //숫자를 키웠더니 충돌체크가 제대로 안됨 고쳐주셈!
	h = 50;
	state = 7;
	dir = 2;
	COMMAND_move = false;

}
void PLAYER::setx(int i)
{
	x = i;
}

void PLAYER::sety(int i)
{
	y = i;
}

void PLAYER::setw(int i)
{
	w = i;
}

void PLAYER::seth(int i)
{
	h = i;
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

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ세터
int PLAYER::getx()
{
	return x;
}

int PLAYER::gety()
{
	return y;
}

int PLAYER::getw()
{
	return w;
}

int PLAYER::geth()
{
	return h;
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


//플레이어 상태 변경
void PLAYER::PlayerSetting(WPARAM wParam)
{
	if (wParam == VK_LEFT)
	{
		LEFTkey = true;				//키 누름상태
		if (RIGHTkey == true)		//좌우를 동시에 누르고있다면 움직이지않음
		{
			LRkey = true;
			return;
		}
		if (state == 7)
		{
			dir = 1;
		}
		else {
			COMMAND_move = 1;			//한쪽키만누르고있다면 움직여줌(하지만 떨어지는 상태에선 변하지않음)
		}
		if (state == 1 || state == 4)
		{
			state = 4;
			dir = 1;
			std::cout << "LEFT눌림" << std::endl;
		}
		else if (state == 2)
		{
			ROWSPEED = 1;
			dir = 1;
		}
		return;
	}
	if (wParam == VK_RIGHT)
	{
		RIGHTkey = true;
		if (LEFTkey == true)
		{
			LRkey = true;
			return;
		}
		if (state == 7)
		{
			dir = 1;
		}
		else {
			COMMAND_move = 2;			//한쪽키만누르고있다면 움직여줌(하지만 떨어지는 상태에선 변하지않음)
		}
		if (state == 1 || state == 4)
		{
			state = 4;
			dir = 2;
			std::cout << "RIGHT 눌림" << std::endl;
		}
		else if (state == 2)
		{
			ROWSPEED = 1;
		}

		return;
	}
	if (wParam == VK_UP)
	{
		UPkey = true;
		if (state == 5)
		{
			dir = 3;
		}
		//이부분에 사다리와의 충돌 체크를 하고 true면 줄에 매달리는 상태로 바꿔줄예정
		return;
	}
	if (wParam == VK_DOWN)
	{
		DOWNkey = true;
		if (state == 5)
		{
			dir = 4;
		}
		else if (state == 1) {
			state = 3;	//숙이는거는 가만히 있을때만 가능하다
		}
		return;
	}
	if (wParam == VK_SPACE)
	{
		if (state != 2 && state != 7)
		{
			state = 2;
			savey = y;
		}
		return;
	}
}
//플레이어 대기상태
void PLAYER::PlayerWaiting(WPARAM wParam)
{
	if (wParam == VK_DOWN)
	{
		DOWNkey = false;
		if (state == 3)
			state = 1;
		return;
	}
	if (wParam == VK_LEFT)
	{
		LRkey = false;
		LEFTkey = false;
		if (state != 7)				//떨어지고 있을때는 이동로직이 작동하면 안된다 무시해줌
		{
			if (RIGHTkey == true)	//키가 동시에 눌리고있었다면 땔때 옮겨줌
			{
				COMMAND_move = 2;
				return;
			}
		}
		if (state == 4)		//움직이고있을땐 멈춰 점프뛰고있을땐 계쏙 점프뛰는상태 유지 개입 x
			state = 1;
		if (state != 7)	//떨어질땐 진행방향쪽으로 계속 떨어져야한다
			COMMAND_move = false;
		std::cout << "LEFT 똄" << std::endl;

		return;
	}
	if (wParam == VK_RIGHT)
	{
		LRkey = false;
		RIGHTkey = false;
		if (state != 7)				//떨어지고 있을때는 이동로직이 작동하면 안된다 무시해줌
		{
			if (LEFTkey == true)
			{
				COMMAND_move = 1;
				return;
			}
		}
		if (state == 4)		//움직이고있을땐 멈춰 점프뛰고있을땐 계쏙 점프뛰는상태 유지 개입 x
			state = 1;
		if (state != 7)	//떨어질땐 진행방향으로 계속 떨어짐
			COMMAND_move = false;
		std::cout << "RIGTH 뗌" << std::endl;

		return;
	}
	if (wParam == VK_UP)
	{
		UPkey = false;
		return;
	}

}


//플레이어 움직임
void PLAYER::move()
{
	static int adjustspd = 0;	//떨어질때 x값을 천천히 이동시켜주기 위함

	if (state == 1)
	{
		adjustspd = 0;		//수직낙하 한 후는 1상태가 되므로 여기서 초기화시켜줌
		if (LRkey == true)
		{

		}
		else {
			if (COMMAND_move == 1)
			{
				x -= ROWSPEED;
			}
			else if (COMMAND_move == 2)
			{
				x += ROWSPEED;
			}
		}
		//가만히있을때는 움직일순있ㅇ므
	}
	else if (state == 2)	//점프상태일때도 진행방향으로 이동은해야함 
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
		if (abs((y - savey)) >= 100)	//30픽셀만큼 점프했다면
		{
			state = 7;			//다시 땅으로 떨어지게함
			savex = x;			//이순간의 x좌표를 기억함(가속도를 받다가 멈춘것처럼 해줄예정)
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
		if (dir == 3)
		{
			y -= 1;
		}
		else if (dir == 4)
		{
			y += 1;
		}
	}
	else if (state == 7)
	{
		y += COLSPEED;
		if (adjustspd < 1000)
			adjustspd++;
		if (LEFTkey == true)
			if (adjustspd % 30 == 0)
				x -= ROWSPEED;
		if (RIGHTkey == true)
			if (adjustspd % 30 == 0)
				x += ROWSPEED;
		if (COMMAND_move == 1)
		{
			if (adjustspd <= 10)
			{
				x -= ROWSPEED;
			}
			if (adjustspd > 10)
			{
				if (adjustspd % 2 == 0)
					x -= ROWSPEED;
			}
			else if (adjustspd > 30)
			{
				if (adjustspd % 5 == 0)
					x -= ROWSPEED;
			}

			if (LEFTkey == 0)
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
}

//상태에따라 비트맵을 선택하는 함수
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

//비트맵을 바꿔주는함수 (애니메이션)
void PLAYER::BitMove()
{
	if (state == 4)
	{
		bx += 68;
		if (bx > 271) bx = 0;
	}

}

//플레이어를 그려줌
void PLAYER::draw(HDC& mem1dc, HDC& pdc)
{
	pdc = CreateCompatibleDC(mem1dc);
	//기본 움직임
	SelectObject(pdc, hbitcur);

	if (state == 1) // 정지상태 
	{

		if (dir == 1)//왼쪽
		{
			TransparentBlt(mem1dc, x, y, w, h, pdc, 0, 0, 62, 50, RGB(255, 255, 255));
		}
		else if (dir == 2)//오른쪽
		{
			TransparentBlt(mem1dc, x, y, w, h, pdc, 0, 50, 62, 50, RGB(255, 255, 255));
		}

	}
	else if (state == 4) //이동상태
	{
		if (dir == 1)//왼쪽
		{
			TransparentBlt(mem1dc, x, y, w, h, pdc, bx, by, bw, bh, RGB(255, 255, 255)); //68 0 130 50
		}
		else if (dir == 2)//오른쪽
		{
			TransparentBlt(mem1dc, x, y, w, h, pdc, bx, by + 50, bw, bh, RGB(255, 255, 255));
		}


	}
	else if (state == 2 || state == 7) //점프하거나 떨어질때
	{
		if (dir == 1)//왼쪽
		{
			TransparentBlt(mem1dc, x, y, w, h, pdc, 0, 107, 62, 48, RGB(255, 255, 255)); //68 0 130 50
		}
		else if (dir == 2)//오른쪽
		{
			TransparentBlt(mem1dc, x, y, w, h, pdc, 77, 107, 62, 48, RGB(255, 255, 255));
		}
	}
	else if (state == 3)
	{
		if (dir == 1)//왼쪽
		{
			TransparentBlt(mem1dc, x, y+26, w,26, pdc, 0, 161, 62, 26, RGB(255, 255, 255)); //68 0 130 50
		}
		else if (dir == 2)//오른쪽
		{
			TransparentBlt(mem1dc, x, y+26, w,26, pdc, 77, 161, 62, 26, RGB(255, 255, 255));
		}
	}
	DeleteObject(pdc);


}


void PLAYER::fall2save()
{
	savex = x;
}