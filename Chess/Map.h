#pragma once
#include "Figure.h"

class Map
{
	Figure* map[64];
public:
	//TODO: add constructor to initialize map
	void Move(Pos from, Pos to);
	void SetToEmpty(Pos target);
	Figure* GetMapArray() const { return *map; }
};