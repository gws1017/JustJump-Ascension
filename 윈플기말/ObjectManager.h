#pragma once
#include "player.h"
#include "object.h"

//땅바닥과 플레이어 충돌체크 1이면 부닥침
bool collp2w(PLAYER, OBJECT);
//플레이어와 오브젝트간 상호작용 판단하고 그에맞게 바꿔줌
void adjustPlayer(PLAYER&, OBJECT*,int);