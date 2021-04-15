#pragma once

#define FIGURE_TYPES 12  // 6 whites + 6 black figures;

#include <vector>
#include <array>
#include "Pos.h"
#include "Map.h"

enum class CHESSENGINE_API FigureType // used current fixed order
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

enum class CHESSENGINE_API Color { White, Black, None }; // None color for Empty figure

enum class CHESSENGINE_API BoardPos { Lock, Empty, Opposite };

template<typename E>
constexpr auto
toUType(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);
}

class Map;


class CHESSENGINE_API Figure
{
	static std::vector<Pos> FindPossibleMovesKing(const Pos& coord, Map& map) noexcept;
	static std::vector<Pos> FindPossibleMovesQueen(const Pos& coord, const Map& map) noexcept;
	static std::vector<Pos> FindPossibleMovesBishop(const Pos& coord, const Map& map) noexcept;
	static std::vector<Pos> FindPossibleMovesKnight(const Pos& coord, const Map& map) noexcept;
	static std::vector<Pos> FindPossibleMovesRook(const Pos& coord, const Map& map) noexcept;
	static std::vector<Pos> FindPossibleMovesPawn(const Pos& coord, const Map& map) noexcept;

	static std::vector<Pos> FindStraightMoves(const Pos& coord, const Map& map) noexcept;
	static std::vector<Pos> FindDiagonalMoves(const Pos& coord, const Map& map) noexcept;
public:
	static std::vector<Pos> FindPossibleMoves(const Pos& figurePosition, FigureType figureType, Map& map) noexcept;
	static Color GetFigureTypeColor(FigureType figureType) noexcept;
};