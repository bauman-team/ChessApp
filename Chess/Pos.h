#pragma once

struct Pos
{
	int x, y;
	Pos(int _x = 0, int _y = 0) : x(_x), y(_y) {}
	Pos& operator=(const Pos& coords)
	{
		x = coords.x;
		y = coords.y;
		return *this;
	}
	Pos& operator*=(float value)
	{
		x *= value;
		y *= value;
		return *this;
	}
};