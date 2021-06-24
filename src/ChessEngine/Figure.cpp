#include "pch.h"
#include "Figure.h"

constexpr auto DIAG_DIRECTIONS{ 4 };
constexpr auto STRAIGHT_DIRECTIONS{ 4 };

void Figure::EraseForbiddenMoves(std::vector<std::vector<MoveInfo>>& allFiguresMoves, Map& map) noexcept
{
	auto posItr{ allFiguresMoves.begin() };
	auto isShah{ false };
	auto activeColor{ GetFigureTypeColor((*posItr)[0].GetTypeActiveFigure()) };
	// imitating all moves to find and erase moves after which the king is attacked
	for (; posItr != allFiguresMoves.end();)
	{
		map.DoImitationMove((*posItr)[0].GetPosBeforeMove(), (*posItr)[0].GetPosAfterMove());
		isShah = IsShahFor(activeColor, map.GetMap());
		map.UndoImitationMove((*posItr)[0].GetPosBeforeMove(), (*posItr)[0].GetPosAfterMove(), (*posItr)[0].GetTypeEatenFigure());
		if (isShah)
			posItr = allFiguresMoves.erase(posItr);
		else ++posItr;
	}
}

bool Figure::IsShahFor(const Color kingColor, std::array<uint64_t, FIGURE_TYPES> map) noexcept
{
	auto king{ kingColor == Color::Black ? FigureType::King_black : FigureType::King_white };
	uint64_t defendStraight, defendDiagonal, attackedStraight, attackedDiagonal, attackedKingKnight,
		mapSpareBorder, kingBitdoard{ map[toUType(king)] };
	// King
	attackedKingKnight = ~Map::mapLeftBorder & (kingBitdoard << Map::offsetHorizontal | kingBitdoard >> Map::offsetMainDiag | kingBitdoard << Map::offsetSideDiag)
		| ~Map::mapRightBorder & (kingBitdoard >> Map::offsetHorizontal | kingBitdoard << Map::offsetMainDiag | kingBitdoard >> Map::offsetSideDiag)
		| kingBitdoard << Map::offsetVertical | kingBitdoard >> Map::offsetVertical;
	if (attackedKingKnight & map[toUType(king == FigureType::King_black ? FigureType::King_white : FigureType::King_black)])
		return true;
	// Knight
	attackedKingKnight = Map::knightBorderGH & (kingBitdoard << 6 | kingBitdoard >> 10)
		| ~Map::mapRightBorder & (kingBitdoard << 15 | kingBitdoard >> 17)
		| ~Map::mapLeftBorder & (kingBitdoard << 17 | kingBitdoard >> 15)
		| Map::knightBorderAB & (kingBitdoard << 10 | kingBitdoard >> 6);
	if (king == FigureType::King_black)
	{
		if (attackedKingKnight & map[toUType(FigureType::Knight_white)])
			return true;
		// Pawn
		if (!(kingBitdoard & Map::mapDownBorder))
		{
			if (!(kingBitdoard & Map::mapLeftBorder))
				if (kingBitdoard >> Map::offsetSideDiag & map[toUType(FigureType::Pawn_white)])
					return true;
			if (!(kingBitdoard & Map::mapRightBorder))
				if (kingBitdoard >> Map::offsetMainDiag & map[toUType(FigureType::Pawn_white)])
					return true;
		}
		defendStraight = map[toUType(FigureType::Queen_black)]
			| map[toUType(FigureType::Bishop_black)] | map[toUType(FigureType::Knight_black)]
			| map[toUType(FigureType::Rook_black)] | map[toUType(FigureType::Pawn_black)]
			| map[toUType(FigureType::Pawn_white)] | map[toUType(FigureType::Knight_white)]
			| map[toUType(FigureType::King_white)];
		defendDiagonal = defendStraight | map[toUType(FigureType::Rook_white)];
		defendStraight |= map[toUType(FigureType::Bishop_white)];

		attackedStraight = map[toUType(FigureType::Queen_white)];
		attackedDiagonal = attackedStraight | map[toUType(FigureType::Bishop_white)];
		attackedStraight |= map[toUType(FigureType::Rook_white)];
	}
	else
	{
		if (attackedKingKnight & map[toUType(FigureType::Knight_black)])
			return true;
		// Pawn
		if (!(kingBitdoard & Map::mapUpBorder))
		{
			if (!(kingBitdoard & Map::mapLeftBorder))
				if (kingBitdoard << Map::offsetMainDiag & map[toUType(FigureType::Pawn_black)])
					return true;
			if (!(kingBitdoard & Map::mapRightBorder))
				if (kingBitdoard << Map::offsetSideDiag & map[toUType(FigureType::Pawn_black)])
					return true;
		}
		defendStraight = map[toUType(FigureType::Queen_white)]
			| map[toUType(FigureType::Bishop_white)] | map[toUType(FigureType::Knight_white)]
			| map[toUType(FigureType::Rook_white)] | map[toUType(FigureType::Pawn_white)]
			| map[toUType(FigureType::Pawn_black)] | map[toUType(FigureType::Knight_black)]
			| map[toUType(FigureType::King_black)];
		defendDiagonal = defendStraight | map[toUType(FigureType::Rook_black)];
		defendStraight |= map[toUType(FigureType::Bishop_black)];

		attackedStraight = map[toUType(FigureType::Queen_black)];
		attackedDiagonal = attackedStraight | map[toUType(FigureType::Bishop_black)];
		attackedStraight |= map[toUType(FigureType::Rook_black)];
	}
	// Rook + Queen
	/*
		* | 0 | *
		---------
		3 | K | 1
		---------
		* | 2 | *
	*/
	mapSpareBorder = Map::mapUpBorder | defendStraight;
	while (!(kingBitdoard & mapSpareBorder)) // 0
	{
		kingBitdoard <<= Map::offsetVertical;
		if (kingBitdoard & attackedStraight)
			return true;
	}
	kingBitdoard = map[toUType(king)];
	mapSpareBorder = Map::mapRightBorder | defendStraight;
	while (!(kingBitdoard & mapSpareBorder)) // 1
	{
		kingBitdoard <<= Map::offsetHorizontal;
		if (kingBitdoard & attackedStraight)
			return true;
	}
	kingBitdoard = map[toUType(king)];
	mapSpareBorder = Map::mapDownBorder | defendStraight;
	while (!(kingBitdoard & mapSpareBorder)) // 2
	{
		kingBitdoard >>= Map::offsetVertical;
		if (kingBitdoard & attackedStraight)
			return true;
	}
	kingBitdoard = map[toUType(king)];
	mapSpareBorder = Map::mapLeftBorder | defendStraight;
	while (!(kingBitdoard & mapSpareBorder)) // 3
	{
		kingBitdoard >>= Map::offsetHorizontal;
		if (kingBitdoard & attackedStraight)
			return true;
	}
	kingBitdoard = map[toUType(king)];

	// Bishop + Queen
	/*
		0 | * | 1
		---------
		* | K | *
		---------
		3 | * | 2
	*/
	mapSpareBorder = Map::mapLeftBorder | Map::mapUpBorder | defendDiagonal; // angle
	while (!(kingBitdoard & mapSpareBorder)) // 0
	{
		kingBitdoard <<= Map::offsetMainDiag;
		if (kingBitdoard & attackedDiagonal)
			return true;
	}
	kingBitdoard = map[toUType(king)];
	mapSpareBorder = Map::mapUpBorder | Map::mapRightBorder | defendDiagonal;
	while (!(kingBitdoard & mapSpareBorder)) // 1
	{
		kingBitdoard <<= Map::offsetSideDiag;
		if (kingBitdoard & attackedDiagonal)
			return true;
	}
	kingBitdoard = map[toUType(king)];
	mapSpareBorder = Map::mapRightBorder | Map::mapDownBorder | defendDiagonal;
	while (!(kingBitdoard & mapSpareBorder)) // 2
	{
		kingBitdoard >>= Map::offsetMainDiag;
		if (kingBitdoard & attackedDiagonal)
			return true;
	}
	kingBitdoard = map[toUType(king)];
	mapSpareBorder = Map::mapDownBorder | Map::mapLeftBorder | defendDiagonal;
	while (!(kingBitdoard & mapSpareBorder)) // 3
	{
		kingBitdoard >>= Map::offsetSideDiag;
		if (kingBitdoard & attackedDiagonal)
			return true;
	}
	return false;
}

BoardPos Figure::CheckEmpty(const Color boardPosFrom, const Color boardPosTo) noexcept
{

	if (boardPosTo == Color::None)
		return BoardPos::Empty; // if boardPosTo is Empty
	if (boardPosFrom != boardPosTo)
		return BoardPos::Opposite; // if boardPosTo contains the figure with opposite color
	return BoardPos::Lock; // if boardPosTo contains the figure with same color
}

std::vector<std::vector<MoveInfo>> Figure::FindDiagonalMoves(const Pos& figurePosition, FigureType figureType, Map& map) noexcept
{
	std::vector<std::vector<MoveInfo>> possibleMoves;
	auto figureColor{ GetFigureTypeColor(figureType) };
	auto possibleCastling{ map.GetPossibleCastling() };
	auto movesCount{ static_cast<uint16_t>(map.GetMovesCount() + 1) };
	auto nextPosition{ figurePosition };
	auto nextPosFigureType{ FigureType::Empty };
	auto emptyStatus{ BoardPos::Empty };
	for (auto i = 0; i < DIAG_DIRECTIONS; ++i)
	{
		nextPosition = figurePosition;
		do
		{
			nextPosition = nextPosition.Add((i == 0 || i == 1 ? 1 : -1), (i == 0 || i == 2 ? 1 : -1)); // diagonal offset
			if (nextPosition.IsValid())
			{
				nextPosFigureType = map.GetFigureType(nextPosition);
				emptyStatus = CheckEmpty(figureColor, GetFigureTypeColor(nextPosFigureType));
				if (toUType(emptyStatus))
					possibleMoves.push_back(std::vector<MoveInfo>{ MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, possibleCastling, false, movesCount } });
			}
			else
				emptyStatus = BoardPos::Lock;
		} 
		while (emptyStatus == BoardPos::Empty);
	}
	return possibleMoves;
}

Color Figure::GetFigureTypeColor(FigureType figureType) noexcept
{
	if (figureType == FigureType::Empty)
		return Color::None;
	if (toUType(figureType) < 6) // used fixed enum order
		return Color::Black;
	return Color::White;
}

std::vector<std::vector<MoveInfo>> Figure::FindPossibleMoves(const Pos& figurePosition, FigureType figureType, Map& map) noexcept
{
	if (figureType == FigureType::King_black || figureType == FigureType::King_white)
	{
		return FindPossibleMovesKing(figurePosition, figureType, map);
	}
	else if (figureType == FigureType::Queen_black || figureType == FigureType::Queen_white)
	{
		return FindPossibleMovesQueen(figurePosition, figureType, map);
	}
	else if (figureType == FigureType::Bishop_black || figureType == FigureType::Bishop_white)
	{
		return FindPossibleMovesBishop(figurePosition, figureType, map);
	}
	else if (figureType == FigureType::Knight_black || figureType == FigureType::Knight_white)
	{
		return FindPossibleMovesKnight(figurePosition, figureType, map);
	}
	else if (figureType == FigureType::Rook_black || figureType == FigureType::Rook_white)
	{
		return FindPossibleMovesRook(figurePosition, figureType, map);
	}
	else 
	{
		return FindPossibleMovesPawn(figurePosition, figureType, map);
	}
}

std::vector<std::vector<MoveInfo>> Figure::FindPossibleMovesKing(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept
{
	std::vector<std::vector<MoveInfo>> possibleMoves;
	auto figureColor{ GetFigureTypeColor(figureType) };
	auto possibleCastling{ map.GetPossibleCastling() };
	std::array<bool, 4> nextPossibleCastling{ figureColor == Color::White ? false, false, possibleCastling[2], possibleCastling[3] : possibleCastling[0], possibleCastling[1], false, false };
	auto movesCount{ static_cast<uint16_t>(map.GetMovesCount() + 1) };
	auto nextPosition{ figurePosition };
	auto nextPosFigureType{ FigureType::Empty };
	// checking all nearby squares
	for (auto i = -1; i != 3; i += 2)
	{
		nextPosition = figurePosition.Add(0, i);
		if (nextPosition.IsValid())
		{
			nextPosFigureType = map.GetFigureType(nextPosition);
			if (toUType(CheckEmpty(figureColor, GetFigureTypeColor(nextPosFigureType))))
				possibleMoves.push_back(std::vector<MoveInfo>{ MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, nextPossibleCastling, false, movesCount } });
		}
		nextPosition = figurePosition.Add(i, i);
		if (nextPosition.IsValid())
		{
			nextPosFigureType = map.GetFigureType(nextPosition);
			if (toUType(CheckEmpty(figureColor, GetFigureTypeColor(nextPosFigureType))))
				possibleMoves.push_back(std::vector<MoveInfo>{ MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, nextPossibleCastling, false, movesCount } });
		}
		nextPosition = figurePosition.Add(i, -i);
		if (nextPosition.IsValid())
		{
			nextPosFigureType = map.GetFigureType(nextPosition);
			if (toUType(CheckEmpty(figureColor, GetFigureTypeColor(nextPosFigureType))))
				possibleMoves.push_back(std::vector<MoveInfo>{ MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, nextPossibleCastling, false, movesCount } });
		}
		nextPosition = figurePosition.Add(i, 0);
		if (nextPosition.IsValid())
		{
			nextPosFigureType = map.GetFigureType(nextPosition);
			if (toUType(CheckEmpty(figureColor, GetFigureTypeColor(nextPosFigureType))))
				possibleMoves.push_back(std::vector<MoveInfo>{ MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, nextPossibleCastling, false, movesCount } });
		}
	}
	
	// castling
	if (figureColor == Color::White ? possibleCastling[0] || possibleCastling[1] : possibleCastling[2] || possibleCastling[3])
	{
		auto y{ static_cast<uint8_t>(figureColor == Color::White ? 0 : 7) };

		// checking long castling
		if (possibleCastling[2 * toUType(figureColor)] &&
			map.GetColor(Pos{ 1, y }) == Color::None &&
			map.GetColor(Pos{ 2, y }) == Color::None &&
			map.GetColor(Pos{ 3, y }) == Color::None)
		{
			map.DoImitationMove(figurePosition, Pos{ 3, y });
			if (!IsShahFor(figureColor, map.GetMap()))
			{
				std::vector<MoveInfo> possibleMove;
				possibleMove.push_back(MoveInfo{ figurePosition, Pos{ 2, y }, figureType, FigureType::Empty, nextPossibleCastling, false, movesCount });
				possibleMove.push_back(MoveInfo{ Pos{ 0, y }, Pos{ 3, y }, figureColor == Color::White ? FigureType::Rook_white : FigureType::Rook_black, FigureType::Empty, nextPossibleCastling, false, movesCount });
				possibleMoves.push_back(std::move(possibleMove));
			}
			map.UndoImitationMove(figurePosition, Pos{ 3, y }, FigureType::Empty);
		}

		// checking short castling
		if (possibleCastling[2 * toUType(figureColor) + 1] &&
			map.GetColor(Pos{ 6, y }) == Color::None &&
			map.GetColor(Pos{ 5, y }) == Color::None)
		{
			map.DoImitationMove(figurePosition, Pos{ 5, y });
			if (!IsShahFor(figureColor, map.GetMap()))
			{
				std::vector<MoveInfo> possibleMove;
				possibleMove.push_back(MoveInfo{ figurePosition, Pos{ 6, y }, figureType, FigureType::Empty, nextPossibleCastling, false, movesCount });
				possibleMove.push_back(MoveInfo{ Pos{ 7, y }, Pos{ 5, y }, figureColor == Color::White ? FigureType::Rook_white : FigureType::Rook_black, FigureType::Empty, nextPossibleCastling, false, movesCount });
				possibleMoves.push_back(std::move(possibleMove));
			}
			map.UndoImitationMove(figurePosition, Pos{ 5, y }, FigureType::Empty);
		}
	}
	return possibleMoves;
}

std::vector<std::vector<MoveInfo>> Figure::FindPossibleMovesQueen(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept
{
	auto possibleMoves{ FindStraightMoves(figurePosition, figureType, map) }, moreMoves{ FindDiagonalMoves(figurePosition, figureType, map) };
	possibleMoves.insert(possibleMoves.end(), moreMoves.begin(), moreMoves.end());
	return possibleMoves;
}

std::vector<std::vector<MoveInfo>> Figure::FindPossibleMovesBishop(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept
{
	return FindDiagonalMoves(figurePosition, figureType, map);
}

std::vector<std::vector<MoveInfo>> Figure::FindPossibleMovesKnight(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept
{
	std::vector<std::vector<MoveInfo>> possibleMoves;
	auto figureColor{ GetFigureTypeColor(figureType) };
	auto possibleCastling{ map.GetPossibleCastling() };
	auto movesCount{ static_cast<uint16_t>(map.GetMovesCount() + 1) };
	auto nextPosition{ figurePosition };
	auto nextPosFigureType{ FigureType::Empty };
	for (auto i = -1, j = 2 * i; i != 3; i += 2, j = 2 * i)
	{
		nextPosition = figurePosition.Add(i, j);
		if (nextPosition.IsValid())
		{
			nextPosFigureType = map.GetFigureType(nextPosition);
			if (toUType(CheckEmpty(figureColor, GetFigureTypeColor(nextPosFigureType))))
				possibleMoves.push_back(std::vector<MoveInfo>{ MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, possibleCastling, false, movesCount } });
		}
		nextPosition = figurePosition.Add(i, -j);
		if (nextPosition.IsValid())
		{
			nextPosFigureType = map.GetFigureType(nextPosition);
			if (toUType(CheckEmpty(figureColor, GetFigureTypeColor(nextPosFigureType))))
				possibleMoves.push_back(std::vector<MoveInfo>{ MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, possibleCastling, false, movesCount } });
		}
		nextPosition = figurePosition.Add(j, i);
		if (nextPosition.IsValid())
		{
			nextPosFigureType = map.GetFigureType(nextPosition);
			if (toUType(CheckEmpty(figureColor, GetFigureTypeColor(nextPosFigureType))))
				possibleMoves.push_back(std::vector<MoveInfo>{ MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, possibleCastling, false, movesCount } });
		}
		nextPosition = figurePosition.Add(-j, i);
		if (nextPosition.IsValid())
		{
			nextPosFigureType = map.GetFigureType(nextPosition);
			if (toUType(CheckEmpty(figureColor, GetFigureTypeColor(nextPosFigureType))))
				possibleMoves.push_back(std::vector<MoveInfo>{ MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, possibleCastling, false, movesCount } });
		}
	}
	return possibleMoves;
}

std::vector<std::vector<MoveInfo>> Figure::FindPossibleMovesRook(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept
{
	return FindStraightMoves(figurePosition, figureType, map);
}

std::vector<std::vector<MoveInfo>> Figure::FindPossibleMovesPawn(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept
{
	std::vector<std::vector<MoveInfo>> possibleMoves;
	auto possibleCastling{ map.GetPossibleCastling() };
	auto movesCount{ static_cast<uint16_t>(map.GetMovesCount() + 1) };
	auto pawnColor{ GetFigureTypeColor(figureType) };
	auto offset{ static_cast<int8_t>(pawnColor == Color::Black ? -1 : 1) }; // black go down | white go up
	auto nextPosition{ figurePosition.Add(0, offset) };
	auto nextPosFigureType{ map.GetFigureType(nextPosition) };

	// checking straight moves
	if (nextPosFigureType == FigureType::Empty)
	{
		std::vector<MoveInfo> possibleMove;
		possibleMove.push_back(MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, possibleCastling, false, movesCount });
		if (figurePosition.GetY() == 6 && pawnColor == Color::White || figurePosition.GetY() == 1 && pawnColor == Color::Black)
		{
			possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Queen_white : FigureType::Queen_black, figureType, possibleCastling, false, movesCount });
			possibleMoves.push_back(possibleMove);
			possibleMove.pop_back();
			possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Bishop_white : FigureType::Bishop_black, figureType, possibleCastling, false, movesCount });
			possibleMoves.push_back(possibleMove);
			possibleMove.pop_back();
			possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Knight_white : FigureType::Knight_black, figureType, possibleCastling, false, movesCount });
			possibleMoves.push_back(possibleMove);
			possibleMove.pop_back();
			possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Rook_white : FigureType::Rook_black, figureType, possibleCastling, false, movesCount });
			possibleMoves.push_back(std::move(possibleMove));
		}
		else
		{
			possibleMoves.push_back(std::move(possibleMove));

			// checking the pawn for the possibility of a long move
			if (figurePosition.GetY() == 1 && pawnColor == Color::White || figurePosition.GetY() == 6 && pawnColor == Color::Black)
			{
				nextPosition = nextPosition.Add(0, offset);
				nextPosFigureType = map.GetFigureType(nextPosition);
				if (nextPosFigureType == FigureType::Empty)
					possibleMoves.push_back(std::vector<MoveInfo>{ MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, possibleCastling, false, movesCount } });
			}
		}
	}

	// checking diagonal moves (eating enemy figure)
	nextPosition = figurePosition.Add(1, offset);
	if (nextPosition.IsValid())
	{
		nextPosFigureType = map.GetFigureType(nextPosition);
		if (CheckEmpty(pawnColor, GetFigureTypeColor(nextPosFigureType)) == BoardPos::Opposite)
		{
			std::vector<MoveInfo> possibleMove;
			possibleMove.push_back(MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, possibleCastling, false, movesCount });
			if (figurePosition.GetY() == 6 && pawnColor == Color::White || figurePosition.GetY() == 1 && pawnColor == Color::Black)
			{
				possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Queen_white : FigureType::Queen_black, figureType, possibleCastling, false, movesCount });
				possibleMoves.push_back(possibleMove);
				possibleMove.pop_back();
				possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Bishop_white : FigureType::Bishop_black, figureType, possibleCastling, false, movesCount });
				possibleMoves.push_back(possibleMove);
				possibleMove.pop_back();
				possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Knight_white : FigureType::Knight_black, figureType, possibleCastling, false, movesCount });
				possibleMoves.push_back(possibleMove);
				possibleMove.pop_back();
				possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Rook_white : FigureType::Rook_black, figureType, possibleCastling, false, movesCount });
				possibleMoves.push_back(std::move(possibleMove));
			}
			else
				possibleMoves.push_back(std::move(possibleMove));
		}
	}
	nextPosition = figurePosition.Add(-1, offset);
	if (nextPosition.IsValid())
	{
		nextPosFigureType = map.GetFigureType(nextPosition);
		if (CheckEmpty(pawnColor, GetFigureTypeColor(nextPosFigureType)) == BoardPos::Opposite)
		{
			std::vector<MoveInfo> possibleMove;
			possibleMove.push_back(MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, possibleCastling, false, movesCount });
			if (figurePosition.GetY() == 6 && pawnColor == Color::White || figurePosition.GetY() == 1 && pawnColor == Color::Black)
			{
				possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Queen_white : FigureType::Queen_black, figureType, possibleCastling, false, movesCount });
				possibleMoves.push_back(possibleMove);
				possibleMove.pop_back();
				possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Bishop_white : FigureType::Bishop_black, figureType, possibleCastling, false, movesCount });
				possibleMoves.push_back(possibleMove);
				possibleMove.pop_back();
				possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Knight_white : FigureType::Knight_black, figureType, possibleCastling, false, movesCount });
				possibleMoves.push_back(possibleMove);
				possibleMove.pop_back();
				possibleMove.push_back(MoveInfo{ Pos::NULL_POS, nextPosition, pawnColor == Color::White ? FigureType::Rook_white : FigureType::Rook_black, figureType, possibleCastling, false, movesCount });
				possibleMoves.push_back(std::move(possibleMove));
			}
			else
				possibleMoves.push_back(std::move(possibleMove));
		}
	}

	// checking capture en passant
	const auto lastMove{ map.GetLastMoveInfo() };
	if (lastMove != MoveInfo::NULL_INFO)
	{
		auto activeType{ lastMove.GetTypeActiveFigure() };
		if (activeType == FigureType::Pawn_black || activeType == FigureType::Pawn_white)
		{
			auto posBefore{ lastMove.GetPosBeforeMove() };
			auto posAfter{ lastMove.GetPosAfterMove() };
			if (abs(posAfter.GetY() - posBefore.GetY()) == 2 && (posAfter.GetY() == figurePosition.GetY()) && abs(posAfter.GetX() - figurePosition.GetX()) == 1)
			{
				std::vector<MoveInfo> possibleMove;
				possibleMove.push_back(MoveInfo{ figurePosition, Pos{ posAfter.GetX(), static_cast<uint8_t>(figurePosition.GetY() + (posAfter.GetY() > posBefore.GetY() ? -1 : 1)) }, figureType, FigureType::Empty, possibleCastling, true, movesCount });
				possibleMove.push_back(MoveInfo{ posAfter, posAfter, figureType, activeType, possibleCastling, true, movesCount });
				possibleMoves.push_back(std::move(possibleMove));
			}
		}
	}
	return possibleMoves;
}

std::vector<std::vector<MoveInfo>> Figure::FindStraightMoves(const Pos& figurePosition, const FigureType figureType, Map& map) noexcept
{
	std::vector<std::vector<MoveInfo>> possibleMoves;
	auto figureColor{ GetFigureTypeColor(figureType) };
	auto nextPossibleCastling{ map.GetPossibleCastling() };
	if (figureType == FigureType::Rook_black || figureType == FigureType::Rook_white)
	{
		if (figurePosition == Pos{ 0, 0 } || figurePosition == Pos{ 0, 7 })
			nextPossibleCastling[2 * toUType(figureColor)] = false;
		if (figurePosition == Pos{ 7, 0 } || figurePosition == Pos{ 7, 7 })
			nextPossibleCastling[2 * toUType(figureColor) + 1] = false;
	}
	auto movesCount{ static_cast<uint16_t>(map.GetMovesCount() + 1) };
	auto nextPosition{ figurePosition };
	auto nextPosFigureType{ FigureType::Empty };
	auto emptyStatus{ BoardPos::Empty };

	for (auto i = 0; i != STRAIGHT_DIRECTIONS; ++i)
	{
		nextPosition = figurePosition;
		do
		{
			if (i < 2)
				nextPosition = nextPosition.Add((i == 0 ? 1 : -1), 0); // offset along Ox
			else
				nextPosition = nextPosition.Add(0, (i == 2 ? 1 : -1)); // offset along Oy

			if (nextPosition.IsValid())
			{
				nextPosFigureType = map.GetFigureType(nextPosition);
				emptyStatus = CheckEmpty(figureColor, GetFigureTypeColor(nextPosFigureType));
				if (toUType(emptyStatus))
				{
					possibleMoves.push_back(std::vector<MoveInfo>{ MoveInfo{ figurePosition, nextPosition, figureType, nextPosFigureType, nextPossibleCastling, false, movesCount } });
				}
			}
			else
				emptyStatus = BoardPos::Lock;
		}
		while (emptyStatus == BoardPos::Empty);
	}
	return possibleMoves;
}
