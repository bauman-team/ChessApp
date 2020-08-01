#pragma once

class Pos
{
	int x, y;
public:
	Pos(int _x = 0, int _y = 0) : x(_x), y(_y) {}

	int GetX() const { return x; }
	int GetY() const { return y; }

	int ToIndex() const;
	bool IsValid() const;

	Pos& operator=(const Pos& coords);
	Pos& operator*=(float value);
	bool operator==(const Pos& coords) const;
};
