#pragma once

class Pos
{
	int x, y;
public:
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
	bool operator==(const Pos& coords) const
	{
		return x == coords.x && y == coords.y;
	}
	int GetX() const { return x; }
	int GetY() const { return y; }
	int ToIndex() const { return x + 8 * y; }
	bool IsValid() const { return (x >= 0 && x <= 7 && y >= 0 && y <= 7); }
};
