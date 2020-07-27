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

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
	return static_cast<typename std::underlying_type<E>::type>(e);
}


struct Pos
{
	uint8_t x, y;
	Pos() : x(0), y(0) {}
	Pos(uint8_t _x, uint8_t _y) : x(_x), y(_y) {}
	Pos& operator=(const Pos& coords)
	{
		x = coords.x;
		y = coords.y;
		return *this;
	}
	bool operator==(const Pos& coords)
	{
		return x == coords.x && y == coords.y;
	}
};

class Map;
class Player;
class Figure
{
protected:
	friend class Player; // for acess to methods to find moves
	static Map* ptrMap;
	Pos coords;
	FigureType id;
	std::vector<Pos> possibleMoves;
	virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos> FindPossibleMoves() = 0;
public:
	Figure(Pos _coords, FigureType _id) : coords(_coords), id(_id) {}
	FigureType GetId() const { return id; }
	const std::vector<Pos>& GetPossibleMoves() const { return possibleMoves; }
};

class EmptY : public Figure
{
	virtual bool MakeMoveTo(const Pos&) { return false; }
	virtual std::vector<Pos> FindPossibleMoves() { return possibleMoves; }
public:
	EmptY(Pos _coords, FigureType _id) : Figure(_coords, _id) {}
};

class King : public Figure
{
	bool possibleCastling; 
	virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos> FindPossibleMoves(); //
public:
	King(Pos _coords, FigureType _id) : Figure(_coords, _id) { possibleCastling = true; }
};

class Queen : public Figure
{
	//virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos> FindPossibleMoves();
public:
	Queen(Pos _coords, FigureType _id) : Figure(_coords, _id) {}
};

class Bishop : public Figure
{
	//virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos> FindPossibleMoves();
public:
	Bishop(Pos _coords, FigureType _id) : Figure(_coords, _id) {}
};

class Knight : public Figure
{
	//virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos> FindPossibleMoves();
public:
	Knight(Pos _coords, FigureType _id) : Figure(_coords, _id) {}
};

class Rook : public Figure
{
	bool possibleCastling; 
	virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos> FindPossibleMoves();
public:
	Rook(Pos _coords, FigureType _id) : Figure(_coords, _id) { possibleCastling = true; }
	bool GetCastling() { return possibleCastling; } // for king castling
};

class Pawn : public Figure
{
	virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos> FindPossibleMoves();
public:
	Pawn(Pos _coords, FigureType _id) : Figure(_coords, _id) {}
};