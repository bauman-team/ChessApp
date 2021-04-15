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
	void DoImitationMove(const Pos& from, const Pos& to) noexcept;
	void UndoImitationMove(const Pos& from, const Pos& to, FigureType eatenType) noexcept;

	BoardPos CheckEmpty(const Pos& from, const Pos& to) const noexcept;

	bool IsShahFor(const Color kingColor) const noexcept;
	void EraseForbiddenMoves(OneFigureMoves& figureMoves) noexcept;

	// castling methods
	bool IsCastlingAllowedForKing(const Pos& kingPos) const noexcept;
	bool IsCastlingAllowedWithRook(const Pos& rookPos, const Color& rookColor) const noexcept;
	void DisableCastlingForKing(const Color& kingColor);
	void DisableCastlingWithRook(const Pos& rookPos, const Color& rookColor);

	Color GetColor(const Pos& pos) const noexcept;
	Color GetColor(const FigureType type) const noexcept;
	FigureType GetFigureType(const Pos& pos) const noexcept;
	FigureType GetFigureType(const int index) const;

	const MoveInfo GetLastMoveInfo() const noexcept;
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