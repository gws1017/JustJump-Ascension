#pragma once
#include <windows.h>
#include "player.h"

#define beltspeed 2 //belt speed
#define gearrowspeed 5 //row gear speed
#define gearcolspeed 3 // col gear speed
class OBJECT {
	int x, y;					//������Ʈ�� x y �� ���ʻ���� �����̴�.
	int w, h;
	int type;					//1.�ٴ� 2.�⺻ ���� 3.�������� 4.�����̾� ��Ʈ 5.����ٴ� 101.����� 102.���������� 103.���� 106,107.��Ϲ��� 201.��Ż 301.����
	int index;					//�ִϸ��̼� ������ �迭�� �ε����� �ٲ��־� �̹����� �ٲ۴�
	int mx, my;					//Variables that increase/decrease based on the object's x,y coordinates
	int s;						//sign bit
	int dir;					//object's moving direction 0 == left , right / 1 == up , down
	HBITMAP hbit;				
public:
	//������Ʈ�� x y �� ���ʻ���� �����̴�.
	int getX();
	int getY();
	int getW();
	int getH();
	int getindex();

	//1.�ٴ� 2.�⺻ ���� 3.�������� 4.�����̾� ��Ʈ 5.����ٴ� 101.����� 102.���������� 103.���� 106,107.��Ϲ��� 201.��Ż 301.����
	int getType();
	//������Ʈ�� �������
	int getocount();

	int getmx();
	int getmy();

	void create(int, int, int, int, int);
	//������Ʈ�� x y �� ���ʻ���� �����̴�.
	void setX(int);
	//������Ʈ�� x y �� ���ʻ���� �����̴�.
	void setY(int);
	void setW(int);
	void setH(int);
	//1~100 �÷��� 101~��ֹ�
	void setType(int);
	void setHbit(HINSTANCE);

	// All objects reset
	void ResetObject();
	//Move gear object
	void move();
	//������Ʈ �ִϸ��̼� ���� ����ϴ��Լ�
	void IndexChange();
	//������Ʈ�� �׷���
	void DrawObj(HDC&, HDC&);
};

//bool collp2w(PLAYER, OBJECT);