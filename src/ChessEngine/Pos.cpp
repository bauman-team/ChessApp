#include "pch.h"
#include "Pos.h"

const Pos Pos::NULL_POS{ };

Pos::Pos(uint8_t x, uint8_t y) noexcept
{
	xy = 255;
	if (x < 8 && x >= 0 && y < 8 && y >= 0)
		xy = (x << 4) | y;
}

auto Pos::GetX() const noexcept -> uint8_t
{
	return xy >> 4;
}

auto Pos::GetY() const noexcept -> uint8_t
{
	return xy & 15;
}

auto Pos::ToIndex() const noexcept -> uint8_t
{
	return (xy & 15) * 8 + (xy >> 4);
}

auto Pos::ToBitboard() const noexcept -> uint64_t
{
	uint64_t bitboard = 1;
	for (auto i = 0; i != GetY(); ++i)
		bitboard <<= 8;
	for (auto i = 0; i != GetX(); ++i)
		bitboard <<= 1;
	return bitboard;
}

auto Pos::IsValid() const noexcept -> bool
{
	return (xy != 255);
}

auto Pos::operator=(const Pos& coords) noexcept -> Pos&
{
	xy = coords.xy;
	return *this;
}

auto Pos::operator==(const Pos& coords) const noexcept -> bool
{
	return xy == coords.xy;
}

auto Pos::operator!=(const Pos& coords) const noexcept -> bool
{
	return xy != coords.xy;
}

auto Pos::ToString() const -> std::string
{
	return static_cast<char>((xy >> 4) + 'A') + std::to_string((xy & 15) + 1);
}

auto Pos::Add(int8_t x, int8_t y) const noexcept -> Pos
{
	return Pos{ static_cast<uint8_t>((xy >> 4) + x), static_cast<uint8_t>((xy & 15) + y) };
}

auto Pos::BitboardToPosition(uint64_t bitboard) noexcept -> Pos
{
	uint8_t x = 0, y = 0;
	while (bitboard > 128)
	{
		++y;
		bitboard >>= 8;
	}
	while (bitboard > 1)
	{
		++x;
		bitboard >>= 1;
	}
	return Pos{ x, y };
}

auto Pos::IndexToPosition(uint8_t index) noexcept -> Pos
{
	return Pos{ static_cast<uint8_t>(index % 8), static_cast<uint8_t>(index / 8) };
}
