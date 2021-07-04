#include "pch.h"
#include "MoveInfo.h"

const MoveInfo MoveInfo::NULL_INFO{ };

MoveInfo::MoveInfo(const Pos& _from, const Pos& _to, FigureType _activeFigure, FigureType _eatenFigure, std::array<bool, 4> _castling, bool _isCaptureEnPassant, uint16_t _numOfMove)
	: activeFigure{ _activeFigure }, eatenFigure{ _eatenFigure }, from{ _from }, to{ _to }, castling{ 0 }, isCaptureEnPassant{ 0 }, numOfMove{ _numOfMove }
{
	auto add{ 1 };
	if (_isCaptureEnPassant)
		isCaptureEnPassant = add;
	for (auto i = 0; i != 4; ++i)
	{
		if (_castling[i])
			castling |= add;
		add <<= 1;
	}
}

bool MoveInfo::operator==(const MoveInfo& comp) const noexcept
{
	if (activeFigure != comp.activeFigure
		|| eatenFigure != comp.eatenFigure
		|| from != comp.from 
		|| to != comp.to
		|| castling != comp.castling
		|| isCaptureEnPassant != comp.isCaptureEnPassant
		|| numOfMove != comp.numOfMove)
		return false;
	return true;
}

bool MoveInfo::operator!=(const MoveInfo& comp) const noexcept
{
	return !(*this == comp);
}
