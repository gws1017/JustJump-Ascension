#include "Global.h"
#include "ui/TitleMenuUI.h"

void TitleMenuUI::Load(HINSTANCE hInst)
{
	const wchar_t* startFiles[] = { L"img/start1.bmp", L"img/start2.bmp", L"img/start3.bmp" };
	const wchar_t* helpFiles[]  = { L"img/help1.bmp",  L"img/help2.bmp" };
	for (int i = 0; i < 3; ++i)
		m_startBit[i] = (HBITMAP)LoadImage(hInst, startFiles[i], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	for (int i = 0; i < 2; ++i)
		m_helpBit[i]  = (HBITMAP)LoadImage(hInst, helpFiles[i],  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

void TitleMenuUI::Unload()
{
	for (auto& b : m_startBit) { if (b) { DeleteObject(b); b = nullptr; } }
	for (auto& b : m_helpBit)  { if (b) { DeleteObject(b); b = nullptr; } }
}

void TitleMenuUI::Reset()
{
	m_startButton = 0;
	m_helpButton  = 0;
	m_occurButton = false;
}

bool TitleMenuUI::IsInRect(LPARAM mouse, int l, int t, int r, int b)
{
	const int x = LOWORD(mouse), y = HIWORD(mouse);
	return l < x && x < r && t < y && y < b;
}

void TitleMenuUI::OnMouseMove(LPARAM mouse, Sound& sound)
{
	const auto& ks = UiHitbox::kStart;
	const auto& kh = UiHitbox::kHelp;

	if (IsInRect(mouse, ks.left, ks.top, ks.right, ks.bottom))
	{
		if (m_startButton == 0)
		{
			if (sound.Channel[1]) sound.Channel[1]->stop();
			sound.System->playSound(sound.effectSound[4], nullptr, false, &sound.Channel[1]);
			m_startButton = 1;
		}
		return;
	}
	if (IsInRect(mouse, kh.left, kh.top, kh.right, kh.bottom))
	{
		if (m_helpButton == 0)
		{
			if (sound.Channel[1]) sound.Channel[1]->stop();
			sound.System->playSound(sound.effectSound[4], nullptr, false, &sound.Channel[1]);
			m_helpButton = 1;
		}
		return;
	}
	Reset();
}

void TitleMenuUI::OnMouseDown(LPARAM mouse, Sound& sound)
{
	const auto& ks = UiHitbox::kStart;
	if (IsInRect(mouse, ks.left, ks.top, ks.right, ks.bottom) && m_startButton == 1)
	{
		if (sound.Channel[1]) sound.Channel[1]->stop();
		sound.System->playSound(sound.effectSound[3], nullptr, false, &sound.Channel[1]);
		m_startButton = 2;
	}
}

bool TitleMenuUI::OnMouseUp(LPARAM mouse)
{
	const auto& ks = UiHitbox::kStart;
	if (IsInRect(mouse, ks.left, ks.top, ks.right, ks.bottom))
	{
		m_occurButton = false;
		return true;
	}
	return false;
}

void TitleMenuUI::Render(HDC mem1dc)
{
	HDC dc = CreateCompatibleDC(mem1dc);

	SelectObject(dc, m_startBit[m_startButton]);
	TransparentBlt(mem1dc, 292, 490, 138, 82, dc, 0, 0, 138, 82, RGB(255, 0, 0));

	SelectObject(dc, m_helpBit[m_helpButton]);
	if (m_helpButton == 0)
		TransparentBlt(mem1dc, 290, 345, 138, 82, dc, 0, 0, 138, 82, RGB(60, 60, 60));
	else
		TransparentBlt(mem1dc, 215, 300, 400, 200, dc, 0, 0, 400, 200, RGB(0, 0, 0));

	DeleteDC(dc);
}
