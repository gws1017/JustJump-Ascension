#pragma once
#include <windows.h>

class PLAYER {
	int x, y, w, h;
	int savex, savey;	//savey 는 점프뛸때 그 순간의 y좌표를 기억하기 위함이고 x는 혹시몰라서 넣어둠
	int state;			//1기본상태,2점프상태,3숙이기상태,4이동상태,5줄에 매달린상태,6피격상태,7공중에있는상태
	int dir;			//1왼쪽 2오른쪽 3위 4아래
	int COMMAND_move;	//이게 움직이고있는 상황인지 아닌지 구분 1이면왼쪽으로움직임  2면 오른쪽으로 움직임0이면 안움직임
	HBITMAP hbitcur;    //현재 사용중인 비트맵
	HBITMAP hbitwalk;   //걸을때 비트맵
	HBITMAP hbitex;		//점프,숙이기 줄타기 비트맵
	int bx, by, bw, bh; //비트맵의 스프라이트위치를 바꾸기위한 좌표 변수
public:
	PLAYER();
	void setx(int);
	void sety(int);
	void setw(int);			//세터
	void seth(int);
	//1기본상태,2점프상태,3숙이기상태,4이동상태,5줄에 매달린상태,6피격상태,7공중에있는상태
	void setstate(int);
	//1왼쪽 2오른쪽 3위 4아래
	void setdir(int);
	//이게 움직이고있는 상황인지 아닌지 구분 
	void setCMD_move(int);
	void setBit(HINSTANCE);
	void initBitPos();

	int getx();
	int gety();
	int getw();			//게터
	int geth();
	//1기본상태,2점프상태,3숙이기상태,4이동상태,5줄에 매달린상태,6피격상태,7공중에있는상태
	int getstate();
	//1왼쪽 2오른쪽 3위 4아래
	int getdir();
	//이게 움직이고있는 상황인지 아닌지 구분 
	int getCMD_move();


	void PlayerSetting(WPARAM);
	void PlayerWaiting(WPARAM);
	void fall2save();
	void move();
	void selectBit();	   //플레이어 스프라이트선택
	void BitMove();		   //애니메이션
	void draw(HDC&, HDC&); //플레이어 그림
};


extern int ROWSPEED; 	//가로 이동속도
extern int COLSPEED;	//세로 이동속도
extern bool LEFTkey;//왼쪽키 눌렀는지 1이면 누름 0이면 안누름
extern bool RIGHTkey;//오른쪽키 눌렀는지 1이면 누름 0이면 안누름
extern bool UPkey;	//위쪽키 눌렀는지 1이면 누름 0이면 안누름
extern bool DOWNkey;//아래쪽키 눌렀는지 1이면 누름 0이면 안누름
		//왼쪽오른쪽키 동시에 눌려있는지확인