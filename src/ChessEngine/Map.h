#pragma once
#include "MoveInfo.h"

enum class CHESSENGINE_API GameStatus { Play, Shah, Mat, Pat, TimeIsOver, Exit };
enum class FigureType;
enum class Color;
enum class BoardPos;

class MoveInfo;
class Figure;
class PlayerWithAIGame;


struct CHESSENGINE_API OneFigureMoves
{
	Pos from;
	std::vector<Pos> to;
};


class CHESSENGINE_API Map
{
	friend class PlayerWithAIGame;

	uint64_t map[FIGURE_TYPES];
	std::vector<MoveInfo> movesHistory;
	std::vector<OneFigureMoves> allPossibleMoves;
	std::array<bool, 4> possibleCastling;
	uint16_t countOfMoves;

	void Move(const Pos& from, const Pos& to);
	void SetToEmpty(const Pos& target);
	void PawnToQueen(const Pos& target);
	void RookCastling(const Pos& kingFrom, const Pos& kingTo);
public:
	Map(); 
	Map(const Map& map);

	GameStatus CheckGameFinal(const Color activePlayerColor);

	std::vector<Pos> GetPossibleMovesFrom(const Pos& figurePosition) const;
	const std::vector<OneFigureMoves>& GetAllPossibleMoves() const { return allPossibleMoves; }

	void FindAllPossibleMoves(const Color& activeColor);

	bool MakeMove(const Pos& previousPosition, const Pos& nextPosition);
	void ClearPossibleMoves();
	
	void UndoMove();

	// functions for bot to easy do and undo different moves
	void DoImitationMove(const Pos& from, const Pos& to);
	void UndoImitationMove(const Pos& from, const Pos& to, FigureType eatenType);

	BoardPos CheckEmpty(const Pos& from, const Pos& to) const;

	bool IsShahFor(const Color kingColor) const noexcept;
	void EraseForbiddenMoves(OneFigureMoves& figureMoves);

	// castling methods
	bool IsCastlingAllowedForKing(const Pos& kingPos) const;
	bool IsCastlingAllowedWithRook(const Pos& rookPos, const Color& rookColor) const;
	void DisableCastlingForKing(const Color& kingColor);
	void DisableCastlingWithRook(const Pos& rookPos, const Color& rookColor);

	Color GetColor(const Pos& pos) const;
	Color GetColor(const FigureType type) const;
	FigureType GetFigureType(const Pos& pos) const;
	FigureType GetFigureType(const int index) const;

	const MoveInfo GetLastMoveInfo() const;
	const std::vector<MoveInfo>& GetMovesHistory() const;
	uint16_t GetMovesCount() const { return countOfMoves; }

	~Map(){}

	static const uint8_t offsetHorizontal;
	static const uint8_t offsetVertical;
	static const uint8_t offsetMainDiag;
	static const uint8_t offsetSideDiag;
	static const uint64_t knightBorderAB;
	static const uint64_t knightBorderGH;
	static const uint64_t mapLeftBorder;
	static const uint64_t mapRightBorder;
	static const uint64_t mapUpBorder;
	static const uint64_t mapDownBorder;
};