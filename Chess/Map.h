#pragma once
#include "Figure.h"
#include "Pos.h"
#include <vector>
#include "MoveInfo.h"

class Figure;
enum class FigureType;
class MoveInfo;
enum class Color;
class PlayerWithAIGame;

class Map
{
	friend Figure; // delete this, King fuction need struct PossibleMoves
	friend PlayerWithAIGame;
	struct PossibleMoves
	{
		Pos *figurePosition;
		std::vector<Pos> *possibleMoves;
	};
	uint64_t map[12]; // 12 type of figure
	std::vector<MoveInfo> movesHistory;
	std::vector<PossibleMoves> *figureWithAccessMoves;
	bool possibleCastling[4];
public:
	Map(); 
	Map(const Map&);

	const std::vector<Pos>* GetPossibleMoves(const Pos&) const; // if moves is empty for selected position return nullptr
	std::vector<PossibleMoves>& GetFigureWithAccessMoves() const { return *figureWithAccessMoves; }

	void RunFindMoves(const Color& activeColor);
	bool RunMakeMove(const Pos& previousPosition, const Pos& nextPosition);
	void RunClearPossibleMoves();

	void Move(const Pos& from, const Pos& to);
	void SetToEmpty(const Pos& target);
	void PawnToQueen(const Pos& target);
	void Castling(const Pos& from, const Pos& to);

	bool CheckingShah(const Pos& kingPos);
	void CheckingPossibleMove(PossibleMoves&);

	Color GetColor(const Pos& pos) const;
	Color GetColor(const FigureType type) const;
	FigureType GetFigureType(const Pos& pos) const;
	bool GetCastling(const Color& selectedColor) const;
	bool GetCastling(const Color& selectedColor, const Pos& selectedPos) const;

	void SetCastling(const Color& selectedColor);
	void SetCastling(const Color& selectedColor, const Pos& selectedPos);

	int8_t CheckEmpty(const Pos& from, const Pos& to) const;

	MoveInfo* GetLastMoveInfo();

	std::vector<MoveInfo>& GetMovesHistory();

	~Map()
	{
		delete figureWithAccessMoves;
	}
};