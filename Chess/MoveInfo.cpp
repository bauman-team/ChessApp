#include "MoveInfo.h"

const MoveInfo MoveInfo::NULL_INFO(Pos::NULL_POS, Pos::NULL_POS, FigureType::Empty, FigureType::Empty, nullptr);

MoveInfo::MoveInfo(const Pos& _from, const Pos& _to, FigureType _activeFigure, FigureType _eatenFigure, bool* _castling)
	: activeFigure(_activeFigure), eatenFigure(_eatenFigure), from(_from), to(_to)
{
	if (_castling)
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

bool MoveInfo::operator==(const MoveInfo& comp) const
{
	if (activeFigure != comp.activeFigure &&
		eatenFigure != comp.eatenFigure &&
		from != comp.from &&
		to != comp.to)
		return false;
	for (int i = 0; i != 4; ++i)
		if (castling[i] != comp.castling[i])
			return false;
	return true;
}

bool MoveInfo::operator!=(const MoveInfo& comp) const
{
	return !(*this == comp);
}
