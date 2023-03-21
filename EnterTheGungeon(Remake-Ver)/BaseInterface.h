#pragma once

#include "pch.h"

class BaseInterface
{
public:
	virtual void update() = 0;
	virtual void render(const HDC& hdc) const = 0;
	virtual void release() = 0;

	virtual ~BaseInterface() {};
};
