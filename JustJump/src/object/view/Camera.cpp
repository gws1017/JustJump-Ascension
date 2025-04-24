#include "Global.h"
#include "object/view/Camera.h"

void CAMERA::CameraSetting(WPARAM key)
{
    switch (key)
    {
    case VK_UP:
        if (y_ > 0)
            y_ -= 20;
        break;

    case VK_DOWN:
        if (y_ + 768 < 4096)
            y_ += 20;
        break;

    case VK_LEFT:
    case VK_RIGHT:
        break;
    }
}
