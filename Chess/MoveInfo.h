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
	bool castling[4];
public:
	MoveInfo(const Pos& _from, const Pos& _to, FigureType _activeFigure, FigureType _eatenFigure, bool *_castling);
	MoveInfo(const MoveInfo& copy);

	bool isEatenFigureExists() const;

	FigureType GetTypeActiveFigure() const { return activeFigure; }
	FigureType GetTypeEatenFigure() const { return eatenFigure; }
	Pos GetPosBeforeMove() const { return from; }
	Pos GetPosAfterMove() const { return to; }
	bool *GetCastlingInfo() { return castling; }

	void SetEatenFigure(FigureType _eatenFigure) { eatenFigure = _eatenFigure; }

	bool operator==(const MoveInfo& comp) const;
	bool operator!=(const MoveInfo& comp) const;

	const static MoveInfo NULL_INFO;
};