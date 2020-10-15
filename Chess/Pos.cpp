#include "Pos.h"

const Pos Pos::NULL_POS(-1, -1);

Pos::Pos(uint8_t x, uint8_t y)
{
	xy = 255;
	if (x < 8 && x >= 0 && y < 8 && y >= 0)
		xy = (x << 4) | y;
}

uint8_t Pos::ToIndex() const
{
	return (xy & 15) * 8 + (xy >> 4);
}

uint64_t& Pos::ToBitboard() const
{
	uint64_t bitboard = 1;
	for (int i = 0; i != GetY(); ++i)
		bitboard <<= 8;
	for (int i = 0; i != GetX(); ++i)
		bitboard <<= 1;
	return bitboard;
}

bool Pos::IsValid() const
{
	return (xy != 255);
}

Pos& Pos::operator=(const Pos& coords)
{
	xy = coords.xy;
	return *this;
}

bool Pos::operator==(const Pos& coords) const
{
	return xy == coords.xy;
}

bool Pos::operator!=(const Pos& coords) const
{
	return xy != coords.xy;
}

std::string Pos::ToString() const
{
	return (char)((xy >> 4) + 'A') + std::to_string((xy & 15) + 1);
}

Pos Pos::Add(int8_t x, int8_t y) const
{
	return Pos((xy >> 4) + x, (xy & 15) + y);
}

Pos& Pos::BitboardToPosition(uint64_t bitboard)
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
	return *(new Pos(x, y));
}

Pos Pos::IndexToPosition(uint8_t index)
{
	return Pos(index % 8, index / 8);
}
