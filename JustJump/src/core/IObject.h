#pragma once

class IObject
{
public:
	virtual void Update(float dt) {}
	virtual ~IObject() = default;
};