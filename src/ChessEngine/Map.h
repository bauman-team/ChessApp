#pragma once
#include "MoveInfo.h"
#include <mutex>

enum class CHESSENGINE_API GameStatus { Play, Shah, Mat, Pat, TimeIsOver, Exit };
enum class CHESSENGINE_API MoveStatus { Made, NeedFigureType, NotFound };

class CHESSENGINE_API Map
{
	std::array<uint64_t, FIGURE_TYPES>  map;
	std::vector<MoveInfo> movesLog;
	MovesInfo allPossibleMoves;
	std::array<bool, 4> possibleCastling; // first 2 elements for white king, last for black
	uint16_t countOfMoves;
public:
	Map() noexcept; 
	Map(const std::array<uint64_t, FIGURE_TYPES> _map) noexcept;
	Map(const Map& baseMap) noexcept;
	Map& operator=(const Map& baseMap) noexcept;

	GameStatus CheckGameFinal(const Color &activePlayerColor);

	std::vector<Pos> GetPossibleMovesFrom(const Pos& figurePosition) const;
	const auto GetAllPossibleMoves() const noexcept { return allPossibleMoves; }
	void FindAllPossibleMoves(const Color& activeColor); 
	MoveStatus MakeMove(const Pos& previousPosition, const Pos& nextPosition, const FigureType selectedFigure);
	void ClearPossibleMoves() noexcept { allPossibleMoves.clear(); }
	void Move(std::vector<MoveInfo> move);
	void UndoMove();

	// functions for bot to easy do and undo different moves
	void DoImitationMove(const Pos& from, const Pos& to) noexcept; // TODO: same method Move without MovesLog
	void UndoImitationMove(const Pos& from, const Pos& to, FigureType eatenType) noexcept;

	// methods for Castling
	auto GetPossibleCastling() const noexcept { return possibleCastling; }
	void DisableCastlingForKing(const Color& kingColor) noexcept;
	void DisableCastlingForRook(const Color& kingColor) noexcept;

	auto GetMap() const noexcept { return map; }
	auto GetFigureType(const Pos& pos) const noexcept { return GetFigureType(pos.ToIndex()); }
	FigureType GetFigureType(const int index) const noexcept;
	Color GetColor(const Pos& pos) const noexcept { return Figure::GetFigureTypeColor(GetFigureType(pos)); }
	auto GetColor(const FigureType type) const noexcept { return Figure::GetFigureTypeColor(type); }
	const MoveInfo GetLastMoveInfo() const noexcept;
	const auto& GetMovesLog() const noexcept { return movesLog; }
	auto GetMovesCount() const noexcept { return countOfMoves; }

	mutable std::mutex mut;

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