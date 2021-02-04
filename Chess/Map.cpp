#include "Map.h"
#include <cassert>
#include <mutex>
#include <SFML/Window.hpp>

extern std::mutex mut1; // TODO: extern, but is not connection with TwoPlayersGame, why it works
const uint8_t Map::offsetHorizontal{ 1 }, Map::offsetVertical{ 8 }, Map::offsetMainDiag{ 7 }, Map::offsetSideDiag{ 9 };
const uint64_t Map::knightBorderAB{ 18'229'723'555'195'321'596 }, Map::knightBorderGH{ 4'557'430'888'798'830'399 },
Map::mapLeftBorder{ 72'340'172'838'076'673 }, Map::mapRightBorder{ 9'259'542'123'273'814'144 },
Map::mapUpBorder{ 18'374'686'479'671'623'680 }, Map::mapDownBorder{ 255 };

Map::Map()
{ // 0 - left down
	map[static_cast<int>(FigureType::Rook_black)] = 9'295'429'630'892'703'744;
	map[static_cast<int>(FigureType::Knight_black)] = 4'755'801'206'503'243'776;
	map[static_cast<int>(FigureType::Bishop_black)] = 2'594'073'385'365'405'696;
	map[static_cast<int>(FigureType::Queen_black)] = 576'460'752'303'423'488;
	map[static_cast<int>(FigureType::King_black)] = 1'152'921'504'606'846'976;
	map[static_cast<int>(FigureType::Pawn_black)] = 71'776'119'061'217'280;
	map[static_cast<int>(FigureType::Rook_white)] = 129;
	map[static_cast<int>(FigureType::Knight_white)] = 66;
	map[static_cast<int>(FigureType::Bishop_white)] = 36;
	map[static_cast<int>(FigureType::Queen_white)] = 8;
	map[static_cast<int>(FigureType::King_white)] = 16;
	map[static_cast<int>(FigureType::Pawn_white)] = 65'280;
	for (int i = 0; i != 4; ++i)
		possibleCastling[i] = true;
}

Map::Map(const Map& baseMap)
{
	for (int i = 0; i != 12; ++i)
	{
		map[i] = baseMap.map[i];
		if (i < 4)
			possibleCastling[i] = baseMap.possibleCastling[i];
	}
	if (!baseMap.movesHistory.empty())
		movesHistory = baseMap.movesHistory;
}

std::vector<Pos> Map::GetPossibleMovesFrom(const Pos& figurePosition) const
{
	assert(figurePosition != Pos::NULL_POS);
	if (!allPossibleMoves.empty())
	{
		std::vector<OneFigureMoves>::const_iterator it = allPossibleMoves.begin(), end = allPossibleMoves.end();
		for (; it != end; ++it)
			if (it->from == figurePosition)
				return it->to;
	}
	return {}; // returns empty vector
}

void Map::FindAllPossibleMoves(const Color& activeColor, const bool isThreading)
{
	int start = (activeColor == Color::Black ? 0 : 6); // used fixed enum order
	for (int i = start; i != start + 6; ++i)
	{
		uint64_t j = 1;
		while (j) // check all positions
		{
			if (j & map[i]) // is selected figure position
			{
				OneFigureMoves moves;
				moves.from = Pos::BitboardToPosition(j);
				moves.to = Figure::FindPossibleMoves(moves.from, (FigureType)i, *this); // find figure possible moves without checking shah 
				if (!isThreading)
					mut1.lock();
				EraseForbiddenMoves(moves); // TODO: maybe call eraseForbiddenMoves() in FindPossibleMoves() ?
				if (!moves.to.empty())
					allPossibleMoves.push_back(std::move(moves));
				if (!isThreading)
					mut1.unlock();
			}
			j <<= 1;
		}
	}
}

bool Map::MakeMove(const Pos& previousPosition, const Pos& nextPosition)
{
	std::vector<OneFigureMoves>::const_iterator it1 = allPossibleMoves.begin(), end1 = allPossibleMoves.end();

	for (; it1 != end1; ++it1) // search figure in the vector of figure with posiible moves
		if (it1->from == previousPosition)
		{
			std::vector<Pos>::const_iterator it2 = it1->to.begin(), end2 = it1->to.end();
			for (; it2 != end2; ++it2) // search move in the vector of possible moves
				if (*it2 == nextPosition)
				{
					mut1.lock();
					Move(previousPosition, nextPosition);
					mut1.unlock();
					return true;
				}
			return false;
		}
	return false;
}

void Map::ClearPossibleMoves()
{
	allPossibleMoves.clear();
}

void Map::Move(const Pos& from, const Pos& to)
{
	FigureType activeFigureType = GetFigureType(from), eatenFigureType = GetFigureType(to);
	MoveInfo info(from, to, activeFigureType, eatenFigureType, possibleCastling); 
	movesHistory.push_back(info); // save info about move
	if (eatenFigureType != FigureType::Empty)
	{
		map[static_cast<int>(eatenFigureType)] -= to.ToBitboard();
		// TODO: decrease numOfFigures
	}
	map[static_cast<int>(activeFigureType)] -= from.ToBitboard();
	map[static_cast<int>(activeFigureType)] += to.ToBitboard();

	if (activeFigureType == FigureType::Pawn_black || activeFigureType == FigureType::Pawn_white)
	{
		// checking capture en passant
		bool isCaptureEnPassant = abs(from.GetX() - to.GetX()) == 1 && abs(from.GetY() - to.GetY()) == 1;
		int lastCoordY = from.GetY();
		if (eatenFigureType == FigureType::Empty && isCaptureEnPassant) // if Pawn eat on passage
			SetToEmpty(Pos(to.GetX(), lastCoordY));

		// checking transform pawn to queen
		if (Figure::GetFigureTypeColor(activeFigureType) == Color::Black && to.GetY() == 0 ||
			Figure::GetFigureTypeColor(activeFigureType) == Color::White && to.GetY() == 7) // if Pawn on the last line
			PawnToQueen(to);
	}
	else if (activeFigureType == FigureType::Rook_black || activeFigureType == FigureType::Rook_white)
	{
		DisableCastlingWithRook(from, GetColor(activeFigureType)); // disable opportunity of castling for Rook
	}
	else if (activeFigureType == FigureType::King_black || activeFigureType == FigureType::King_white)
	{
		if (abs(from.GetX() - to.GetX()) == 2)
			RookCastling(from, to); 
		DisableCastlingForKing(GetColor(activeFigureType)); // disable opportunity of castling for King
	}
}

void Map::UndoMove()
{
	MoveInfo info(GetLastMoveInfo());
	movesHistory.pop_back();
	if (GetLastMoveInfo() != MoveInfo::NULL_INFO &&
		GetColor(GetLastMoveInfo().GetTypeActiveFigure()) == GetColor(info.GetTypeActiveFigure()))
	{
		if (info.GetTypeEatenFigure() != FigureType::Empty)
			map[static_cast<int>(info.GetTypeEatenFigure())] += info.GetPosAfterMove().ToBitboard();
		map[static_cast<int>(info.GetTypeActiveFigure())] -= info.GetPosAfterMove().ToBitboard();
		map[static_cast<int>(info.GetTypeActiveFigure())] += info.GetPosBeforeMove().ToBitboard();
		info = GetLastMoveInfo();
		movesHistory.pop_back();
	}
	if (info.GetTypeEatenFigure() != FigureType::Empty)
		map[static_cast<int>(info.GetTypeEatenFigure())] += info.GetPosAfterMove().ToBitboard();
	map[static_cast<int>(info.GetTypeActiveFigure())] -= info.GetPosAfterMove().ToBitboard();
	map[static_cast<int>(info.GetTypeActiveFigure())] += info.GetPosBeforeMove().ToBitboard();
	for (int i = 0; i != 4; ++i)
		possibleCastling[i] = info.GetCastlingInfo()[i];
}

void Map::DoImitationMove(const Pos& from, const Pos& to)
{
	FigureType moved = GetFigureType(from);
	FigureType eaten = GetFigureType(to);
	if (eaten != FigureType::Empty)
		map[static_cast<int>(eaten)] -= to.ToBitboard();
	map[static_cast<int>(moved)] -= from.ToBitboard();
	map[static_cast<int>(moved)] += to.ToBitboard();
}

void Map::UndoImitationMove(const Pos& from, const Pos& to, FigureType eatenType)
{
	FigureType moved = GetFigureType(to);
	FigureType eaten = eatenType;
	if (eaten != FigureType::Empty)
		map[static_cast<int>(eaten)] += to.ToBitboard();
	map[static_cast<int>(moved)] -= to.ToBitboard();
	map[static_cast<int>(moved)] += from.ToBitboard();
	
}

void Map::SetToEmpty(const Pos& target)
{
	assert(target.IsValid());
	FigureType eatenFigure = GetFigureType(target);
	movesHistory.back().SetEatenFigure(eatenFigure);
	map[static_cast<int>(eatenFigure)] -= target.ToBitboard();
}

void Map::PawnToQueen(const Pos& target)
{
	FigureType figureType = GetFigureType(target);
	FigureType Queen = Figure::GetFigureTypeColor(figureType) == Color::White ? FigureType::Queen_white : FigureType::Queen_black;
	MoveInfo info(target, target, Queen, figureType, possibleCastling);
	movesHistory.push_back(info);
	map[static_cast<int>(figureType)] -= target.ToBitboard();
	map[static_cast<int>(Queen)] += target.ToBitboard();
}

void Map::RookCastling(const Pos& kingFrom, const Pos& kingTo)
{
	Pos rookFrom;
	Pos rookTo;
	int y = (GetColor(kingTo) == Color::Black) ? 7 : 0;
	if (kingFrom.GetX() > kingTo.GetX())
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

bool Map::IsShahFor(const Pos& kingPos) const
{
	FigureType king = GetFigureType(kingPos);
	uint64_t defendStraight, defendDiagonal, attackedStraight, attackedDiagonal, attackedKnight,
		mapSpareBorder, kingBitdoard = map[static_cast<int>(king)];
	// Knight
	attackedKnight = knightBorderGH & (kingBitdoard << 6 | kingBitdoard >> 10)
		| ~mapRightBorder & (kingBitdoard << 15 | kingBitdoard >> 17)
		| ~mapLeftBorder & (kingBitdoard << 17 | kingBitdoard >> 15)
		| knightBorderAB & (kingBitdoard << 10 | kingBitdoard >> 6);
	if (king == FigureType::King_black)
	{
		if (attackedKnight & map[static_cast<int>(FigureType::Knight_white)])
			return true;
		// Pawn
		if (!(kingBitdoard & mapDownBorder))
		{
			if (!(kingBitdoard & mapLeftBorder))
				if (kingBitdoard >> offsetSideDiag & map[static_cast<int>(FigureType::Pawn_white)])
					return true;
			if (!(kingBitdoard & mapRightBorder))
				if (kingBitdoard >> offsetMainDiag & map[static_cast<int>(FigureType::Pawn_white)])
					return true;
		}
		defendStraight = map[static_cast<int>(FigureType::Queen_black)]
			| map[static_cast<int>(FigureType::Bishop_black)] | map[static_cast<int>(FigureType::Knight_black)]
			| map[static_cast<int>(FigureType::Rook_black)] | map[static_cast<int>(FigureType::Pawn_black)]
			| map[static_cast<int>(FigureType::Pawn_white)] | map[static_cast<int>(FigureType::Knight_white)];
		defendDiagonal = defendStraight | map[static_cast<int>(FigureType::Rook_white)];
		defendStraight |= map[static_cast<int>(FigureType::Bishop_white)];

		attackedStraight = map[static_cast<int>(FigureType::Queen_white)] | map[static_cast<int>(FigureType::King_white)];
		attackedDiagonal = attackedStraight;
		attackedDiagonal |= map[static_cast<int>(FigureType::Bishop_white)];
		attackedStraight |= map[static_cast<int>(FigureType::Rook_white)];
	}
	else
	{
		if (attackedKnight & map[static_cast<int>(FigureType::Knight_black)])
			return true;
		// Pawn
		if (!(kingBitdoard & mapUpBorder))
		{
			if (!(kingBitdoard & mapLeftBorder))
				if (kingBitdoard << offsetMainDiag & map[static_cast<int>(FigureType::Pawn_black)])
					return true;
			if (!(kingBitdoard & mapRightBorder))
				if (kingBitdoard << offsetSideDiag & map[static_cast<int>(FigureType::Pawn_black)])
					return true;
		}
		defendStraight = map[static_cast<int>(FigureType::Queen_white)]
			| map[static_cast<int>(FigureType::Bishop_white)] | map[static_cast<int>(FigureType::Knight_white)]
			| map[static_cast<int>(FigureType::Rook_white)] | map[static_cast<int>(FigureType::Pawn_white)]
			| map[static_cast<int>(FigureType::Pawn_black)] | map[static_cast<int>(FigureType::Knight_black)];
		defendDiagonal = defendStraight | map[static_cast<int>(FigureType::Rook_black)];
		defendStraight |= map[static_cast<int>(FigureType::Bishop_black)];

		attackedStraight = map[static_cast<int>(FigureType::Queen_black)] | map[static_cast<int>(FigureType::King_black)];
		attackedDiagonal = attackedStraight;
		attackedDiagonal |= map[static_cast<int>(FigureType::Bishop_black)];
		attackedStraight |= map[static_cast<int>(FigureType::Rook_black)];
	}
	// Rook + Queen
	/*
		* | 0 | *
		---------
		3 | K | 1
		---------
		* | 2 | *
	*/
	mapSpareBorder = mapUpBorder | defendStraight;
	while (!(kingBitdoard & mapSpareBorder)) // 0
	{
		kingBitdoard <<= offsetVertical;
		if (kingBitdoard & attackedStraight)
			return true;
	}
	kingBitdoard = map[static_cast<int>(king)];
	mapSpareBorder = mapRightBorder | defendStraight;
	while (!(kingBitdoard & mapSpareBorder)) // 1
	{
		kingBitdoard <<= offsetHorizontal;
		if (kingBitdoard & attackedStraight)
			return true;
	}
	kingBitdoard = map[static_cast<int>(king)];
	mapSpareBorder = mapDownBorder | defendStraight;
	while (!(kingBitdoard & mapSpareBorder)) // 2
	{
		kingBitdoard >>= offsetVertical;
		if (kingBitdoard & attackedStraight)
			return true;
	}
	kingBitdoard = map[static_cast<int>(king)];
	mapSpareBorder = mapLeftBorder | defendStraight;
	while (!(kingBitdoard & mapSpareBorder)) // 3
	{
		kingBitdoard >>= offsetHorizontal;
		if (kingBitdoard & attackedStraight)
			return true;
	}
	kingBitdoard = map[static_cast<int>(king)];

	// Bishop + Queen
	/*
		0 | * | 1
		---------
		* | K | *
		---------
		3 | * | 2
	*/
	mapSpareBorder = mapLeftBorder | mapUpBorder | defendDiagonal; // angle
	while (!(kingBitdoard & mapSpareBorder)) // 0
	{
		kingBitdoard <<= offsetMainDiag;
		if (kingBitdoard & attackedDiagonal)
			return true;
	}
	kingBitdoard = map[static_cast<int>(king)];
	mapSpareBorder = mapUpBorder | mapRightBorder | defendDiagonal;
	while (!(kingBitdoard & mapSpareBorder)) // 1
	{
		kingBitdoard <<= offsetSideDiag;
		if (kingBitdoard & attackedDiagonal)
			return true;
	}
	kingBitdoard = map[static_cast<int>(king)];
	mapSpareBorder = mapRightBorder | mapDownBorder | defendDiagonal;
	while (!(kingBitdoard & mapSpareBorder)) // 2
	{
		kingBitdoard >>= offsetMainDiag;
		if (kingBitdoard & attackedDiagonal)
			return true;
	}
	kingBitdoard = map[static_cast<int>(king)];
	mapSpareBorder = mapDownBorder | mapLeftBorder | defendDiagonal;
	while (!(kingBitdoard & mapSpareBorder)) // 3
	{
		kingBitdoard >>= offsetSideDiag;
		if (kingBitdoard & attackedDiagonal)
			return true;
	}
	return false;

	/*
	Color kingColor = GetColor(kingPos);
	Pos selectedPosition;
	bool isChecked;
	FigureType selectedFigureType; // 8 directions

	// checking queen, rook, bishop and king attack
	for (int x, y, i = 0; i != 8; ++i)
	{
		x = 0; y = 0;
		isChecked = false;
		do 
		{
			
				//0 | 7 | 6
				//---------
				//1 | K | 5
				//---------
				//2 | 3 | 4
			
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
					if (GetColor(selectedFigureType) != kingColor) // different colors => opponent figure
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

	// checking pawn attack
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

	// checking knight attack
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
	*/
}

void Map::EraseForbiddenMoves(OneFigureMoves& figureMoves)
{
	if (!figureMoves.to.empty())
	{
		FigureType activeFigureType = GetFigureType(figureMoves.from), eatenFigureType;
		std::vector<Pos>::iterator posItr = figureMoves.to.begin();
		Pos kingPos;
		bool isShah;

		// finding king position
		if (activeFigureType == FigureType::King_black || activeFigureType == FigureType::King_white)
			kingPos = figureMoves.from; // chosen figure is king
		else
			kingPos = Pos::BitboardToPosition(map[static_cast<int>(Figure::GetFigureTypeColor(activeFigureType) == Color::Black ? FigureType::King_black : FigureType::King_white)]);

		// imitating all moves to find and erase moves after which the king is attacked
		for (; posItr != figureMoves.to.end();)
		{
			eatenFigureType = GetFigureType(*posItr);
			DoImitationMove(figureMoves.from, *posItr);
			isShah = IsShahFor(kingPos == figureMoves.from ? *posItr : kingPos);
			UndoImitationMove(figureMoves.from, *posItr, eatenFigureType);
			if (isShah)
				posItr = figureMoves.to.erase(posItr);
			else ++posItr;
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
	for (int i = 0; i != 12; ++i) // TODO: const count types of figures (12)
		if (map[i] & bitboard)
			return (FigureType)i;
	return FigureType::Empty;
}

bool Map::IsCastlingAllowedForKing(const Pos& kingPos) const // is castling possible
{
	Color kingColor = GetColor(kingPos);
	return possibleCastling[static_cast<int>(kingColor)] || possibleCastling[static_cast<int>(kingColor) + 1];
}

bool Map::IsCastlingAllowedWithRook(const Pos& rookPos) const // is castling possible with selected Rook
{
	int kingCoeff = 2 * static_cast<int>(GetColor(rookPos));
	if (rookPos == Pos(0, 0) || rookPos == Pos(0, 7))
		return possibleCastling[kingCoeff];
	if (rookPos == Pos(7, 0) || rookPos == Pos(7, 7))
		return possibleCastling[kingCoeff + 1];
	return false; // for incorrect rookPos
}

void Map::DisableCastlingForKing(const Color& kingColor) // if made a King move or was attacked disable possible castling
{
	int kingCoeff = 2 * static_cast<int>(kingColor);
	possibleCastling[kingCoeff] = false;
	possibleCastling[kingCoeff + 1] = false;
}

void Map::DisableCastlingWithRook(const Pos& rookPos, const Color & rookColor) // if made a Rook move disable possible castling with selected Rook
{
	int kingCoeff = 2 * static_cast<int>(rookColor);
	if (rookPos == Pos(0, 0) || rookPos == Pos(0, 7))
		possibleCastling[kingCoeff] = false;
	if (rookPos == Pos(7, 0) || rookPos == Pos(7, 7))
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

const MoveInfo Map::GetLastMoveInfo() const
{
	return (!movesHistory.empty()) ? movesHistory.back() : MoveInfo::NULL_INFO;
}

const std::vector<MoveInfo>& Map::GetMovesHistory() const
{
	return movesHistory;
}