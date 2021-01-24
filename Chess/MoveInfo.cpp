#include "MoveInfo.h"

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