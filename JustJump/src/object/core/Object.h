#pragma once
#include "core/IObject.h"


class Object : public IObject
{
public:
	Object() = default;
	Object(int x, int y, int width, int height);
	virtual ~Object();

public:

	int GetX() { return x; };
	int GetY() { return y; };
	int GetWidth() { return width; };
	int GetHeight() { return height; };

	void SetX(int _x) { x = _x; }
	void SetY(int _y) { y = _y; }
	void SetWidth(int _w) { width = _w; }
	void SetHeight(int _h) { height = _h; }

public:

	virtual void Draw() {};
	
protected :

	int x, y, width, height;					//오브젝트의 x y 는 왼쪽상단이 기준이다.
};