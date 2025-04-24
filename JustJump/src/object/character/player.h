#pragma once
#include <Windows.h>
#include"system/Sound.h"

class Sound;

class PLAYER {

public:
	PLAYER();

public:
	// ĳ������ġ �ʱ�ȭ �Լ�
	void Initialzie();


	void SetPositionX(int x) { PositionX = x; }
	void SetPositionY(int y) { PositionY = y; }
	void SetHalfWidth(int w) { HalfWidth = w; }
	void SetHalfHeight(int h) { HalfHeight = h; }
	void SetSpriteWidth(int w) { SpriteWidth = w; }
	void SetSpriteHeight(int h) { SpriteHeight = h; }

	void SetSaveY(int y) { SavedY = y; }
	void SetCurrentHP(int hp) { CurrentHP = hp; }
	void SetFallSpeed(int speed) { FallAdjustSpeed = speed; }
	void SetState(int state_) { state = state_; }

	void SetDirection(int dir_) { dir = dir_; }
	void SetCMDMove(int value) { COMMAND_move = value; }
	void SetCMDHurt(bool value) { bIsHurt = value; }
	void SetCMDRopeHurt(bool value) { bIsRopHurt = value; }
	void SetCMDDie(bool value) { bIsDead = value; }

	void SetGameMode(bool value) { IsGameMode = value; }
	void SetInvicible(int value) { InvincibleTime = value; }
	void SetSpikeHurt(int value) { SpikeKnockback = value; }

	void SetBitMap(HINSTANCE hInstance);
	void InitializeAnimPosition();

	int GetPositionX() { return PositionX; }
	int GetPositionY() { return PositionY; }
	int GetSaveY() { return SavedY; }
	int GetHalfWidth() { return HalfWidth; }
	int GetHalfHeight() { return HalfHeight; }
	int GetSpriteWidth() { return SpriteWidth; }
	int GetSpriteHeight() { return SpriteHeight; }

	int GetCurrentHP() { return CurrentHP; }
	int GetFallSpeed() { return FallAdjustSpeed; }
	int GetState() { return state; }

	int GetDirection() { return dir; }
	int GetCMDMove() { return COMMAND_move; }
	bool IsHurt() { return bIsHurt; }
	bool IsRopeHurt() { return bIsRopHurt; }
	bool IsDead() { return bIsDead; }
	bool GetGameMode() { return IsGameMode; }

	int GetInvincibleTime() { return InvincibleTime; }
	int GetJumpCooldown() { return RopeJumpCooldown; }
	int GetSpikeKnockback() { return SpikeKnockback; }

	//����Ű �������� �÷��̾� ����
	void OnKeyPressed(WPARAM key, Sound& sound);
	//����Ű ������ �÷��̾� ����Ǯ��
	void OnKeyReleased(WPARAM key);
	//�������� ������ y��ǥ ���
	void SavePositionX() {	SavedX = PositionX;	}
	//�÷��̾� ����
	void UpdateMovement(int delta_time);
	//�÷��̾� ��������Ʈ����
	void selectBit();
	//�ִϸ��̼�
	void BitMove();
	//�÷��̾� �׸�
	void Render(HDC& backDC, HDC& playerDC);
	//�ǰݽ� �����ð�
	void UpdateInvincibilityTimer();
	//�������� ������� �����̵���
	void UpdateSpikeKnockback();
	//�������� �� �ٴ� ���� + �������ʱ�ȭ
	void TakeDamage(Sound& sound);
	//������ �ʱ�ȭ
	void Die();

private:
	int PositionX, PositionY, HalfWidth, HalfHeight;		// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�.
	int SpriteWidth, SpriteHeight;	//ĳ���� ��ü �������̴�. 
	int SavedX, SavedY;	//savey �� �����۶� �� ������ y��ǥ�� ����ϱ� �����̰� x�� Ȥ�ø��� �־��

	int CurrentHP;				//�÷��̾��� hp�̴�.
	int dir;			//1���� 2������ 3�� 4�Ʒ� �÷��̾ �����ִ�'����'
	int state;			//1�⺻����,2��������,3���̱����,4�̵�����,5�� ����,6�ǰݻ���,7���߿��ִ»��� 8 ���̵�
	int FallAdjustSpeed;		//�������� x�� õõ�� �̵������ֱ� ���� ����

	int COMMAND_move;	//�̰� �����̰��ִ� ��Ȳ���� �ƴ��� ���� 1�̸�������ο�����  2�� ���������� ������0�̸� �ȿ����� 3�̸� ���ο����� 4��Ʒ��ο�����
	bool bIsHurt;	//�ĸ����� 1 �ƴϸ� 0 �����Ҷ� ������ ��
	bool bIsRopHurt;	//�������� �ĸ����� 1 �ƴϸ� 0
	bool bIsDead;	//������ 1 �ƴϸ� 0
	bool IsGameMode;		//0�̸� �Ϲ� 1�̸� �ö��׸��

	int InvincibleTime;		//�ǰݽ� ���� 2�ʸ� ���������Ѵ�.
	int RopeJumpCooldown;		//�ٿ��� ������ �� �ٷ� ������Ѵ�. stealth �� ���� ���ư�����
	int SpikeKnockback;		//�ϰ��� ���÷����� ������ ��ǥ�̵��� -�̸� �������ΰ����� + �̸� ���������� �����̵�

	HBITMAP CurrentBitmap;    //���� ������� ��Ʈ��
	HBITMAP WalkBitmap;   //������ ��Ʈ��
	HBITMAP ActionBitmap;		//����,���̱� ��Ÿ�� ��Ʈ��
	int AnimX, AnimY, AnimWidth, AnimHeight; //��Ʈ���� ��������Ʈ��ġ�� �ٲٱ����� ��ǥ ����
};


extern int ROWSPEED; 	//���� �̵��ӵ�
extern int COLSPEED;	//���� �̵��ӵ�
extern bool LEFTkey;//����Ű �������� 1�̸� ���� 0�̸� �ȴ���
extern bool RIGHTkey;//������Ű �������� 1�̸� ���� 0�̸� �ȴ���
extern bool UPkey;	//����Ű �������� 1�̸� ���� 0�̸� �ȴ���
extern bool DOWNkey;//�Ʒ���Ű �������� 1�̸� ���� 0�̸� �ȴ���
extern int jumpcount;
extern int diecount;
//���ʿ�����Ű ���ÿ� �����ִ���Ȯ��