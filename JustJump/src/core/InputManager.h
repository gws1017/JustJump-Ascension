#pragma once

#include <Windows.h>
#include <array>
#include <cassert>
#include <cstdint>

class InputManager
{
public:
	static void Register(InputManager* instance);
	static void Unregister();
	static InputManager& Get();
	static bool IsRegistered();

	void OnKeyDown(WPARAM vk);
	void OnKeyUp(WPARAM vk);
	void OnMouseMove(LPARAM lParam);
	void OnMouseDown(WPARAM mk, LPARAM lParam);
	void OnMouseUp(WPARAM mk, LPARAM lParam);

	void BeginFrame();
	void EndFrame();

	bool IsKeyDown(std::uint8_t vk) const;
	bool IsKeyPressed(std::uint8_t vk) const;
	bool IsKeyReleased(std::uint8_t vk) const;

	POINT GetMouseClientPos() const { return m_mousePos; }
	bool IsMouseHeld() const;
	bool IsMousePressed() const;
	bool IsMouseReleased() const;

private:
	static InputManager* s_instance;

	static std::uint8_t VkIndex(WPARAM wParam);

	std::array<bool, 256> m_keyCurr{};
	std::array<bool, 256> m_keyPrev{};

	POINT m_mousePos{};
	bool m_lmbCurr = false;
	bool m_lmbPrev = false;
};

namespace InputHelper
{
	inline bool IsLeftDown()
	{
		return InputManager::IsRegistered() && InputManager::Get().IsKeyDown(VK_LEFT);
	}
	inline bool IsRightDown()
	{
		return InputManager::IsRegistered() && InputManager::Get().IsKeyDown(VK_RIGHT);
	}
	inline bool IsUpDown()
	{
		return InputManager::IsRegistered() && InputManager::Get().IsKeyDown(VK_UP);
	}
	inline bool IsDownDown()
	{
		return InputManager::IsRegistered() && InputManager::Get().IsKeyDown(VK_DOWN);
	}
	inline bool IsLRConflict() { return IsLeftDown() && IsRightDown(); }
	inline bool IsUDConflict() { return IsUpDown() && IsDownDown(); }
}