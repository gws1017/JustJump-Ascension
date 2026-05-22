#pragma once

class CAMERA{

public:
	CAMERA() : x_(0), y_(GameConst::kDefaultCameraY) {}

public:
    void SetX(int x) { x_ = x; }
    void SetY(int y) { y_ = y; }

    int GetX() const { return x_; }
    int GetY() const { return y_; }

public:

    void CameraSetting(WPARAM key);

private:
    int x_;
    int y_;
};