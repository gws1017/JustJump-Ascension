#pragma once

#include <Windows.h>
#include <array>
#include <cassert>
#include <cstdint>

#include "core/InputAction.h"

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

	// 액션 매핑 레이어: 호출부는 물리 키 대신 EInputAction을 조회한다.
	// 키 리바인딩은 InputManager.cpp의 매핑 테이블만 수정하면 된다.
	bool IsActionDown(EInputAction action) const;
	bool IsActionPressed(EInputAction action) const;
	bool IsActionReleased(EInputAction action) const;

	POINT GetMouseClientPos() const { return m_mousePos; }
	bool IsMouseHeld() const;
	bool IsMousePressed() const;
	bool IsMouseReleased() const;

private:
	static InputManager* s_instance;

	static std::uint8_t VkIndex(WPARAM wParam);
	static std::uint8_t MappedKey(EInputAction action);

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
		return InputManager::IsRegistered() && InputManager::Get().IsActionDown(EInputAction::MoveLeft);
	}
	inline bool IsRightDown()
	{
		return InputManager::IsRegistered() && InputManager::Get().IsActionDown(EInputAction::MoveRight);
	}
	inline bool IsUpDown()
	{
		return InputManager::IsRegistered() && InputManager::Get().IsActionDown(EInputAction::MoveUp);
	}
	inline bool IsDownDown()
	{
		return InputManager::IsRegistered() && InputManager::Get().IsActionDown(EInputAction::MoveDown);
	}
	inline bool IsLRConflict() { return IsLeftDown() && IsRightDown(); }
	inline bool IsUDConflict() { return IsUpDown() && IsDownDown(); }
}