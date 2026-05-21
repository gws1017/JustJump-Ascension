#pragma once


#define MAPWIDTH 1024
#define MAPHEIGHT 4098
#define MAPSPEED 5

class CAMERA;
class PLAYER;
class Sound;

class MAP
{

public:
	//맵번호를 읽음 10~
	int GetMapNumber() { return mapnum; }
	//Black Screen Time get
	int GetBlackTime() { return black_t; }

	//맵번호를 바꿈
	void SetMapNumber(int i) { mapnum = i; }
	//Black Screen Time set
	void SetBlackTime(int i) { black_t = i; }

public:

	void CreateMap(HINSTANCE g_hinst);
	void CreateUi(HINSTANCE g_hinst);
	void CreateHP(HINSTANCE g_hinst);
	void CreateDie(HINSTANCE g_hinst);
	void CreateStart(HINSTANCE g_hinst);
	void CreateHelp(HINSTANCE g_hinst);

	void ChangeDieNotice(HINSTANCE g_hinst, int i);

	//로딩 타임 확인
	bool BlackTime();
	//맵 배경 이동
	void movemap();
	void DrawBK(HDC& mem1dc, HDC& mem2dc, RECT& rectview ,const PLAYER& player);
	//로딩화면 알파블렌딩
	void DrawLoadBK(HDC& mem1dc, HDC& mem2dc, BLENDFUNCTION bf);
	//상태창
	void DrawUi(HDC& mem1dc, HDC& mem2dc, CAMERA camera);
	//HP바
	void DrawHP(HDC& mem1dc, HDC& mem2dc, CAMERA camera,PLAYER player);
	//die ui
	void DrawDie(HDC& mem1dc, HDC& mem2dc, CAMERA camera, Sound& sound);
	//시작 버튼
	void DrawStart(HDC& mem1dc, HDC& mem2dc, int i);
	//조작법
	void DrawHelp(HDC& mem1dc, HDC& mem2dc,int i);
	//die 상태변화

	/*void CreateBlack(HINSTANCE)*/;

private:

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

};
