#include "MoveInfo.h"

const MoveInfo MoveInfo::NULL_INFO(Pos::NULL_POS, Pos::NULL_POS, FigureType::Empty, FigureType::Empty, NULL);

MoveInfo::MoveInfo(const MoveInfo& copy)
{
	activeFigure = copy.activeFigure;
	eatenFigure = copy.eatenFigure;
	from = copy.from;
	to = copy.to;
	additionalInfo = copy.additionalInfo;
}

bool MoveInfo::isEatenFigureExists() const
{
	return eatenFigure != FigureType::Empty;
}

bool MoveInfo::operator==(const MoveInfo& comp) const
{
	if (activeFigure != comp.activeFigure
		|| eatenFigure != comp.eatenFigure
		|| from != comp.from 
		|| to != comp.to
		|| additionalInfo != comp.additionalInfo)
		return false;
	return true;
}

bool MoveInfo::operator!=(const MoveInfo& comp) const
{
	return !(*this == comp);
}
