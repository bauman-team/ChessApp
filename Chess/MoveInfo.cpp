#include "MoveInfo.h"

MoveInfo::MoveInfo(const Pos& _from, const Pos& _to, FigureType _activeFigure, FigureType _eatenFigure, bool* _castling)
	: activeFigure(_activeFigure), eatenFigure(_eatenFigure), from(_from), to(_to)
{
	for (int i = 0; i != 4; ++i)
		castling[i] = _castling[i];
}

MoveInfo::MoveInfo(const MoveInfo& copy)
{
	activeFigure = copy.activeFigure;
	eatenFigure = copy.eatenFigure;
	from = copy.from;
	to = copy.to;
	for (int i = 0; i != 4; ++i)
		castling[i] = copy.castling[i];
}

bool MoveInfo::isEatenFigureExists() const
{
	return eatenFigure != FigureType::Empty;
}