#include "Pos.h"

int Pos::ToIndex() const
{
	return x + 8 * y;
}

bool Pos::IsValid() const
{
	return (x >= 0 && x <= 7 && y >= 0 && y <= 7);
}

Pos& Pos::operator=(const Pos& coords)
{
	x = coords.x;
	y = coords.y;
	return *this;
}

Pos& Pos::operator*=(float value)
{
	x *= value;
	y *= value;
	return *this;
}

bool Pos::operator==(const Pos& coords) const
{
	return x == coords.x && y == coords.y;
}
