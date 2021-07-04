#pragma once
#include "Figure.h"

class CHESSENGINE_API MoveInfo // TODO: add using
{
	FigureType activeFigure;
	FigureType eatenFigure;
	Pos from;
	Pos to;
	uint8_t castling : 4;
	uint8_t isCaptureEnPassant : 1;
	uint16_t numOfMove;
public:
	MoveInfo() noexcept : from{ }, to{ }, activeFigure{ FigureType::Empty }, eatenFigure{ FigureType::Empty }, castling{ 0 }, isCaptureEnPassant{ 0 }, numOfMove{ 0 } { }
	MoveInfo(const Pos& _from, const Pos& _to, FigureType _activeFigure, FigureType _eatenFigure, std::array<bool, 4> _castling, bool _isCaptureEnPassant, uint16_t _numOfMove);

	auto isEatenFigureExists() const noexcept { return eatenFigure != FigureType::Empty; }

	auto GetTypeActiveFigure() const noexcept { return activeFigure; }
	auto GetTypeEatenFigure() const noexcept { return eatenFigure; }
	auto GetPosBeforeMove() const noexcept { return from; }
	auto GetPosAfterMove() const noexcept { return to; }
	auto GetNumOfMove() const noexcept { return numOfMove; }
	bool GetCaptureEnPassant() const noexcept { return isCaptureEnPassant; }
	std::array<bool, 4> GetPossibleCastling() const noexcept { return { static_cast<bool>(1 & castling), static_cast<bool>(2 & castling), static_cast<bool>(4 & castling), static_cast<bool>(8 & castling) }; }

	bool operator==(const MoveInfo& comp) const noexcept;
	bool operator!=(const MoveInfo& comp) const noexcept;

	const static MoveInfo NULL_INFO;
};