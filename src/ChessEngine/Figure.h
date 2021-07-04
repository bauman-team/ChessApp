#pragma once

#define FIGURE_TYPES 12  // 6 whites + 6 black figures;

#include <vector>
#include <array>
#include "Pos.h"

class Map;
class MoveInfo;

using MovesInfo = std::vector<std::vector<MoveInfo>>;

enum class CHESSENGINE_API FigureType // used fixed enum order
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

enum class CHESSENGINE_API Color { White, Black, None }; // None color for Empty figure, used fixed enum order

enum class CHESSENGINE_API BoardPos { Lock, Empty, Opposite }; // used fixed enum order

template<typename E>
constexpr auto
toUType(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);
}

class CHESSENGINE_API Figure
{
	static BoardPos CheckEmpty(const Color boardPosFrom, const Color boardPosTo) noexcept;
	static MovesInfo FindPossibleMovesKing(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept;
	static MovesInfo FindPossibleMovesQueen(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept;
	static MovesInfo FindPossibleMovesBishop(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept;
	static MovesInfo FindPossibleMovesKnight(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept;
	static MovesInfo FindPossibleMovesRook(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept;
	static MovesInfo FindPossibleMovesPawn(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept;

	static MovesInfo FindStraightMoves(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept;
	static MovesInfo FindDiagonalMoves(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept;
public:
	static void EraseForbiddenMoves(MovesInfo& allFiguresMoves, Map& map) noexcept;
	static bool IsShahFor(const Color kingColor, std::array<uint64_t, FIGURE_TYPES> map) noexcept;
	static MovesInfo FindPossibleMoves(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept;
	static Color GetFigureTypeColor(FigureType figureType) noexcept;
};