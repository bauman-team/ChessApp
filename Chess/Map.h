#pragma once
#include "Figure.h"

class Map
{
	Figure* map[64];
public:
	void Move(Pos from, Pos to);
	void SetToEmpty(Pos target);
};