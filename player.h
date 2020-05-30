#pragma once




class PLAYER {
	int x, y, w, h;
	int state;			//1기본상태,2점프상태,3숙이기상태,4이동상태
public:
	PLAYER() {};
	void setx(int);
	void sety(int);
	void setw(int);			//세터
	void seth(int);
	//1기본상태,2점프상태,3숙이기상태,4이동상태
	void setstate(int);

	int getx();
	int gety();
	int getw();			//게터
	int geth();
	//1기본상태,2점프상태,3숙이기상태,4이동상태
	int getstate();


};
