#pragma once
#include <stdint.h>

class Pos
{
	int8_t x, y;
public:
	Pos(int8_t _x = 0, int8_t _y = 0) : x(_x), y(_y) {}

	int8_t GetX() const { return x; }
	int8_t GetY() const { return y; }

	int8_t ToIndex() const;
	bool IsValid() const;

	Pos operator+(const Pos& coords);
	Pos& operator=(const Pos& coords);
	Pos& operator*=(float value);
	bool operator==(const Pos& coords) const;

	Pos& AddToX(int8_t) const; // don't change count, only return
	Pos& AddToY(int8_t) const; // don't change count, only return
};
