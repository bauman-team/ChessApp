#include "MoveInfo.h"

MoveInfo::MoveInfo(const Pos& _from, const Pos& _to, FigureType _activeFigure)
	: activeFigure(_activeFigure), eatenFigure(FigureType::Empty), from(_from), to(_to) {}

MoveInfo::MoveInfo(const MoveInfo& copy)
{
	activeFigure = copy.activeFigure;
	eatenFigure = copy.eatenFigure;
	from = copy.from;
	to = copy.to;
}

bool MoveInfo::isEatenFigureExists() const
{
	return eatenFigure != FigureType::Empty;
}