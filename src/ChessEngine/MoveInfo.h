#pragma once
#include "Figure.h"

class Figure;
enum class FigureType;

struct CHESSENGINE_API AdditionalInfo // fieldbite
{
	uint8_t castling : 4;
	uint8_t isCaptureEnPassant : 1; // TODO: only for logs, maybe delete?

	AdditionalInfo() : castling{ 0 }, isCaptureEnPassant{ 0 } { }
	AdditionalInfo(std::array<bool, 4> _castling, bool _isCaptureEnPassant);
	bool operator==(const AdditionalInfo comp) const;
	bool operator!=(const AdditionalInfo comp) const { return !(*this == comp); }
	void SetCaptureEnPassant() { isCaptureEnPassant = 1; }
	bool GetCaptureEnPassant() const { return isCaptureEnPassant; }
	std::array<bool, 4> GetPossibleCastling() const { return { static_cast<bool>(1 & castling), static_cast<bool>(2 & castling), static_cast<bool>(4 & castling), static_cast<bool>(8 & castling) }; }
	const static AdditionalInfo NULL_INFO;
};


class CHESSENGINE_API MoveInfo
{
	FigureType activeFigure;
	FigureType eatenFigure;
	Pos from;
	Pos to;
	AdditionalInfo additionalInfo;
	uint16_t numOfMove;
public:
	MoveInfo();

	MoveInfo(const Pos& _from, const Pos& _to, FigureType _activeFigure, FigureType _eatenFigure, std::array<bool, 4> _castling, bool _isCaptureEnPassant, uint16_t _numOfMove)
		: activeFigure{ _activeFigure }, eatenFigure{ _eatenFigure }, from{ _from }, to{ _to }, additionalInfo{ _castling, _isCaptureEnPassant }, numOfMove{ _numOfMove } { }

	MoveInfo(const Pos& _from, const Pos& _to, FigureType _activeFigure, FigureType _eatenFigure, AdditionalInfo _additionalInfo, uint16_t _numOfMove)
		: activeFigure{ _activeFigure }, eatenFigure{ _eatenFigure }, from{ _from }, to{ _to }, additionalInfo{ _additionalInfo }, numOfMove{ _numOfMove } { }

	bool isEatenFigureExists() const;

	FigureType GetTypeActiveFigure() const noexcept { return activeFigure; }
	FigureType GetTypeEatenFigure() const { return eatenFigure; }
	Pos GetPosBeforeMove() const noexcept { return from; }
	Pos GetPosAfterMove() const noexcept { return to; }
	uint16_t GetNumOfMove() const { return numOfMove; }

	void SetEatenFigure(const FigureType _eatenFigure) { eatenFigure = _eatenFigure; }
	void SetCaptureEnPassant() { additionalInfo.SetCaptureEnPassant(); }

	bool GetCaptureEnPassant() const { return additionalInfo.GetCaptureEnPassant(); }
	std::array<bool, 4> GetPossibleCastling() const { return additionalInfo.GetPossibleCastling(); }

	bool operator==(const MoveInfo& comp) const;
	bool operator!=(const MoveInfo& comp) const;

	const static MoveInfo NULL_INFO;
};