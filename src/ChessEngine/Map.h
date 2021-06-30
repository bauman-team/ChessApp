#pragma once
#include "MoveInfo.h"

enum class CHESSENGINE_API GameStatus { Play, Shah, Mat, Pat, TimeIsOver, Exit };
enum class CHESSENGINE_API MoveStatus { Made, NeedFigureType, NotFound };
enum class FigureType;
enum class Color;
enum class BoardPos;

class MoveInfo;
class Figure;


class CHESSENGINE_API Map
{
	std::array<uint64_t, FIGURE_TYPES>  map;
	std::vector<MoveInfo> movesLog;
	std::vector<std::vector<MoveInfo>> allPossibleMoves;
	std::array<bool, 4> possibleCastling; // first 2 elements for white king, last for black
	uint16_t countOfMoves;

public:
	Map(); 
	Map(const std::array<uint64_t, FIGURE_TYPES> _map);
	Map(const Map& map);
	Map(Map && map) noexcept;

	GameStatus CheckGameFinal(const Color &activePlayerColor);

	std::vector<Pos> GetPossibleMovesFrom(const Pos& figurePosition) const;
	const auto GetAllPossibleMoves() const { return allPossibleMoves; }

	void FindAllPossibleMoves(const Color& activeColor); 

	MoveStatus MakeMove(const Pos& previousPosition, const Pos& nextPosition, const FigureType selectedFigure);
	void ClearPossibleMoves();
	
	void Move(std::vector<MoveInfo> move);
	void UndoMove();

	// functions for bot to easy do and undo different moves
	void DoImitationMove(const Pos& from, const Pos& to) noexcept; // TODO: same method Move without MovesLog
	void UndoImitationMove(const Pos& from, const Pos& to, FigureType eatenType) noexcept;

	// methods for Castling
	auto GetPossibleCastling() const noexcept { return possibleCastling; }
	void DisableCastlingForKing(const Color& kingColor);
	void DisableCastlingForRook(const Color& kingColor);

	auto GetMap() const noexcept { return map; }

	Color GetColor(const Pos& pos) const noexcept;
	Color GetColor(const FigureType type) const noexcept;
	FigureType GetFigureType(const Pos& pos) const noexcept;
	FigureType GetFigureType(const int index) const;

	const MoveInfo GetLastMoveInfo() const noexcept;
	const std::vector<MoveInfo>& GetMovesLog() const;
	auto GetMovesCount() const { return countOfMoves; }

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