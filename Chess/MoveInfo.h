#pragma once
#include "Pos.h"
#include "Figure.h"

class Figure;
enum class FigureType;

class MoveInfo
{
	FigureType activeFigure;
	FigureType eatenFigure;
	Pos from;
	Pos to;
public:
	MoveInfo(const Pos& _from, const Pos& _to, FigureType _activeFigure, FigureType _eatenFigure = FigureType::Empty)
		: activeFigure(_activeFigure), eatenFigure(_eatenFigure), from(_from), to(_to) { }
	MoveInfo(const MoveInfo& copy);

	bool isEatenFigureExists() const;

	FigureType GetTypeActiveFigure() const { return activeFigure; }
	FigureType GetTypeEatenFigure() const { return eatenFigure; }
	Pos GetPosBeforeMove() const { return from; }
	Pos GetPosAfterMove() const { return to; }

	void SetEatenFigure(FigureType _eatenFigure) { eatenFigure = _eatenFigure; }
};