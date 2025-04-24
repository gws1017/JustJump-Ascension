#pragma once
#include <Windows.h>
#include"system/Sound.h"

class Sound;

// �ʹ� �����ʳ�
// hurt bool ���ִµ� �ֶ� ������Ʈ������
// crouch�϶� ��Ʈ�� �����ִ� ������ �Լ� ������������
enum class EPlayerState
{
	Idle = 1,
	Jump,
	Crouch,
	Move,
	RopeIdle,
	Hurt,
	Airborne,
	RopeMove
};
enum class EPlayerDirection 
{
	Left = 1,
	Right,
	Up,
	Down
};
enum class EMoveCommand {
	None = 0,
	Left = 1,
	Right = 2,
	Up = 3,
	Down = 4
};

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
	void SetState(EPlayerState state) { PlayerState = state; }
	
	//bool���� �ʹ������ʳ�
	void SetDirection(EPlayerDirection dir) { PlayerDirection = dir; }
	void SetMoveCommand(EMoveCommand command) { MoveCommand = command; }
	void SetHurt(bool value) { bIsHurt = value; }
	void SetRopeHurt(bool value) { bIsRopHurt = value; }
	void SetDead(bool value) { bIsDead = value; }

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
	EPlayerState GetState() { return PlayerState; }

	EPlayerDirection GetDirection() { return PlayerDirection; }
	EMoveCommand GetMoveCommand() { return MoveCommand; }
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
	void SelectBitmap();
	//�ִϸ��̼�
	void PlayAnim();
	//�÷��̾� �׸�
	void Render(HDC& backDC, HDC& playerDC);
	//�ǰݽ� �����ð�
	void UpdateInvincibilityTimer();
	//�������� ������� �����̵���
	void UpdateSpikeKnockback();
	//�������� �� �ٴ� ���� + �������ʱ�ȭ
	void TakeDamage(Sound& sound);
	//������ �ʱ�ȭ
	void Die(Sound& sound);

private:

	void HandleLeftPressed();
	void HandleRightPressed();
	void HandleUpPressed();
	void HandleDownPressed();
	void HandleSpacePressed(Sound& sound);

	void HandleLeftReleased();
	void HandleRightReleased();
	void HandleUpReleased();
	void HandleDownReleased();

private:
	int PositionX, PositionY, HalfWidth, HalfHeight;		// x y �� ĳ������ �߽���ǥ�̰� w,h �� xy���� �¿�� ��ƴ���� �� ��ǥ�̴�.
	int SpriteWidth, SpriteHeight;	//ĳ���� ��ü �������̴�. 
	int SavedX, SavedY;	//savey �� �����۶� �� ������ y��ǥ�� ����ϱ� �����̰� x�� Ȥ�ø��� �־��

	int CurrentHP;				//�÷��̾��� hp�̴�.
	EPlayerDirection PlayerDirection;			//1���� 2������ 3�� 4�Ʒ� �÷��̾ �����ִ�'����'
	EPlayerState PlayerState;			//1�⺻����,2��������,3���̱����,4�̵�����,5�� ����,6�ǰݻ���,7���߿��ִ»��� 8 ���̵�
	int FallAdjustSpeed;		//�������� x�� õõ�� �̵������ֱ� ���� ����

	EMoveCommand MoveCommand;	//�̰� �����̰��ִ� ��Ȳ���� �ƴ��� ���� 1�̸�������ο�����  2�� ���������� ������0�̸� �ȿ����� 3�̸� ���ο����� 4��Ʒ��ο�����
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