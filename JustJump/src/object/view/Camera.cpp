#include "Global.h"
#include "object/view/Camera.h"
#include "core/InputManager.h"

void CAMERA::ProcessInput()
{
    if (!InputManager::IsRegistered())
        return;

    auto& input = InputManager::Get();
    if (input.IsActionDown(EInputAction::CameraUp) && y_ > 0)
        y_ -= GameConst::kCameraScrollSpeed;
    if (input.IsActionDown(EInputAction::CameraDown) && y_ + GameConst::kViewportHeight < GameConst::kWorldHeight)
        y_ += GameConst::kCameraScrollSpeed;
}
