#include "core/InputManager.h"

InputManager* InputManager::s_instance = nullptr;

void InputManager::Register(InputManager* instance)
{
	s_instance = instance;
}

void InputManager::Unregister()
{
	s_instance = nullptr;
}

InputManager& InputManager::Get()
{
	assert(s_instance != nullptr && "InputManager::Get — Register() 먼저 호출");
	return *s_instance;
}

bool InputManager::IsRegistered()
{
	return s_instance != nullptr;
}

std::uint8_t InputManager::VkIndex(WPARAM wParam)
{
	return static_cast<std::uint8_t>(wParam & 0xFF);
}

void InputManager::OnKeyDown(WPARAM vk)
{
	m_keyCurr[VkIndex(vk)] = true;
}

void InputManager::OnKeyUp(WPARAM vk)
{
	m_keyCurr[VkIndex(vk)] = false;
}

void InputManager::OnMouseMove(LPARAM lParam)
{
	m_mousePos.x = static_cast<short>(LOWORD(lParam));
	m_mousePos.y = static_cast<short>(HIWORD(lParam));
}

void InputManager::OnMouseDown(WPARAM /*mk*/, LPARAM lParam)
{
	OnMouseMove(lParam);
	m_lmbCurr = true;
}

void InputManager::OnMouseUp(WPARAM /*mk*/, LPARAM lParam)
{
	OnMouseMove(lParam);
	m_lmbCurr = false;
}

void InputManager::BeginFrame()
{
}

void InputManager::EndFrame()
{
	m_keyPrev = m_keyCurr;
	m_lmbPrev = m_lmbCurr;
}

bool InputManager::IsKeyDown(std::uint8_t vk) const
{
	if (!IsRegistered()) return false;
	return m_keyCurr[vk];
}

bool InputManager::IsKeyPressed(std::uint8_t vk) const
{
	return m_keyCurr[vk] && !m_keyPrev[vk];
}

bool InputManager::IsKeyReleased(std::uint8_t vk) const
{
	return !m_keyCurr[vk] && m_keyPrev[vk];
}

bool InputManager::IsMouseHeld() const
{
	return m_lmbCurr;
}

bool InputManager::IsMousePressed() const
{
	return m_lmbCurr && !m_lmbPrev;
}

bool InputManager::IsMouseReleased() const
{
	return !m_lmbCurr && m_lmbPrev;
}

namespace InputHelper{
	bool IsLeftDown() { return InputManager::Get().IsKeyDown(VK_LEFT); }
	bool IsRightDown() { return InputManager::Get().IsKeyDown(VK_RIGHT); }
	bool IsUpDown() { return InputManager::Get().IsKeyDown(VK_UP); }
	bool IsDownDown() { return InputManager::Get().IsKeyDown(VK_DOWN); }
	bool IsLRConflict() { return IsLeftDown() && IsRightDown(); }  // 예전 LRkey 대체
	bool IsUDConflict() { return IsUpDown() && IsDownDown(); }     // 예전 UDkey 대체
}