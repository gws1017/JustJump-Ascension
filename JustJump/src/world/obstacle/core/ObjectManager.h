#pragma once

#include "object/character/player.h"
#include "world/obstacle/object.h"
#include "object/view/Camera.h"
#include "world/Map.h"
using namespace std;
//���ٴڰ� �÷��̾� �浹üũ 1�̸� �δ�ħ
bool collp2w(PLAYER, OBJECT);
//�÷��̾�� ������Ʈ�� ��ȣ�ۿ� �Ǵ��ϰ� �׿��°� �ٲ���
void adjustPlayer(PLAYER&, OBJECT*, MAP&, int&, HINSTANCE,Sound&);

//int(�� ��ȣ) �� ���� ��ֹ� ��ġ�� �־��ְ� ��� ������Ʈ�� ������ �˷��ִ� �Լ�
int initObject(OBJECT*, int, HINSTANCE);
//ī�޶� ������ũ
void adjustCamera(CAMERA&,PLAYER);