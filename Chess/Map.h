#pragma once
#include "Figure.h"

class Map
{
	Figure* map[64];
public:
	Map(); 
	void Move(const Pos& from, const Pos& to);
	void SetToEmpty(const Pos& target);
	void PawnToQueen(const Pos& target);
	void Castling(const Pos& from, const Pos& to);
	bool CheckingShah(); // add to all figures; impossible, because need log of moves
	bool CheckingMate(); // Player need info about who makes the move
	bool CheckingPat(); //
	int8_t CheckEmpty(const Pos& from, const Pos& to);
	Figure* GetMap() { return *map; }
	Figure* GetMapArray() const { return *map; }
};