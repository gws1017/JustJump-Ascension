#pragma once
#include "object/core/Object.h"
class Sound;

// 너무 많지않나
// hurt bool 이있는데 왜또 스테이트가있지
// crouch일때 비트맵 셋해주는 식으로 함수 묶을수잇을듯
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

namespace PlayerSprite {
	// 스프라이트 프레임 기본 크기 (WalkBitmap / ActionBitmap 공통)
	constexpr int kFrameW      = 62;
	constexpr int kFrameH      = 50;

	// WalkBitmap: 상행=좌향 y=0, 하행=우향 y=kFrameH, x 간격=kWalkStride
	constexpr int kWalkStride  = 68;

	// ActionBitmap
	constexpr int kRightColX    = 77;   // 우향 컬럼 x (점프·웅크리기) — 로프 stride와 동일
	constexpr int kRopeY        = 54;   // 로프 프레임 y
	constexpr int kJumpY        = 107;  // 점프/공중 프레임 y
	constexpr int kJumpRightH   = 48;   // 점프 우향 프레임 높이 (좌향 50과 다름)
	constexpr int kCrouchY      = 161;  // 웅크리기 프레임 y
	constexpr int kCrouchFrameH = 26;   // 웅크리기 프레임 높이
	constexpr int kDeathPoseH   = 13;   // 사망 자세 높이
}

class PLAYER : public Object
{

public:
	PLAYER();
	virtual ~PLAYER();
public:


	int GetSaveY() { return SavedY; }
	int GetSpriteWidth() { return SpriteWidth; }
	int GetSpriteHeight() { return SpriteHeight; }

	int GetRowSpeed() { return m_rowSpeed; }
	int GetCurrentHP() { return CurrentHP; }
	int GetFallSpeed() { return FallAdjustSpeed; }
	int GetInvincibleTime() { return InvincibleTime; }
	int GetJumpCooldown() { return RopeJumpCooldown; }
	int GetSpikeKnockback() { return SpikeKnockback; }
	int GetJumpCount() const { return m_jumpCount; }
	int GetDieCount() const { return m_dieCount; }

	bool GetGameMode() { return IsGameMode; }
	bool IsHurt() { return bIsHurt; }
	bool IsRopeHurt() { return bIsRopHurt; }
	bool IsDead() { return bIsDead; }

	EPlayerState GetState() { return PlayerState; }
	EPlayerDirection GetDirection() { return PlayerDirection; }
	EMoveCommand GetMoveCommand() { return MoveCommand; }

	void SetSpriteWidth(int w) { SpriteWidth = w; }
	void SetSpriteHeight(int h) { SpriteHeight = h; }

	void SetSaveY(int y) { SavedY = y; }
	void SetCurrentHP(int hp) { CurrentHP = hp; }
	void SetFallSpeed(int speed) { FallAdjustSpeed = speed; }
	void SetState(EPlayerState state) { PlayerState = state; }
	void SetRowSpeed(int val) { m_rowSpeed = val; }
	//bool변수 너무많지않나
	void SetDirection(EPlayerDirection dir) { PlayerDirection = dir; }
	void SetMoveCommand(EMoveCommand command) { MoveCommand = command; }
	void SetHurt(bool value) { bIsHurt = value; }
	void SetRopeHurt(bool value) { bIsRopHurt = value; }
	void SetDead(bool value) { bIsDead = value; }

	void SetGameMode(bool value) { IsGameMode = value; }
	void SetInvicible(int value) { InvincibleTime = value; }
	void SetSpikeHurt(int value) { SpikeKnockback = value; }

	void SetBitMap(HINSTANCE hInstance);


public:
	// 캐릭터위치 초기화 함수
	void Initialzie();
	void InitializeAnimPosition();

	void ProcessInput(Sound& sound);

	void Update(float dt) override;

	//방향키 눌렀을때 플레이어 세팅
	void OnKeyPressed(WPARAM key, Sound& sound);
	//방향키 땠을때 플레이어 세팅풀기
	void OnKeyReleased(WPARAM key);

	void SavePositionX() {	SavedX = x;	}
	//플레이어 무브
	void UpdateMovement();
	//플레이어 스프라이트선택
	void SelectBitmap();
	//애니메이션
	void PlayAnim();
	//플레이어 그림
	void Render(HDC& backDC);
	//피격시 무적시간
	void UpdateInvincibilityTimer();
	//가시한테 찔렸을때 강제이동값
	void UpdateSpikeKnockback();
	//다쳤을때 피 다는 계산식 + 뒤지면초기화
	void TakeDamage(Sound& sound);
	//뒤지면 초기화
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
	int SpriteWidth, SpriteHeight;	//캐릭터 전체 사이즈이다. 
	int SavedX, SavedY;	//savey 는 점프뛸때 그 순간의 y좌표를 기억하기 위함이고 x는 혹시몰라서 넣어둠

	int CurrentHP;				//플레이어의 hp이다.
	EPlayerDirection PlayerDirection;			//1왼쪽 2오른쪽 3위 4아래 플레이어가 보고있는'방향'
	EPlayerState PlayerState;			//1기본상태,2점프상태,3숙이기상태,4이동상태,5줄 정지,6피격상태,7공중에있는상태 8 줄이동
	int FallAdjustSpeed;		//떨어질때 x값 천천히 이동시켜주기 위한 변수

	EMoveCommand MoveCommand;	//이게 움직이고있는 상황인지 아닌지 구분 1이면왼쪽으로움직임  2면 오른쪽으로 움직임0이면 안움직임 3이면 위로움직임 4면아래로움직임
	bool bIsHurt;	//쳐맞으면 1 아니면 0 점프할때 로직에 들어감
	bool bIsRopHurt;	//로프에서 쳐맞으면 1 아니면 0
	bool bIsDead;	//죽으면 1 아니면 0
	bool IsGameMode;		//0이면 일반 1이면 플라잉모드

	int InvincibleTime;		//피격시 무적 2초를 기준으로한다.
	int RopeJumpCooldown;		//줄에서 점프시 줄 바로 못잡게한다. stealth 와 같이 돌아갈예정
	int SpikeKnockback;		//하강중 가시로인한 강제적 좌표이동값 -이면 왼쪽으로가야함 + 이면 오른쪽으로 강제이동
	
	// 이동 속도 (예전 전역 ROWSPEED / kColSpeed / kRopeSpeed)
	static constexpr int kColSpeed = 10;
	static constexpr int kRopeSpeed = 2;
	static constexpr int kDefaultRowSpeed = 3;
	static constexpr int kJumpRowSpeed = 1;
	int m_rowSpeed = kDefaultRowSpeed;

	// 애니메이션 누적 시간 (5/60s = 걷기, 10/60s = 로프)
	static constexpr float kWalkAnimPeriod = 5.f / 60.f;
	static constexpr float kRopeAnimPeriod = 10.f / 60.f;
	float m_walkAnimAccum = 0.f;
	float m_ropeAnimAccum = 0.f;

	int m_jumpCount = 0;
	int m_dieCount = 0;

	HBITMAP m_oldplayerBitmap = nullptr;
	HBITMAP CurrentBitmap;    //현재 사용중인 비트맵 (WalkBitmap/ActionBitmap 중 하나를 가리키는 비소유 별칭)
	BmpPtr WalkBitmap;   //걸을때 비트맵
	BmpPtr ActionBitmap;		//점프,숙이기 줄타기 비트맵
	int AnimX, AnimY, AnimWidth, AnimHeight; //비트맵의 스프라이트위치를 바꾸기위한 좌표 변수
};