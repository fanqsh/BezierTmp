#include "Vector2.h"

Vector2::~Vector2()
{
}

Vector2::Vector2(void)
{
	x = 0;
	y = 0;
}

Vector2::Vector2(const Vector2&)
{
}

Vector2::Vector2(int px, int py)
{
	x = px;
	y = py;
}

void Vector2::setValue(int px, int py)
{
	this->x = px;
	this->y = py;
}

void Vector2::Zero()
{
	x = 0;
	y = 0;
}
