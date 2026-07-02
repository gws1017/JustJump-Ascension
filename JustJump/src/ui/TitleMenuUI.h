#pragma once
#include <Windows.h>
class Sound;

class TitleMenuUI
{
public:
	void Load(HINSTANCE hInst);
	void Unload();
	void Reset();

	void OnMouseMove(LPARAM mouse, Sound& sound);
	void OnMouseDown(LPARAM mouse, Sound& sound);
	bool OnMouseUp(LPARAM mouse);	// true = 게임 시작 확정

	void Render(HDC mem1dc);

private:
	static bool IsInRect(LPARAM mouse, int l, int t, int r, int b);

	int  m_startButton = 0;		// 0=기본, 1=hover, 2=press
	int  m_helpButton  = 0;		// 0=기본, 1=열림
	bool m_occurButton = false;

	HBITMAP m_startBit[3] = {};
	HBITMAP m_helpBit[2]  = {};
};
