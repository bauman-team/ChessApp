#pragma once
#include "Figure.h"
#include "Pos.h"

class Figure;
class Rook;
class Map
{
	Figure* map[64];

	friend void ChangeCoordsForCastling(Rook&, Pos newCoords);
public:
	Map(); 

	Figure* GetFigureAt(const Pos& pos) const;
	Figure* GetFigureAt(int index) const;

	void Move(const Pos& from, const Pos& to);
	void SetToEmpty(const Pos& target);
	void PawnToQueen(const Pos& target);
	void Castling(const Pos& from, const Pos& to);

	bool CheckingShah(const Pos& kingPos);
	bool CheckingMate(); // Player need info about who makes the move
	bool CheckingPat();
	void CancelMove();

	int8_t CheckEmpty(const Pos& from, const Pos& to) const;
};