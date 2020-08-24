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

	Pos& operator=(const Pos& coords);
	bool operator==(const Pos& coords) const;
	bool operator!=(const Pos& coords) const;

	Pos Add(int8_t x, int8_t y) const;

	static Pos& BitboardToPosition(uint64_t);
	static Pos IndexToPosition(uint8_t);

	const static Pos NULL_POS;
};