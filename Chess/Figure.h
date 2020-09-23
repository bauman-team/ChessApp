#pragma once
#include <vector>
#include "Pos.h"
#include "Player.h"
#include "Map.h"

enum class FigureType
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

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
	return static_cast<typename std::underlying_type<E>::type>(e);
}


class Map;
class Player;

class Figure
{
	friend class Map;

	static std::vector<Pos>& FindPossibleMoves(const FigureType&, const Pos&, Map& ptrMap);
	static std::vector<Pos>& FindPossibleMovesKing(const Pos&, Map& ptrMap);
	static std::vector<Pos>& FindPossibleMovesQueen(const Pos&, Map& ptrMap);
	static std::vector<Pos>& FindPossibleMovesBishop(const Pos&, Map& ptrMap);
	static std::vector<Pos>& FindPossibleMovesKnight(const Pos&, Map& ptrMap);
	static std::vector<Pos>& FindPossibleMovesRook(const Pos&, Map& ptrMap);
	static std::vector<Pos>& FindPossibleMovesPawn(const Pos&, Map& ptrMap);

	static std::vector<Pos> FindStraightMoves(const Pos& coords, Map& ptrMap);
	static std::vector<Pos> FindDiagonalMoves(const Pos& coords, Map& ptrMap);
public:
	static Color GetFigureTypeColor(const FigureType&);
};