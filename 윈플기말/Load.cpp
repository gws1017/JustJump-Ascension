#include "Load.h"
#include "resource.h"
#include <windows.h>
#include <tchar.h>

//HBITMAP LoadBlack(HBITMAP hbit1, HINSTANCE g_hinst)
//{
//	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk_black.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
//	return hbit1;
//
//}

HBITMAP LoadBK(HBITMAP hbit1, HINSTANCE g_hinst)
{
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit1;

}
//상태 Ui표시
HBITMAP LoadUi(HBITMAP hbit, HINSTANCE g_hinst)
{
	hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/Ui.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit;

}
//HP Ui 표시
HBITMAP LoadHP(HBITMAP hbit, HINSTANCE g_hinst)
{
	hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/Ui_HP.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit;

}
//사망 Ui
HBITMAP LoadDieNotice(HINSTANCE g_hinst)
{
	HBITMAP hbit;
	hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/notice3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit;
}
//사망 Ui 상태변화
HBITMAP LoadDieNoticeChange(HINSTANCE g_hinst,int i)
{
	HBITMAP hbit=NULL;
	if(i==0)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/notice3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	else if (i == 1)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/notice4.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	else if (i == 2)
		hbit = (HBITMAP)LoadImage(g_hinst, TEXT("img/notice5.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit;
}


HBITMAP LoadObj(HBITMAP hbit1, HINSTANCE g_hinst, int type) //오브젝트의 이미지파일을 불러옴
{
	if (type == 1) // 바닥
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if(type == 2) // 발판
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	else if(type == 3)
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 4) //컨베이어 벨트
		hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/belt.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if(type==101)
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/nasa.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 102)
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/breakpipe1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if(type == 103)
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/gas.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 106 || type == 107)
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/gear.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 201)
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/Portal_sprite.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if (type == 301)
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/breakpipe1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	return hbit1;
}

HBITMAP LoadWalk(HINSTANCE g_hinst) // 캐릭터 이미지를 불러옴 좌우 걷기
{
	HBITMAP hbit1;
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/char_sprite.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	return hbit1;
}
HBITMAP LoadEx(HINSTANCE g_hinst) // 캐릭터 이미지를 불러옴 숙이기 점프 줄타기
{
	HBITMAP hbit1;
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/char_sprite2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	return hbit1;
}