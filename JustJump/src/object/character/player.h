#pragma once
#include <Windows.h>
#include"system/Sound.h"

class Sound;

class PLAYER {

public:
	PLAYER();

public:
	// 캐릭터위치 초기화 함수
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

	//방향키 눌렀을때 플레이어 세팅
	void OnKeyPressed(WPARAM key, Sound& sound);
	//방향키 땠을때 플레이어 세팅풀기
	void OnKeyReleased(WPARAM key);
	//떨어지는 시점의 y좌표 기억
	void SavePositionX() {	SavedX = PositionX;	}
	//플레이어 무브
	void UpdateMovement(int delta_time);
	//플레이어 스프라이트선택
	void selectBit();
	//애니메이션
	void BitMove();
	//플레이어 그림
	void Render(HDC& backDC, HDC& playerDC);
	//피격시 무적시간
	void UpdateInvincibilityTimer();
	//가시한테 찔렸을때 강제이동값
	void UpdateSpikeKnockback();
	//다쳤을때 피 다는 계산식 + 뒤지면초기화
	void TakeDamage(Sound& sound);
	//뒤지면 초기화
	void Die();

private:
	int PositionX, PositionY, HalfWidth, HalfHeight;		// x y 는 캐릭터의 중심좌표이고 w,h 는 xy에서 좌우로 반틈씩만 간 좌표이다.
	int SpriteWidth, SpriteHeight;	//캐릭터 전체 사이즈이다. 
	int SavedX, SavedY;	//savey 는 점프뛸때 그 순간의 y좌표를 기억하기 위함이고 x는 혹시몰라서 넣어둠

	int CurrentHP;				//플레이어의 hp이다.
	int dir;			//1왼쪽 2오른쪽 3위 4아래 플레이어가 보고있는'방향'
	int state;			//1기본상태,2점프상태,3숙이기상태,4이동상태,5줄 정지,6피격상태,7공중에있는상태 8 줄이동
	int FallAdjustSpeed;		//떨어질때 x값 천천히 이동시켜주기 위한 변수

	int COMMAND_move;	//이게 움직이고있는 상황인지 아닌지 구분 1이면왼쪽으로움직임  2면 오른쪽으로 움직임0이면 안움직임 3이면 위로움직임 4면아래로움직임
	bool bIsHurt;	//쳐맞으면 1 아니면 0 점프할때 로직에 들어감
	bool bIsRopHurt;	//로프에서 쳐맞으면 1 아니면 0
	bool bIsDead;	//죽으면 1 아니면 0
	bool IsGameMode;		//0이면 일반 1이면 플라잉모드

	int InvincibleTime;		//피격시 무적 2초를 기준으로한다.
	int RopeJumpCooldown;		//줄에서 점프시 줄 바로 못잡게한다. stealth 와 같이 돌아갈예정
	int SpikeKnockback;		//하강중 가시로인한 강제적 좌표이동값 -이면 왼쪽으로가야함 + 이면 오른쪽으로 강제이동

	HBITMAP CurrentBitmap;    //현재 사용중인 비트맵
	HBITMAP WalkBitmap;   //걸을때 비트맵
	HBITMAP ActionBitmap;		//점프,숙이기 줄타기 비트맵
	int AnimX, AnimY, AnimWidth, AnimHeight; //비트맵의 스프라이트위치를 바꾸기위한 좌표 변수
};


extern int ROWSPEED; 	//가로 이동속도
extern int COLSPEED;	//세로 이동속도
extern bool LEFTkey;//왼쪽키 눌렀는지 1이면 누름 0이면 안누름
extern bool RIGHTkey;//오른쪽키 눌렀는지 1이면 누름 0이면 안누름
extern bool UPkey;	//위쪽키 눌렀는지 1이면 누름 0이면 안누름
extern bool DOWNkey;//아래쪽키 눌렀는지 1이면 누름 0이면 안누름
extern int jumpcount;
extern int diecount;
//왼쪽오른쪽키 동시에 눌려있는지확인