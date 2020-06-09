#include "Load.h"
#include "resource.h"
#include <windows.h>
#include <tchar.h>

HBITMAP LoadBK(HBITMAP hbit1, HINSTANCE g_hinst)
{
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/bk.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	return hbit1;

}
HBITMAP LoadObj(HBITMAP hbit1, HINSTANCE g_hinst, int type) //오브젝트의 이미지파일을 불러옴
{
	if (type == 1) // 바닥
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	else if(type == 2) // 발판
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //상대경로로 변경
	else if(type == 3)
	hbit1 = (HBITMAP)LoadImage(g_hinst, TEXT("img/foothold3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
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