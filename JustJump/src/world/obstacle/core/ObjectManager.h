#pragma once

#include "object/character/player.h"
#include "world/obstacle/object.h"
#include "object/view/Camera.h"
#include "world/Map.h"
using namespace std;
//땅바닥과 플레이어 충돌체크 1이면 부닥침
bool collp2w(PLAYER, OBJECT);
//플레이어와 오브젝트간 상호작용 판단하고 그에맞게 바꿔줌
void adjustPlayer(PLAYER&, OBJECT*, MAP&, int&, HINSTANCE,Sound&);

//int(맵 번호) 에 따라 장애물 위치값 넣어주고 몇개의 오브젝트가 들어갔는지 알려주는 함수
int initObject(OBJECT*, int, HINSTANCE);
//카메라 무빙워크
void adjustCamera(CAMERA&,PLAYER);