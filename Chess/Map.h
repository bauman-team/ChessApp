#pragma once
#include "Figure.h"
#include "Pos.h"
#include <vector>
#include <stack>
#include "MoveInfo.h"

class Figure;
class Rook;
class MoveInfo;
enum class Color;

class Map
{
	Figure* map[64];
	std::vector<MoveInfo> movesHistory;

public:
	Map(); 
	Map(const Map&);

	Figure* GetFigureAt(const Pos& pos) const;
	Figure* GetFigureAt(int index) const;
	Pos GetFigurePosition(Figure*) const;

	void RunFindMoves(Figure* choseFigure);
	bool RunMakeMove(Figure* choseFigure, Pos& nextPosition);
	void RunClearPossibleMoves(const Color& activeColor);

	void Move(const Pos& from, const Pos& to);
	void SetToEmpty(const Pos& target);
	void PawnToQueen(const Pos& target);
	void Castling(const Pos& from, const Pos& to);

	bool CheckingShah(const Pos& kingPos);
	std::vector<Pos> CheckingPossibleMove(Pos, const std::vector<Pos>&);

	int8_t CheckEmpty(const Pos& from, const Pos& to) const;

	MoveInfo* GetLastMoveInfo();

	std::vector<MoveInfo>& GetMovesHistory();
};