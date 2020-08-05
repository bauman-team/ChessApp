#pragma once
#include "Pos.h"
#include "Figure.h"

class Figure;

class MoveInfo
{
	Figure* activeFigure;
	Figure* eatenFigure;
	Pos from;
	Pos to;
public:
	MoveInfo(const Pos& _from, const Pos& _to, Figure* _activeFigure, Figure* _eatenFigure = nullptr)
		: activeFigure(_activeFigure), eatenFigure(_eatenFigure), from(_from), to(_to) {};

	bool isEatenFigureExists() const;

	Figure* GetActiveFigure() const { return activeFigure; }
	Figure* GetEatenFigure() const { return eatenFigure; }
	Pos GetPosBeforeMove() const { return from; }
	Pos GetPosAfterMove() const { return to; }

	void SetEatenFigure(Figure* _eatenFigure) { eatenFigure = _eatenFigure; }
};