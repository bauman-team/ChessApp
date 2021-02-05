#pragma once
#include "Figure.h"

class Figure;
enum class FigureType;

class MoveInfo
{
	FigureType activeFigure;
	FigureType eatenFigure;
	Pos from;
	Pos to;
	uint8_t additionalInfo; // bitboard: {3bit empty}{4bit about castling}{1bit about CaptureEnPassant}
public:
	MoveInfo(const Pos& _from, const Pos& _to, FigureType _activeFigure, FigureType _eatenFigure, uint8_t _additionalInfo)
		: activeFigure(_activeFigure), eatenFigure(_eatenFigure), from(_from), to(_to), additionalInfo(_additionalInfo) { }
	MoveInfo(const MoveInfo& copy);

	bool isEatenFigureExists() const;

	FigureType GetTypeActiveFigure() const { return activeFigure; }
	FigureType GetTypeEatenFigure() const { return eatenFigure; }
	Pos GetPosBeforeMove() const { return from; }
	Pos GetPosAfterMove() const { return to; }
	const uint8_t GetAdditionalInfo() const { return additionalInfo; }

	void SetEatenFigure(const FigureType _eatenFigure) { eatenFigure = _eatenFigure; }
	void SetCaptureEnPassant() { additionalInfo |= 16; } 

	bool operator==(const MoveInfo& comp) const;
	bool operator!=(const MoveInfo& comp) const;

	const static MoveInfo NULL_INFO;
};