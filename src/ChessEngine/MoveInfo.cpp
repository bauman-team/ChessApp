#include "pch.h"
#include "MoveInfo.h"

const AdditionalInfo AdditionalInfo::NULL_INFO{ };
const MoveInfo MoveInfo::NULL_INFO{ };

inline bool AdditionalInfo::operator==(const AdditionalInfo comp) const
{
	if (castling != comp.castling
		|| isCaptureEnPassant != comp.isCaptureEnPassant)
		return false;
	return true;
}

inline AdditionalInfo::AdditionalInfo(std::array<bool, 4> _castling, bool _isCaptureEnPassant) : castling{ 0 }, isCaptureEnPassant{ 0 }
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

MoveInfo::MoveInfo() : from{ }, to{ }, activeFigure{ FigureType::Empty }, eatenFigure{ FigureType::Empty }, additionalInfo{ AdditionalInfo::NULL_INFO }, numOfMove{ 0 } { }

MoveInfo::MoveInfo(const MoveInfo& copy)
{
	activeFigure = copy.activeFigure;
	eatenFigure = copy.eatenFigure;
	from = copy.from;
	to = copy.to;
	additionalInfo = copy.additionalInfo;
	numOfMove = copy.numOfMove;
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
		|| additionalInfo != comp.additionalInfo
		|| numOfMove != comp.numOfMove)
		return false;
	return true;
}

bool MoveInfo::operator!=(const MoveInfo& comp) const
{
	return !(*this == comp);
}
