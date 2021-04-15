#pragma once
#include <stdint.h>
#include <cassert>
#include <string>
#include "ChessEngine.h"


class CHESSENGINE_API Pos
{
	uint8_t xy;
public:
	Pos() noexcept : xy(255) {}
	Pos(uint8_t x, uint8_t y) noexcept;

	uint8_t GetX() const noexcept;
	uint8_t GetY() const noexcept;

	uint8_t ToIndex() const noexcept;
	uint64_t ToBitboard() const noexcept;
	bool IsValid() const noexcept;

	Pos& operator=(const Pos& coords) noexcept;
	bool operator==(const Pos& coords) const noexcept;
	bool operator!=(const Pos& coords) const noexcept;
	std::string ToString() const;

	Pos Add(int8_t x, int8_t y) const noexcept;

	static Pos BitboardToPosition(uint64_t bitboard) noexcept;
	static Pos IndexToPosition(uint8_t index) noexcept;

	const static Pos NULL_POS;
};