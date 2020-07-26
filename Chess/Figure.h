#pragma once
#include <vector>
#include "Pos.h"

enum FigureType
{
	
	Queen_black,
	Rook_black,
	Bishop_black,
	
	Pawn_black,

	King_white,
	Queen_white,
	Rook_white,
	Knight_black,
	Bishop_white,
	Knight_white,
	Pawn_white,
	King_black,
	
	Empty
};

class Figure
{
protected:
	Pos coords;
	FigureType id;
	std::vector<Pos> possibleMoves;
	bool virtual MakeMoveTo() = 0;
	std::vector<Pos> virtual FindPossibleMoves() = 0;
public:
	Figure(Pos _coords, FigureType _id) : coords(_coords), id(_id) {}
	FigureType GetId() const { return id; }
	const std::vector<Pos>& GetPossibleMoves() const { return possibleMoves; }
};