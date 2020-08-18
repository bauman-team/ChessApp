#pragma once
#include <stdint.h>
#include <cassert>

class Pos
{
	uint8_t xy;
public:
	Pos(uint8_t x = 0, uint8_t y = 0);

	int8_t GetX() const { return xy >> 4; }
	int8_t GetY() const { return xy & 15; }

	int8_t ToIndex() const;
	bool IsValid() const;

	Pos operator+(const Pos& coords);
	Pos& operator=(const Pos& coords);
	Pos& operator*=(float value);
	bool operator==(const Pos& coords) const;

	Pos& AddToX(int8_t) const; // don't change count, only return
	Pos& AddToY(int8_t) const; // don't change count, only return
};
