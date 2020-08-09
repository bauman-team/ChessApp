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
protected:
	friend class Player; // for acess to methods to find moves

	static Map* ptrMap;

	Pos coords;
	const Color color;
	FigureType type;
	std::vector<Pos> possibleMoves;

	bool movesFound;

	void ClearPossibleMoves();

	virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos>& FindPossibleMoves() = 0;

public:
	Figure(Pos _coords, Color _color) : coords(_coords), color(_color), movesFound(false) {}

	Pos GetPos() const { return coords; }
	bool IsMovesFound() const { return movesFound; }
	FigureType GetType() const { return type; }
	Color GetColor() const { return color; }
	const std::vector<Pos>& GetPossibleMoves() const { return possibleMoves; }
	static void SetMapPtr(Map* _ptrMap) { ptrMap = _ptrMap; }
};


class Empty : public Figure
{
	virtual bool MakeMoveTo(const Pos&) override { return false; }
	virtual std::vector<Pos>& FindPossibleMoves() override { return possibleMoves; }
public:
	Empty(Pos _coords);
};


class King : public Figure
{
	bool possibleCastling;

	virtual bool MakeMoveTo(const Pos&) override;
	virtual std::vector<Pos>& FindPossibleMoves() override; //
public:
	King(Pos _coords, Color _color);

	void SetCastling(bool changeTo) { possibleCastling = changeTo; } // for king castling
};


class Queen : public Figure
{
	virtual std::vector<Pos>& FindPossibleMoves() override;
public:
	Queen(Pos _coords, Color _color);
};


class Bishop : public Figure
{
	virtual std::vector<Pos>& FindPossibleMoves() override;
public:
	Bishop(Pos _coords, Color _color);
};


class Knight : public Figure
{
	virtual std::vector<Pos>& FindPossibleMoves() override;
public:
	Knight(Pos _coords, Color _color);
};


class Rook : public Figure
{
	bool possibleCastling;

	virtual bool MakeMoveTo(const Pos&) override;
	virtual std::vector<Pos>& FindPossibleMoves() override;

	friend void ChangeCoordsForCastling(Rook&, Pos newCoords);
public:
	Rook(Pos _coords, Color _color);

	bool GetCastling() { return possibleCastling; } // for king castling
};


class Pawn : public Figure
{
	virtual bool MakeMoveTo(const Pos&) override;
	virtual std::vector<Pos>& FindPossibleMoves() override;
public:
	Pawn(Pos _coords, Color _color);
};