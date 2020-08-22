#pragma once
#include <stdint.h>
#include <cassert>

class Pos
{
	uint8_t xy;
public:
	Pos(uint8_t x = 0, uint8_t y = 0);

	uint8_t GetX() const { return xy >> 4; }
	uint8_t GetY() const { return xy & 15; }

	uint8_t ToIndex() const;
	uint64_t& ToBitboard() const;
	bool IsValid() const;

	Pos operator+(const Pos& coords) const;
	Pos& operator=(const Pos& coords);
	Pos& operator*=(float value);
	bool operator==(const Pos& coords) const;

	Pos& AddToX(int8_t) const; // don't change count, only return
	Pos& AddToY(int8_t) const; // don't change count, only return

	static Pos& BitboardToPosition(uint64_t);
	static Pos& IndexToPosition(uint8_t);
};
