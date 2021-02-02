#pragma once
#include <vector>
#include "Pos.h"
#include "Map.h"

#define FIGURE_TYPES 12 // 6 whites + 6 black figures;

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
	static std::vector<Pos> FindPossibleMovesKing(const Pos& coord, Map& map);
	static std::vector<Pos> FindPossibleMovesQueen(const Pos& coord, const Map& map);
	static std::vector<Pos> FindPossibleMovesBishop(const Pos& coord, const Map& map);
	static std::vector<Pos> FindPossibleMovesKnight(const Pos& coord, const Map& map);
	static std::vector<Pos> FindPossibleMovesRook(const Pos& coord, const Map& map);
	static std::vector<Pos> FindPossibleMovesPawn(const Pos& coord, const Map& map);

	static std::vector<Pos> FindStraightMoves(const Pos& coord, const Map& map);
	static std::vector<Pos> FindDiagonalMoves(const Pos& coord, const Map& map);
public:
	static std::vector<Pos> FindPossibleMoves(const Pos& figurePosition, FigureType figureType, Map& map);
	static Color GetFigureTypeColor(FigureType figureType);
};