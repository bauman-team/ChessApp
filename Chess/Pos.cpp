#include "Pos.h"

Pos::Pos(uint8_t x, uint8_t y)
{
	xy = 0;
	if (x < 16 && x >= 0 && y < 16 && y >= 0)
	{
		xy = x;
		xy <<= 4;
		xy |= y;
	}
	else
	{
		xy = 255;
	}
}

uint8_t Pos::ToIndex() const
{

	return (xy & 15) * 8 + (xy >> 4);
}

uint64_t& Pos::ToBitboard() const
{
	uint64_t bitboard =  1;
	for (int i = 0; i != GetY(); ++i)
		bitboard <<= 8;
	for (int i = 0; i != GetX(); ++i)
		bitboard <<= 1;
	return bitboard;
}

bool Pos::IsValid() const
{
	return ((xy >> 4) >= 0 && (xy >> 4) <= 7 && (xy & 15) >= 0 && (xy & 15) <= 7);
}

Pos& Pos::operator=(const Pos& coords)
{
	xy = coords.xy;
	return *this;
}

Pos Pos::operator+(const Pos& coords) const
{
	return Pos(xy + coords.xy);
}

Pos& Pos::operator*=(float value)
{
	*this = Pos((xy >> 4) * value, (xy & 15) * value);
	return *this;
}

bool Pos::operator==(const Pos& coords) const
{
	return xy == coords.xy;
}

Pos& Pos::AddToX(int8_t x) const
{
	return *(new Pos((xy >> 4) + x, (xy & 15)));
}

Pos& Pos::AddToY(int8_t y) const
{
	return *(new Pos((xy >> 4), (xy & 15) + y));
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

Pos& Pos::IndexToPosition(uint8_t index)
{
	return *(new Pos(index % 8, index / 8));
}
