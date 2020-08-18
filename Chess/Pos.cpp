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

int8_t Pos::ToIndex() const
{

	return (xy & 15) * 8 + (xy >> 4);
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

Pos Pos::operator+(const Pos& coords)
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
