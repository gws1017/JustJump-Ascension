#include "Global.h"
#include "ui/DeadScreenUI.h"
#include "object/view/Camera.h"

void DeadScreenUI::Load(HINSTANCE hInst)
{
	const wchar_t* files[] = { L"img/notice3.bmp", L"img/notice4.bmp", L"img/notice5.bmp" };
	for (int i = 0; i < 3; ++i)
		m_dieBit[i] = CreateBmpPtr((HBITMAP)LoadImage(hInst, files[i], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
}

void DeadScreenUI::Unload()
{
	for (auto& b : m_dieBit) b.reset();
}

bool DeadScreenUI::IsInRect(LPARAM mouse, int l, int t, int r, int b)
{
	const int x = LOWORD(mouse), y = HIWORD(mouse);
	return l < x && x < r && t < y && y < b;
}

void DeadScreenUI::OnMouseMove(LPARAM mouse, Sound& sound)
{
	const auto& kr = UiHitbox::kRevive;
	if (IsInRect(mouse, kr.left, kr.top, kr.right, kr.bottom))
	{
		m_state = 1;
		if (!m_occurButton)
		{
			if (sound.Channel[1]) sound.Channel[1]->stop();
			sound.System->playSound(sound.effectSound[4], nullptr, false, &sound.Channel[1]);
			m_occurButton = true;
		}
		return;
	}
	m_state = 0;
	m_occurButton = false;
}

void DeadScreenUI::OnMouseDown(LPARAM mouse, Sound& sound)
{
	const auto& kr = UiHitbox::kRevive;
	if (IsInRect(mouse, kr.left, kr.top, kr.right, kr.bottom))
	{
		m_state = 2;
		if (sound.Channel[1]) sound.Channel[1]->stop();
		sound.System->playSound(sound.effectSound[3], nullptr, false, &sound.Channel[1]);
	}
}

bool DeadScreenUI::OnMouseUp(LPARAM mouse)
{
	const auto& kr = UiHitbox::kRevive;
	if (IsInRect(mouse, kr.left, kr.top, kr.right, kr.bottom))
	{
		m_state = 0;
		return true;
	}
	return false;
}

void DeadScreenUI::Render(HDC mem1dc, const UPtr<CAMERA>& camera)
{
	HDC dc = CreateCompatibleDC(mem1dc);
	SelectObject(dc, m_dieBit[m_state].get());
	TransparentBlt(mem1dc, camera->GetX() + 380, camera->GetY() + 240, 260, 130, dc, 0, 0, 260, 130, RGB(255, 0, 0));
	DeleteDC(dc);

	HFONT hfont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 bold"));
	HFONT oldfont = (HFONT)SelectObject(mem1dc, hfont);
	SetTextColor(mem1dc, RGB(32, 108, 168));
	TextOut(mem1dc, camera->GetX() + 459, camera->GetY() + 260, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	TextOut(mem1dc, camera->GetX() + 461, camera->GetY() + 260, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	TextOut(mem1dc, camera->GetX() + 460, camera->GetY() + 259, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	TextOut(mem1dc, camera->GetX() + 460, camera->GetY() + 261, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	SetTextColor(mem1dc, RGB(255, 255, 255));
	TextOut(mem1dc, camera->GetX() + 460, camera->GetY() + 260, L"확인을 누르시면 부활하게 됩니다.", lstrlenW(L"확인을 누르시면 부활하게 됩니다."));
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);

	hfont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("메이플스토리 light"));
	oldfont = (HFONT)SelectObject(mem1dc, hfont);
	TextOut(mem1dc, camera->GetX() + 460, camera->GetY() + 285, L"아쉽지만 확인을 누르시면 맵의 맨 처음", lstrlenW(L"아쉽지만 확인을 누르시면 맵의 맨 처음."));
	TextOut(mem1dc, camera->GetX() + 460, camera->GetY() + 297, L"위치로 돌아가게 됩니다. 포기하지 마세요!", lstrlenW(L"위치로 돌아가게 됩니다. 포기하지 마세요!"));
	SelectObject(mem1dc, oldfont);
	DeleteObject(hfont);
}
