#pragma once

class CAMERA;
class PLAYER;

class MAP
{

public:
	//맵번호를 읽음 10~
	int GetMapNumber() { return mapnum; }
	//Black Screen Time get
	int GetBlackTime() { return black_t; }

	//맵번호를 바꿈
	void SetMapNumber(int i) { mapnum = i; }
	//Black Screen Time set
	void SetBlackTime(int i) { black_t = i; }

public:

	void CreateMap(HINSTANCE g_hinst);
	void CreateUi(HINSTANCE g_hinst);
	void CreateHP(HINSTANCE g_hinst);

	//로딩 타임 확인
	bool BlackTime();
	//맵 배경 이동
	void movemap();
	void DrawBK(HDC& mem1dc, HDC& mem2dc, RECT& rectview, const UPtr<PLAYER>& player);
	//로딩화면 알파블렌딩
	void DrawLoadBK(HDC& mem1dc, HDC& mem2dc, BLENDFUNCTION bf);
	//상태창
	void DrawUi(HDC& mem1dc, HDC& mem2dc, const UPtr<CAMERA>& camera);
	//HP바
	void DrawHP(HDC& mem1dc, HDC& mem2dc, const UPtr<CAMERA>& camera, const UPtr<PLAYER>& player);

private:

	int mapnum = static_cast<int>(EMapId::Title);
	int black_t = 0;
	int ms;
	int index;

	BmpPtr hbitbk;
	BmpPtr hbitui;
	BmpPtr hbithp;

};
