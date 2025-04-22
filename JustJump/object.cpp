#pragma comment (lib, "Msimg32.lib")
#include "object.h"
#include "Load.h"
#include <iostream>
int OBJECT::getX()
{
	return x;
}
int OBJECT::getY()
{
	return y;
}
int OBJECT::getW()
{
	return w;
}
int OBJECT::getH() {
	return h;
}

int OBJECT::getType()
{
	return type;
}
int OBJECT::getindex()
{
	return index;
}
int OBJECT::getmx()
{
	return mx;
}
int OBJECT::getmy()
{
	return my;
}
void OBJECT::create(int _x, int _y, int _w, int _h, int _type)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	type = _type;
	index = 0;

	mx = 0;
	my = 0;
	
	//row gear
	if (type == 106)
	{
		s = 1;
		dir = 0;
	}
	//col gear
	else if (type == 107)
	{
		s = 1;
		dir = 1;
	}
}
void OBJECT::setX(int _x)
{
	x = _x;
}
void OBJECT::setY(int _y)
{
	y = _y;
}
void OBJECT::setW(int _w)
{
	w = _w;
}
void OBJECT::setH(int _h)
{
	h = _h;
}
void OBJECT::setType(int _type)
{
	type = _type;
}
void OBJECT::setHbit(HINSTANCE g_hinst)
{
	hbit = LoadObj(hbit, g_hinst, type);

}

// All objects reset
void OBJECT::ResetObject()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	type = 0;
	index = 0;
	hbit = NULL;
}
//�ε����� �ٲ��ִ��Լ�
void OBJECT::IndexChange()
{
	index += 1;
	if (type == 0)
	{
		if (index >= 7) index = 0;
	}
	else if (type == 4 || type ==6)
	{
		if (index >= 4) index = 0;
	}
	else if (type == 103) //���� �������� �̹��� 4��
	{
		if (index >= 4) index = 0;
	}
	else if (type == 106 || type == 107) //��Ϲ��� ȸ���ϴ� �̹��� 2��
	{
		if (index >= 2) index = 0;
	}
	else if (type == 201)//��Ż �̹��� 7��
	{
		if (index >= 7) index = 0;
	}
	
}

void OBJECT::DrawObj(HDC& mem1dc, HDC& odc)
{
	odc = CreateCompatibleDC(mem1dc);
	SelectObject(odc, hbit);
	if (type == 1)
	{
	  TransparentBlt(mem1dc, x, y, w , h, odc, 0, 0, 1023, 62, RGB(255, 255, 255));
	}
	else if(type == 0)TransparentBlt(mem1dc, x, y, w, h, odc, 0, 0 + index * 768, 1024, 768, RGB(142, 203, 162));
	else if (type == 2) TransparentBlt(mem1dc, x, y, w, h + 17, odc, 11, 15, 77, 18, RGB(255, 255, 255));	// �����׸����� x 11~88 y 15 33 ��ŭ �߶󳻼� ����ó���� ���
	else if (type == 3) TransparentBlt(mem1dc, x, y, w, h + 18, odc, 0, 0, 19, 19, RGB(255, 255, 255));
	else if (type == 4) TransparentBlt(mem1dc, x, y, w, h + 42, odc, 16 + index * 272, 9, 250, 43, RGB(0, 0, 0));
	else if (type == 6) TransparentBlt(mem1dc, x, y, w, h + 42, odc, 16 + index * 272, 9, 250, 43, RGB(0, 0, 0));
	else if (type == 7)
	{
		//23 50 // 1.73 / 2.123  /3.173 4.223 5.273 6.323 7. 373 350 
		TransparentBlt(mem1dc, x, y, 14, h+17, odc, 11, 15, 14, 18, RGB(255, 255, 255));//head

		
		for (int i = 0; i < 6; i++)
		{
			TransparentBlt(mem1dc, x + 14 + 50 * i, y ,50, h+17, odc, 27, 15, 50, 18, RGB(255, 255, 255));//body
		}
			

	

		TransparentBlt(mem1dc, x + 14 + 300, y , 13, h+17, odc, 78, 15, 13, 18, RGB(255, 255, 255));//tail
	}
	else if (type == 5) TransparentBlt(mem1dc, x, y, w, h , odc, 0, 9, 0, 0, RGB(255, 255, 255));
	else if (type == 101) TransparentBlt(mem1dc, x, y, w+11, h, odc, 1, 0, 26, 15, RGB(255, 255, 255));
	else if (type == 102) TransparentBlt(mem1dc, x, y, w, h, odc, 0, 1, 17, 75, RGB(255, 255, 255));
	else if (type == 103) // 103���� ��� 102���� y������ 51�� �� ��ġ�� ������ �����κ��̴�.
	{
		//�׸��� ũ�Ⱑ ���� �ٸ��⶧���� �ӽú����� �����  ���� �����ϰ� �ε��������� �ٲ��ִ� �����̴�.
		RECT gas[4] = { {0,2,7,7},{10,2,18,7},{31,0,33,9},{0,0,0,0} }; //1��°���ڴ� �ƹ��͵� �ȱ׸��°�
		int tx;
		if (index == 0) tx = x + 26;
		else if (index == 1) tx = x + 15;
		else if (index == 2) tx = x;
		else tx = 0;
		TransparentBlt(mem1dc, tx, y, gas[index].right, gas[index].bottom, odc, gas[index].left, gas[index].top, gas[index].right, gas[index].bottom, RGB(255, 255, 255));
	}
	else if (type == 106 || type == 107) //gear
	{
		TransparentBlt(mem1dc, x+mx , y+my, w, h, odc, index * 23, 4, 18, 18, RGB(255, 255, 255)); // �ε����� only x change
	}
	else if (type == 201) //portal
	{
		TransparentBlt(mem1dc, x, y, w, h, odc, index*79, 55, 63, 135, RGB(0, 0, 0)); // �ε����� only x change
	}
	else if (type == 301)//Rope
	{
		//�Ӹ�+ ���� 53 ���� 41 ��ü 94 �⺻ ���� 94 ����Ŵ� 135 176 217 258 299 340 381 422 463
		int body = (h - 53) / 41;
		TransparentBlt(mem1dc, x, y, w, 32, odc, 0, 0, 24,32, RGB(255, 255, 255));//head
		
		for (int i = 0; i < body; i++)
		{
			TransparentBlt(mem1dc, x, y + 32 + i * 41, w, 41, odc, 0, 33, 24, 41, RGB(255, 255, 255));//body
			
		}
		
		TransparentBlt(mem1dc, x, y + 32 + body * 41, w, 21, odc, 0, 148, 24, 21, RGB(255, 255, 255));//tail
	}
	DeleteObject(odc);
}

void OBJECT::move()
{
	
	if (dir == 0) //Left or Down
	{
		if (mx > 150) s = -1;
		else if (mx < -150) s = 1;
		mx += s * gearrowspeed;
		
	}
	else if (dir == 1) //Up or Down
	{
		if (my > 100) s = -1;
		else if (my < -100) s = 1;
		my += s * gearcolspeed;

	}
	
}
//���ٴڰ� �÷��̾� �浹üũ 1�̸� �δ�ħ
//
//bool collp2w(PLAYER player, OBJECT obj)
//{
//	if (obj.getX() <= player.getx() && player.getx() + player.getw() <= obj.getX() + obj.getW())
//	{
//		if (obj.getY() <= player.gety()+player.geth())
//		{
//			return 1;
//		}
//	}
//	return 0;
//	/*if (player.getx() + player.getw() < obj.getX() || obj.getX() + obj.getW() < player.getx()) return 0;
//	if (player.gety() + player.geth() < obj.getY() || obj.getY() + obj.getH() < player.gety())return 0;
//
//	return 1;
//*/
//
//}