#pragma once
#include <Windows.h>
class CAMERA;

class DeadScreenUI
{
public:
	void Load(HINSTANCE hInst);
	void Unload();

	void OnMouseMove(LPARAM mouse);
	void OnMouseDown(LPARAM mouse);
	bool OnMouseUp(LPARAM mouse);	// true = 부활 확정

	void Render(HDC mem1dc, const UPtr<CAMERA>& camera);

private:
	static bool IsInRect(LPARAM mouse, int l, int t, int r, int b);

	bool      m_occurButton = false;
	int       m_state       = 0;		// 0=기본, 1=hover, 2=press
	BmpPtr    m_dieBit[3];
};
