#include "Map.h"
#include <cassert>

Map::Map()
{
	map[(int)(FigureType::Rook_white)] = 129;
	map[(int)(FigureType::Knight_white)] = 66;
	map[(int)(FigureType::Bishop_white)] = 36;
	map[(int)(FigureType::Queen_white)] = 8;
	map[(int)(FigureType::King_white)] = 16;
	map[(int)(FigureType::Pawn_white)] = 65'280;
	map[(int)(FigureType::Rook_black)] = 9'295'429'630'892'703'744;
	map[(int)(FigureType::Knight_black)] = 4'755'801'206'503'243'776;
	map[(int)(FigureType::Bishop_black)] = 2'594'073'385'365'405'696;
	map[(int)(FigureType::Queen_black)] = 576'460'752'303'423'488;
	map[(int)(FigureType::King_black)] = 1'152'921'504'606'846'976;
	map[(int)(FigureType::Pawn_black)] = 71'776'119'061'217'280;
	possibleCastling[0] = true;
	possibleCastling[1] = true;
	possibleCastling[2] = true;
	possibleCastling[3] = true;
	figureWithAccessMoves = new std::vector<PossibleMoves>;
}

Map::Map(const Map& baseMap)
{
	for (int i = 0; i != 12; ++i)
		map[i] = baseMap.map[i];
	possibleCastling[0] = baseMap.possibleCastling[0];
	possibleCastling[1] = baseMap.possibleCastling[1];
	possibleCastling[2] = baseMap.possibleCastling[2];
	possibleCastling[3] = baseMap.possibleCastling[3];	
	if (!baseMap.movesHistory.empty())
		movesHistory = baseMap.movesHistory;
	figureWithAccessMoves = new std::vector<PossibleMoves>;
}

const std::vector<Pos>* Map::GetPossibleMoves(const Pos& figurePosition) const
{
	assert(figurePosition != Pos::NULL_POS); // TODO: fix bug with invalid figurePosition
	if (!figureWithAccessMoves->empty())
	{
		std::vector<PossibleMoves>::const_iterator it = figureWithAccessMoves->begin(), end = figureWithAccessMoves->end();
		for (; it != end; ++it)
			if ((*it).figurePosition == figurePosition)
				return (*it).possibleMoves;
	}
	return nullptr;
}

void Map::RunFindMoves(const Color& activeColor)
{
	for (int i = 0; i != 12; ++i)
	{
		if (Figure::GetFigureTypeColor((FigureType)i) == activeColor)
		{
			uint64_t j = 1;
			while (j)
			{
				if (j & map[i])
				{
					PossibleMoves Moves;
					Moves.figurePosition = Pos::BitboardToPosition(j);
					Moves.possibleMoves = &Figure::FindPossibleMoves((FigureType)i, Moves.figurePosition, *this); // for checking shah give numberOfFigures
					CheckingPossibleMove(Moves);
					if (!Moves.possibleMoves->empty())
						figureWithAccessMoves->push_back(Moves);
					else
						delete Moves.possibleMoves;
				}
				j <<= 1;
			}
		}
	}
}

bool Map::RunMakeMove(const Pos& previousPosition, const Pos& nextPosition)
{
	std::vector<PossibleMoves>::const_iterator it1 = figureWithAccessMoves->begin(), end1 = figureWithAccessMoves->end();
	for (; it1 != end1; ++it1)
		if ((*it1).figurePosition == previousPosition)
		{
			std::vector<Pos>::const_iterator it2 = (*it1).possibleMoves->begin(), end2 = (*it1).possibleMoves->end();
			for (; it2 != end2; ++it2)
				if (*it2 == nextPosition)
				{
					Move(previousPosition, nextPosition);
					return true;
				}
			return false;
		}
	return false;
}

void Map::RunClearPossibleMoves()
{
	std::vector<PossibleMoves>::const_iterator it = figureWithAccessMoves->begin(), end = figureWithAccessMoves->end();
	for (; it != end; ++it)
		delete (*it).possibleMoves;
	figureWithAccessMoves->clear();
}

void Map::Move(const Pos& from, const Pos& to)
{
	FigureType movableFigure = GetFigureType(from), eatenFigure = GetFigureType(to);
	MoveInfo info(from, to, movableFigure);
	if (eatenFigure != FigureType::Empty)
	{
		info.SetEatenFigure(eatenFigure);
		map[(int)(eatenFigure)] -= to.ToBitboard();
		// TODO: decrease numOfFigures
	}
	map[(int)(movableFigure)] -= from.ToBitboard();
	map[(int)(movableFigure)] += to.ToBitboard();
	movesHistory.push_back(info);
	if (movableFigure == FigureType::King_black || movableFigure == FigureType::King_white)
	{
		SetCastling(Figure::GetFigureTypeColor(movableFigure));
		if (abs(from.GetX() - to.GetX()) == 2)
			Castling(from, to);
	}
	else if (movableFigure == FigureType::Rook_black || movableFigure == FigureType::Rook_white)
	{
		SetCastling(Figure::GetFigureTypeColor(movableFigure), from);
	}
	else if (movableFigure == FigureType::Pawn_black || movableFigure == FigureType::Pawn_white)
	{
		bool isCaptureEnPassant = abs(from.GetX() - to.GetX()) == 1 && abs(from.GetY() - to.GetY()) == 1;
		int lastCoordY = from.GetY();
		if (eatenFigure == FigureType::Empty && isCaptureEnPassant)
			SetToEmpty(Pos(to.GetX(), lastCoordY));
		if (Figure::GetFigureTypeColor(movableFigure) == Color::Black && to.GetY() == 0 || Figure::GetFigureTypeColor(movableFigure) == Color::White && to.GetY() == 7)
			PawnToQueen(to);
	}
}

void Map::DoImitationMove(const Pos& from, const Pos& to)
{
	FigureType moved = GetFigureType(from);
	FigureType eaten = GetFigureType(to);
	if (eaten != FigureType::Empty)
		map[(int)eaten] -= to.ToBitboard();
	map[(int)moved] -= from.ToBitboard();
	map[(int)moved] += to.ToBitboard();
}

void Map::UndoImitationMove(const Pos& from, const Pos& to, FigureType eatenType)
{
	FigureType moved = GetFigureType(to);
	FigureType eaten = eatenType;
	if (eaten != FigureType::Empty)
		map[(int)eaten] += to.ToBitboard();
	map[(int)moved] -= to.ToBitboard();
	map[(int)moved] += from.ToBitboard();
	
}

void Map::SetToEmpty(const Pos& target)
{
	assert(target.IsValid());
	FigureType eatenFigure = GetFigureType(target);
	movesHistory.back().SetEatenFigure(eatenFigure);
	map[(int)(eatenFigure)] -= target.ToBitboard();
}

void Map::PawnToQueen(const Pos& target)
{
	FigureType movableFigure = GetFigureType(target);
	FigureType Queen = Figure::GetFigureTypeColor(movableFigure) == Color::White ? FigureType::Queen_white : FigureType::Queen_black;
	MoveInfo info(target, target, Queen);
	info.SetEatenFigure(movableFigure);
	movesHistory.push_back(info);
	map[(int)(movableFigure)] -= target.ToBitboard();
	map[(int)(Queen)] += target.ToBitboard();
}

void Map::Castling(const Pos& from, const Pos& to)
{
	Pos rookFrom;
	Pos rookTo;
	int y = (GetColor(to) == Color::Black) ? 7 : 0;
	if (from.GetX() > to.GetX())
	{
		rookFrom = Pos(0, y);
		rookTo = Pos(3, y);
	}
	else
	{
		rookFrom = Pos(7, y);
		rookTo = Pos(5, y);
	}
	Move(rookFrom, rookTo);
}

bool Map::CheckingShah(const Pos& kingPos) const
{
	if (true) // numOfFigures
	{ // high // method FROM KING
		Color kingColor = GetColor(kingPos);
		Pos selectedPosition;
		bool isChecked;
		FigureType selectedFigureType; // 8 directions
		for (int x, y, i = 0; i != 8; ++i)
		{
			x = 0; y = 0;
			isChecked = false;
			do 
			{
				/*
					0 | 7 | 6
					---------
					1 | K | 5
					---------
					2 | 3 | 4
				*/
				if (i == 0 || i == 1 || i == 2) // 0 - up && left; 1 - left; 2 - down && left; ... (counter-clockwise)
					--x;
				else if (i == 4 || i == 5 || i == 6)
					++x;
				if (i == 0 || i == 7 || i == 6)
					++y;
				else if (i == 2 || i == 3 || i == 4)
					--y;
				selectedPosition = kingPos.Add(x, y);
				if (selectedPosition.IsValid()) // over the edge of the map
				{
					selectedFigureType = GetFigureType(selectedPosition);
					if (selectedFigureType != FigureType::Empty)
					{
						if (GetColor(selectedFigureType) != kingColor) // opponent figure 
						{
							if (selectedFigureType == FigureType::Queen_black || selectedFigureType == FigureType::Queen_white) // all
								return true;
							if (i % 2 && (selectedFigureType == FigureType::Rook_black || selectedFigureType == FigureType::Rook_white)) // 1, 3, 5, 7 
								return true;
							if (!(i % 2) && (selectedFigureType == FigureType::Bishop_black || selectedFigureType == FigureType::Bishop_white)) // 0, 2, 4, 6
								return true;			
							if (abs(x) < 2 && abs(y) < 2 && (selectedFigureType == FigureType::King_black || selectedFigureType == FigureType::King_white))
								return true;
						}
						isChecked = true;
					}
				}
				else
				{
					isChecked = true;
				}
						
			} while (!isChecked);
		}
		// pawn
		if (kingColor == Color::White)
		{
			selectedPosition = kingPos.Add(-1, 1);
			if (selectedPosition.IsValid())
				if (GetFigureType(selectedPosition) == FigureType::Pawn_black)
					return true;
			selectedPosition = kingPos.Add(1, 1);
			if (selectedPosition.IsValid())
				if (GetFigureType(selectedPosition) == FigureType::Pawn_black)
					return true;
		}
		else
		{
			selectedPosition = kingPos.Add(-1, -1);
			if (selectedPosition.IsValid())
				if (GetFigureType(selectedPosition) == FigureType::Pawn_white)
					return true;
			selectedPosition = kingPos.Add(1, -1);
			if (selectedPosition.IsValid())
				if (GetFigureType(selectedPosition) == FigureType::Pawn_white)
					return true;
		}
		// knight
		FigureType type;
		for (int i = 0; i != 2; ++i)
		{
			selectedPosition = kingPos.Add((i % 2 + 1), ((i + 1) % 2 + 1));
			if (selectedPosition.IsValid())
			{
				type = GetFigureType(selectedPosition);
				if (type == FigureType::Knight_black || type == FigureType::Knight_white)
					if (kingColor != GetColor(type))
						return true;
			}

			selectedPosition = kingPos.Add(-(i % 2 + 1), ((i + 1) % 2 + 1));
			if (selectedPosition.IsValid())
			{
				type = GetFigureType(selectedPosition);
				if (type == FigureType::Knight_black || type == FigureType::Knight_white)
					if (kingColor != GetColor(type))
						return true;
			}

			selectedPosition = kingPos.Add((i % 2 + 1), -((i + 1) % 2 + 1));
			if (selectedPosition.IsValid())
			{
				type = GetFigureType(selectedPosition);
				if (type == FigureType::Knight_black || type == FigureType::Knight_white)
					if (kingColor != GetColor(type))
						return true;
			}

			selectedPosition = kingPos.Add(-(i % 2 + 1), -((i + 1) % 2 + 1));
			if (selectedPosition.IsValid())
			{
				type = GetFigureType(selectedPosition);
				if (type == FigureType::Knight_black || type == FigureType::Knight_white)
					if (kingColor != GetColor(type))
						return true;
			}
		}
		return false;
	}
	else
	{ // low // method TO KING

	}
}

void Map::CheckingPossibleMove(PossibleMoves& figureMoves)
{
	if (!figureMoves.possibleMoves->empty())
	{
		Pos posMainFigure = figureMoves.figurePosition;
		FigureType mainFigureType = GetFigureType(posMainFigure), secondaryFigureType;
		uint64_t mainBitboard = posMainFigure.ToBitboard(), secondBitboard;
		std::vector<Pos>::iterator it = figureMoves.possibleMoves->begin();
		uint8_t mainFigureIndex = (int)(mainFigureType);
		Pos kingPos = Pos::NULL_POS;

		if (mainFigureType == FigureType::King_black || mainFigureType == FigureType::King_white)
			kingPos = posMainFigure;
		else
			kingPos = Pos::BitboardToPosition(map[(int)(Figure::GetFigureTypeColor(mainFigureType) == Color::Black ? FigureType::King_black : FigureType::King_white)]);

		for (; it != figureMoves.possibleMoves->end();)
		{
			secondaryFigureType = GetFigureType(*it);
			secondBitboard = (*it).ToBitboard();
			map[mainFigureIndex] -= mainBitboard;
			map[mainFigureIndex] += secondBitboard;
			if (secondaryFigureType == FigureType::Empty)
			{
				if (kingPos == posMainFigure)
				{
					if (CheckingShah(*it))
						it = figureMoves.possibleMoves->erase(it);
					else
						++it;
				}
				else
				{
					if (CheckingShah(kingPos))
						it = figureMoves.possibleMoves->erase(it);
					else
						++it;
				}
			}
			else
			{
				map[(int)(secondaryFigureType)] -= secondBitboard;
				if (kingPos == posMainFigure)
				{
					if (CheckingShah(*it))
						it = figureMoves.possibleMoves->erase(it);
					else
						++it;
				}
				else
				{
					if (CheckingShah(kingPos))
						it = figureMoves.possibleMoves->erase(it);
					else
						++it;
				}
				map[(int)(secondaryFigureType)] += secondBitboard;
			}
			map[mainFigureIndex] += mainBitboard;
			map[mainFigureIndex] -= secondBitboard;
		}
	}
}

Color Map::GetColor(const Pos& pos) const
{
	FigureType type = GetFigureType(pos);
	if (type != FigureType::Empty)
		return Figure::GetFigureTypeColor(type);
	return Color::None;
}

Color Map::GetColor(const FigureType type) const
{
	if (type != FigureType::Empty)
		return Figure::GetFigureTypeColor(type);
	return Color::None;
}

FigureType Map::GetFigureType(const Pos& pos) const
{
	uint64_t bitboard = 1ULL << pos.ToIndex();
	for (int i = 0; i != 12; ++i)
		if (map[i] & bitboard)
			return (FigureType)i;
	return FigureType::Empty;
}

bool Map::GetCastling(const Color& selectedColor) const
{
	return possibleCastling[(int)(selectedColor)] || possibleCastling[(int)(selectedColor) + 1]; // TODO: why ||
}

bool Map::GetCastling(const Color& selectedColor, const Pos& selectedPos) const
{
	int kingCoeff = 2 * (int)(selectedColor);
	if (selectedPos == Pos(0, 0) || selectedPos == Pos(0, 7))
		return possibleCastling[kingCoeff];
	if (selectedPos == Pos(7, 0) || selectedPos == Pos(7, 7))
		return possibleCastling[kingCoeff + 1];
}

void Map::SetCastling(const Color& selectedColor)
{
	int kingCoeff = 2 * (int)(selectedColor);
	possibleCastling[kingCoeff] = false;
	possibleCastling[kingCoeff + 1] = false;
}

void Map::SetCastling(const Color& selectedColor, const Pos& selectedPos)
{
	int kingCoeff = 2 * (int)(selectedColor);
	if (selectedPos == Pos(0, 0) || selectedPos == Pos(0, 7))
		possibleCastling[kingCoeff] = false;
	if (selectedPos == Pos(7, 0) || selectedPos == Pos(7, 7))
		possibleCastling[kingCoeff + 1] = false;
}

int8_t Map::CheckEmpty(const Pos& from, const Pos& to) const
{
	if (to.IsValid())
	{
		if (GetFigureType(to) == FigureType::Empty)
			return 1; // if Pos to is Empty
		if (GetColor(from) != GetColor(to))
			return 2; // if Pos contains the figure with opposite color
	}
	return 0; // if Pos contains the figure with same color or output border
}

const MoveInfo* Map::GetLastMoveInfo() const
{
	return (!movesHistory.empty()) ? &movesHistory.back() : nullptr;
}

const std::vector<MoveInfo>& Map::GetMovesHistory() const
{
	return movesHistory;
}