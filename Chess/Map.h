#pragma once
#include "Figure.h"
#include "Pos.h"
#include "MoveInfo.h"

enum class FigureType;
enum class Color;

class MoveInfo;
class Figure;
class PlayerWithAIGame;

struct PossibleMoves
{
	Pos figurePosition;
	std::vector<Pos> possibleMoves; 
};

class Map
{
	friend class PlayerWithAIGame;

	uint64_t map[12]; // 12 types of figure
	std::vector<MoveInfo> movesHistory;
	std::vector<PossibleMoves> figureWithAccessMoves;
	bool possibleCastling[4];

public:
	Map(); 
	Map(const Map&);

	std::vector<Pos> GetPossibleMoves(const Pos&) const;
	const std::vector<PossibleMoves>& GetFigureWithAccessMoves() const { return figureWithAccessMoves; }

	void RunFindMoves(const Color& activeColor);
	bool RunMakeMove(const Pos& previousPosition, const Pos& nextPosition);
	void RunClearPossibleMoves();

	void Move(const Pos& from, const Pos& to);
	void UndoMove();

	// functions for bot to easy do and undo different moves
	void DoImitationMove(const Pos& from, const Pos& to);
	void UndoImitationMove(const Pos& from, const Pos& to, FigureType eatenType);

	void SetToEmpty(const Pos& target);
	void PawnToQueen(const Pos& target);
	void Castling(const Pos& from, const Pos& to);

	bool CheckingShah(const Pos& kingPos) const;
	void CheckingPossibleMove(PossibleMoves&);

	Color GetColor(const Pos& pos) const;
	Color GetColor(const FigureType type) const;
	FigureType GetFigureType(const Pos& pos) const;
	bool GetCastling(const Color& selectedColor) const;
	bool GetCastling(const Color& selectedColor, const Pos& selectedPos) const;

	void SetCastling(const Color& selectedColor);
	void SetCastling(const Color& selectedColor, const Pos& selectedPos);

	int8_t CheckEmpty(const Pos& from, const Pos& to) const;

	const MoveInfo* GetLastMoveInfo() const;
	const std::vector<MoveInfo>& GetMovesHistory() const;
	int GetMovesCount() const { return movesHistory.size(); }

	~Map(){}
};