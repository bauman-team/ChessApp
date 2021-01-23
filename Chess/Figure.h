#pragma once
#include <vector>
#include "Pos.h"
#include "Map.h"

enum class FigureType // used current fixed order
{
	King_black,
	Queen_black,
	Bishop_black,
	Knight_black,
	Rook_black,
	Pawn_black,
	King_white,
	Queen_white,
	Bishop_white,
	Knight_white,
	Rook_white,
	Pawn_white,
	Empty,
};

enum class Color { White, Black, None }; // None color for Empty figure

class Map;

class Figure
{
	static std::vector<Pos>& FindPossibleMovesKing(const Pos&, Map& ptrMap);
	static std::vector<Pos>& FindPossibleMovesQueen(const Pos&, const Map& ptrMap);
	static std::vector<Pos>& FindPossibleMovesBishop(const Pos&, const Map& ptrMap);
	static std::vector<Pos>& FindPossibleMovesKnight(const Pos&, const Map& ptrMap);
	static std::vector<Pos>& FindPossibleMovesRook(const Pos&, const Map& ptrMap);
	static std::vector<Pos>& FindPossibleMovesPawn(const Pos&, const Map& ptrMap);

	static std::vector<Pos> FindStraightMoves(const Pos& coords, const Map& ptrMap);
	static std::vector<Pos> FindDiagonalMoves(const Pos& coords, const Map& ptrMap);
public:
	static std::vector<Pos>& FindPossibleMoves(const FigureType&, const Pos&, Map& ptrMap);
	static Color GetFigureTypeColor(const FigureType&);
};