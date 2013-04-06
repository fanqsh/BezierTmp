#pragma once

#ifndef _VECTOR2_H_
#define _VECTOR2_H_

class Vector2
{
public:
	int x, y;
	Vector2(void);
	Vector2(const Vector2&);
	Vector2(int, int);
	~Vector2();
	void Zero();
	void setValue(int, int);
};

#endif