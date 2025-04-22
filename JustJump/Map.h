#pragma once
#include <windows.h>
#include "Camera.h"
#include "player.h"
#define MAPWIDTH 1024
#define MAPHEIGHT 4098
#define MAPSPEED 5
class MAP {
	int mapnum = 9;
	int black_t = 0;
	int ms;
	int index;
	HBITMAP hbitbk;
	HBITMAP hbitui;
	HBITMAP hbithp;
	HBITMAP hbitdie;
	HBITMAP hbitstart[3];
	HBITMAP hbithelp[2];
public:
	//�ʹ�ȣ�� ���� 10~
	int getmapnum();
	//Black Screen Time get
	int getblack_t();

	//�ʹ�ȣ�� �ٲ�
	void setmapnum(int);
	//Black Screen Time set
	void setblack_t(int);

	//For 2 sec Screen hide
	bool BlackTime();
	//�� ����� ������
	void movemap();
	void DrawBK(HDC&, HDC&, RECT&);
	//������ �Ÿ����� �� ����
	void DrawLoadBK(HDC&, HDC&, BLENDFUNCTION);
	//����â
	void DrawUi(HDC&, HDC&, CAMERA);
	//HP��
	void DrawHP(HDC&, HDC&, CAMERA,PLAYER);
	//die ui
	void DrawDie(HDC&, HDC&, CAMERA, Sound&);
	//���� ��ư
	void DrawStart(HDC&, HDC&, int);
	//���۹�
	void DrawHelp(HDC&, HDC&,int);
	//die ���º�ȭ
	void ChangeDieNotice(HINSTANCE,int);
	/*void CreateBlack(HINSTANCE)*/;
	void CreateMap(HINSTANCE);
	void CreateUi(HINSTANCE);
	void CreateHP(HINSTANCE);
	void CreateDie(HINSTANCE);
	void CreateStart(HINSTANCE);
	void CreateHelp(HINSTANCE);
};
