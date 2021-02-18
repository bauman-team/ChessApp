#pragma once
#include <stdint.h>
#include <cassert>
#include <string>
#include <utility>
#include <limits.h>
#include "ChessEngine.h"


class CHESSENGINE_API Pos
{
	uint8_t xy;
public:
	Pos() : xy(255) {}
	Pos(uint8_t x, uint8_t y);

	uint8_t GetX() const;
	uint8_t GetY() const;

	uint8_t ToIndex() const;
	uint64_t ToBitboard() const;
	bool IsValid() const;

	Pos& operator=(const Pos& coords);
	bool operator==(const Pos& coords) const;
	bool operator!=(const Pos& coords) const;
	std::string ToString() const;

	Pos Add(int8_t x, int8_t y) const;

	static Pos BitboardToPosition(uint64_t bitboard);
	static Pos IndexToPosition(uint8_t index);

	const static Pos NULL_POS;
};