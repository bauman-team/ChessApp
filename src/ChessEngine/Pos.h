#pragma once
#include <stdint.h>
#include <cassert>
#include <string>
#include "ChessEngine.h"


class CHESSENGINE_API Pos
{
	uint8_t xy;
public:
	constexpr Pos() noexcept : xy{ 255 } { }
	Pos(uint8_t x, uint8_t y) noexcept;

	uint8_t GetX() const noexcept { return xy >> 4; }
	uint8_t GetY() const noexcept { return xy & 15; }

	uint8_t ToIndex() const noexcept { return (xy & 15) * 8 + (xy >> 4); }
	uint64_t ToBitboard() const noexcept;
	auto IsValid() const noexcept { return xy != 255; }

	auto operator==(const Pos& coords) const noexcept { return xy == coords.xy; }
	auto operator!=(const Pos& coords) const noexcept { return xy != coords.xy; }
	auto ToString() const { return static_cast<char>((xy >> 4) + 'A') + std::to_string((xy & 15) + 1); }

	auto Add(int8_t x, int8_t y) const noexcept { return Pos{ static_cast<uint8_t>((xy >> 4) + x), static_cast<uint8_t>((xy & 15) + y) }; }

	static Pos BitboardToPosition(uint64_t bitboard) noexcept;
	static auto IndexToPosition(uint8_t index) noexcept { return Pos{ static_cast<uint8_t>(index % 8), static_cast<uint8_t>(index / 8) }; }

	const static Pos NULL_POS;
};