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


class Map;
class Player;
class Figure
{
protected:
	friend class Player; // for acess to methods to find moves
	static Map* ptrMap;
	Pos coords;
	FigureType type;
	std::vector<Pos> possibleMoves;
	virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos> FindPossibleMoves() = 0;
	friend void SetMapPtr(Map*); // TODO: maybe replace with 'static void SetMapPtr(Map*);'
public:
	Figure(Pos _coords, FigureType _type) : coords(_coords), type(_type) {}
	FigureType GetType() const { return type; }
	const std::vector<Pos>& GetPossibleMoves() const { return possibleMoves; }
};


class EmptY : public Figure
{
	virtual bool MakeMoveTo(const Pos&) override { return false; }
	virtual std::vector<Pos> FindPossibleMoves() override { return possibleMoves; }
public:
	EmptY(Pos _coords, FigureType _type) : Figure(_coords, _type) {}
};

class King : public Figure
{
	bool possibleCastling; 
	virtual bool MakeMoveTo(const Pos&) override;
	virtual std::vector<Pos> FindPossibleMoves() override; //
public:
	King(Pos _coords, FigureType _type) : Figure(_coords, _type) { possibleCastling = true; }
};

class Queen : public Figure
{
	//virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos> FindPossibleMoves() override;
public:
	Queen(Pos _coords, FigureType _type) : Figure(_coords, _type) {}
};

class Bishop : public Figure
{
	//virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos> FindPossibleMoves() override;
public:
	Bishop(Pos _coords, FigureType _type) : Figure(_coords, _type) {}
};

class Knight : public Figure
{
	//virtual bool MakeMoveTo(const Pos&);
	virtual std::vector<Pos> FindPossibleMoves() override;
public:
	Knight(Pos _coords, FigureType _type) : Figure(_coords, _type) {}
};

class Rook : public Figure
{
	bool possibleCastling; 
	virtual bool MakeMoveTo(const Pos&) override;
	virtual std::vector<Pos> FindPossibleMoves() override;
public:
	Rook(Pos _coords, FigureType _type) : Figure(_coords, _type) { possibleCastling = true; }
	bool GetCastling() { return possibleCastling; } // for king castling
};

class Pawn : public Figure
{
	virtual bool MakeMoveTo(const Pos&) override;
	virtual std::vector<Pos> FindPossibleMoves() override;
public:
	Pawn(Pos _coords, FigureType _type) : Figure(_coords, _type) {}
};