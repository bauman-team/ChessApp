#pragma once
#include "Figure.h"
#include "Pos.h"
#include "MoveInfo.h"

enum class FigureType;
enum class Color;

class MoveInfo;
class Figure;
class PlayerWithAIGame;

struct OneFigureMoves
{
	Pos from;
	std::vector<Pos> to;
};


class Map
{
	friend class PlayerWithAIGame;

	uint64_t map[12];
	std::vector<MoveInfo> movesHistory;
	std::vector<OneFigureMoves> allPossibleMoves;
	bool possibleCastling[4];

	void Move(const Pos& from, const Pos& to);
	void SetToEmpty(const Pos& target);
	void PawnToQueen(const Pos& target);
	void RookCastling(const Pos& from, const Pos& to);
public:
	Map(); 
	Map(const Map& map);

	std::vector<Pos> GetPossibleMovesFrom(const Pos& figurePosition) const;
	const std::vector<OneFigureMoves>& GetAllPossibleMoves() const { return allPossibleMoves; }

	void FindAllPossibleMoves(const Color& activeColor);

	bool MakeMove(const Pos& previousPosition, const Pos& nextPosition);
	void ClearPossibleMoves();
	
	void UndoMove();

	// functions for bot to easy do and undo different moves
	void DoImitationMove(const Pos& from, const Pos& to);
	void UndoImitationMove(const Pos& from, const Pos& to, FigureType eatenType);

	int8_t CheckEmpty(const Pos& from, const Pos& to) const;

	bool IsShahFor(const Pos& kingPos) const;
	void EraseForbiddenMoves(OneFigureMoves& figureMoves);

	// castling methods
	bool IsCastlingAllowedForKing(const Pos& kingPos) const;
	bool IsCastlingAllowedWithRook(const Pos& rookPos) const;
	void DisableCastlingForKing(const Color& kingColor);
	void DisableCastlingWithRook(const Pos& rookPos, const Color& rookColor);

	Color GetColor(const Pos& pos) const;
	Color GetColor(const FigureType type) const;
	FigureType GetFigureType(const Pos& pos) const;

	const MoveInfo GetLastMoveInfo() const;
	const std::vector<MoveInfo>& GetMovesHistory() const;
	int GetMovesCount() const { return movesHistory.size(); }

	~Map(){}
};