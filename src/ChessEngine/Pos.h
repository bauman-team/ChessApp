#pragma once
#include <stdint.h>
#include <cassert>
#include <string>
#include "ChessEngine.h"


class CHESSENGINE_API Pos
{
	uint8_t xy;
public:
	Pos() noexcept : xy{ 255 } { }
	Pos(uint8_t x, uint8_t y) noexcept;

	auto GetX() const noexcept -> uint8_t;
	auto GetY() const noexcept -> uint8_t;

	auto ToIndex() const noexcept -> uint8_t;
	auto ToBitboard() const noexcept -> uint64_t;
	auto IsValid() const noexcept -> bool;

	auto operator=(const Pos& coords) noexcept -> Pos&;
	auto operator==(const Pos& coords) const noexcept -> bool;
	auto operator!=(const Pos& coords) const noexcept -> bool;
	auto ToString() const -> std::string;

	auto Add(int8_t x, int8_t y) const noexcept -> Pos;

	static auto BitboardToPosition(uint64_t bitboard) noexcept -> Pos;
	static auto IndexToPosition(uint8_t index) noexcept -> Pos;

	const static Pos NULL_POS;
};