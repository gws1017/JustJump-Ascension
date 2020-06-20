#pragma once
#include <Windows.h>

//HBITMAP LoadBlack(HBITMAP, HINSTANCE);
HBITMAP LoadBK(HBITMAP, HINSTANCE);
//상태 Ui표시
HBITMAP LoadUi(HBITMAP, HINSTANCE);
//HP Ui 표시
HBITMAP LoadHP(HBITMAP, HINSTANCE);

HBITMAP LoadObj(HBITMAP, HINSTANCE, int);

HBITMAP LoadWalk(HINSTANCE);

HBITMAP LoadEx(HINSTANCE);
//죽으면 뜨는 상태창
HBITMAP LoadDieNotice(HINSTANCE);
//클릭,확인에 손 올라갈때 변화
HBITMAP LoadDieNoticeChange(HINSTANCE, int);