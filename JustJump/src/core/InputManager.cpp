#include "core/InputManager.h"

namespace
{
	// EInputAction 순서와 1:1로 대응하는 키 매핑 테이블. 리바인딩 시 이 테이블만 수정하면 된다.
	constexpr std::uint8_t kActionKeyMap[] = {
		VK_LEFT,   // MoveLeft
		VK_RIGHT,  // MoveRight
		VK_UP,     // MoveUp
		VK_DOWN,   // MoveDown
		VK_SPACE,  // Jump
		VK_UP,     // CameraUp
		VK_DOWN,   // CameraDown
	};
	static_assert(sizeof(kActionKeyMap) / sizeof(kActionKeyMap[0]) == static_cast<size_t>(EInputAction::Count),
		"kActionKeyMap과 EInputAction 항목 수가 일치해야 합니다.");
}

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

std::uint8_t InputManager::MappedKey(EInputAction action)
{
	return kActionKeyMap[static_cast<size_t>(action)];
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

bool InputManager::IsActionDown(EInputAction action) const
{
	return IsKeyDown(MappedKey(action));
}

bool InputManager::IsActionPressed(EInputAction action) const
{
	return IsKeyPressed(MappedKey(action));
}

bool InputManager::IsActionReleased(EInputAction action) const
{
	return IsKeyReleased(MappedKey(action));
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
