#include "Global.h"
#include "object/view/Camera.h"
#include "core/InputManager.h"

void CAMERA::ProcessInput()
{
    if (!InputManager::IsRegistered())
        return;

    auto& input = InputManager::Get();
    if (input.IsKeyDown(VK_UP) && y_ > 0)
        y_ -= 20;
    if (input.IsKeyDown(VK_DOWN) && y_ + 768 < 4096)
        y_ += 20;
}
