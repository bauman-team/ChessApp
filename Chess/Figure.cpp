#include "Figure.h"

constexpr auto DIAG_DIRECTIONS = 4;
constexpr auto STRAIGHT_DIRECTIONS = 4;

std::vector<Pos> Figure::FindDiagonalMoves(const Pos& coord, const Map& map)
{
	std::vector<Pos> possibleMoves;
	Pos nextPosition;
	int8_t emptyStatus;

	for (int i = 0; i < DIAG_DIRECTIONS; ++i)
	{
		nextPosition = coord;
		do
		{
			nextPosition = nextPosition.Add((i == 0 || i == 1 ? 1 : -1), (i == 0 || i == 2 ? 1 : -1)); // diagonal offset
			emptyStatus = map.CheckEmpty(coord, nextPosition); // TODO: create enum class for empty status
			if (emptyStatus)
				possibleMoves.push_back(nextPosition);
		} 
		while (emptyStatus == 1);
	}
	return possibleMoves;
}

Color Figure::GetFigureTypeColor(FigureType figureType)
{
	if (figureType == FigureType::Empty)
		return Color::None;
	if (static_cast<int>(figureType) < 6) // used fixed enum order
		return Color::Black;
	return Color::White;
}

std::vector<Pos> Figure::FindPossibleMoves(const Pos& figurePosition, FigureType figureType, Map& map)
{
	if (figureType == FigureType::King_black || figureType == FigureType::King_white)
	{
		return FindPossibleMovesKing(figurePosition, map);
	}
	else if (figureType == FigureType::Queen_black || figureType == FigureType::Queen_white)
	{
		return FindPossibleMovesQueen(figurePosition, map);
	}
	else if (figureType == FigureType::Bishop_black || figureType == FigureType::Bishop_white)
	{
		return FindPossibleMovesBishop(figurePosition, map);
	}
	else if (figureType == FigureType::Knight_black || figureType == FigureType::Knight_white)
	{
		return FindPossibleMovesKnight(figurePosition, map);
	}
	else if (figureType == FigureType::Rook_black || figureType == FigureType::Rook_white)
	{
		return FindPossibleMovesRook(figurePosition, map);
	}
	else 
	{
		return FindPossibleMovesPawn(figurePosition, map);
	}
}

std::vector<Pos> Figure::FindPossibleMovesKing(const Pos& coord, Map& map)
{
	Pos nextPosition;
	std::vector<Pos> possibleMoves;

	// checking all nearby squares
	for (int i = -1; i != 3; i += 2)
	{
		nextPosition = coord.Add(0, i);
		if (map.CheckEmpty(coord, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coord.Add(i, i);
		if (map.CheckEmpty(coord, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coord.Add(i, -i);
		if (map.CheckEmpty(coord, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coord.Add(i, 0);
		if (map.CheckEmpty(coord, nextPosition))
			possibleMoves.push_back(nextPosition);
	}
	
	// castling
	if (map.IsCastlingAllowedForKing(coord))
	{
		Color activeColor = map.GetColor(coord);
		int y = (activeColor == Color::White) ? 0 : 7;

		// checking long castling
		if (map.IsCastlingAllowedWithRook(Pos(0, y), activeColor) &&
			map.CheckEmpty(coord, Pos(1, y)) == 1 &&
			map.CheckEmpty(coord, Pos(2, y)) == 1 &&
			map.CheckEmpty(coord, Pos(3, y)) == 1)
		{
			OneFigureMoves checkMoves;
			checkMoves.from = coord;
			checkMoves.to.push_back(Pos(3, y));

			map.EraseForbiddenMoves(checkMoves);
			if (!checkMoves.to.empty())
				possibleMoves.push_back(Pos(2, y));
		}

		// checking short castling
		if (map.IsCastlingAllowedWithRook(Pos(7, y), activeColor) &&
			map.CheckEmpty(coord, Pos(6, y)) == 1 &&
			map.CheckEmpty(coord, Pos(5, y)) == 1)
		{
			OneFigureMoves checkMoves;
			checkMoves.from = coord;
			checkMoves.to.push_back(Pos(5, y));

			map.EraseForbiddenMoves(checkMoves);
			if (!checkMoves.to.empty())
				possibleMoves.push_back(Pos(6, y));
		}
	}
	return possibleMoves;
}

std::vector<Pos> Figure::FindPossibleMovesQueen(const Pos& coord, const Map& map)
{
	std::vector<Pos> possibleMoves = FindStraightMoves(coord, map);
	std::vector<Pos> moreMoves = FindDiagonalMoves(coord, map);
	possibleMoves.insert(possibleMoves.end(), moreMoves.begin(), moreMoves.end());
	return possibleMoves;
}

std::vector<Pos> Figure::FindPossibleMovesBishop(const Pos& coord, const Map& map)
{
	return FindDiagonalMoves(coord, map);
}

std::vector<Pos> Figure::FindPossibleMovesKnight(const Pos& coord, const Map& map)
{
	Pos nextPosition;
	std::vector<Pos> possibleMoves;
	for (int i = -1, j = 2 * i; i != 3; i += 2, j = 2 * i)
	{
		nextPosition = coord.Add(i, j);
		if (map.CheckEmpty(coord, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coord.Add(i, -j);
		if (map.CheckEmpty(coord, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coord.Add(j, i);
		if (map.CheckEmpty(coord, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coord.Add(-j, i);
		if (map.CheckEmpty(coord, nextPosition))
			possibleMoves.push_back(nextPosition);
	}
	return possibleMoves;
}

std::vector<Pos> Figure::FindPossibleMovesRook(const Pos& coord, const Map& map)
{
	return FindStraightMoves(coord, map);
}

std::vector<Pos> Figure::FindPossibleMovesPawn(const Pos& coord, const Map& map)
{
	Pos nextPosition = coord;
	std::vector<Pos> possibleMoves;
	Color pawnColor = map.GetColor(coord);
	int8_t offset = pawnColor == Color::Black ? -1 : 1; // black go down | white go up
	nextPosition = nextPosition.Add(0, offset); 

	// checking straight moves
	if (map.CheckEmpty(coord, nextPosition) == 1)
	{
		possibleMoves.push_back(nextPosition);

		// checking the pawn for the possibility of a long move
		if (coord.GetY() == 1 && pawnColor == Color::White || coord.GetY() == 6 && pawnColor == Color::Black)
		{
			nextPosition = nextPosition.Add(0, offset);
			if (map.CheckEmpty(coord, nextPosition) == 1)
				possibleMoves.push_back(nextPosition);
		}
	}

	// checking diagonal moves (eating enemy figure)
	nextPosition = coord.Add(1, offset);
	if (map.CheckEmpty(coord, nextPosition) == 2)
		possibleMoves.push_back(nextPosition);
	nextPosition = coord.Add(-1, offset);
	if (map.CheckEmpty(coord, nextPosition) == 2)
		possibleMoves.push_back(nextPosition);

	// checking capture en passant
	const MoveInfo lastMove = map.GetLastMoveInfo();
	if (lastMove != MoveInfo::NULL_INFO)
	{
		FigureType activeType = lastMove.GetTypeActiveFigure();
		if (activeType == FigureType::Pawn_black || activeType == FigureType::Pawn_white)
		{
			Pos posBefore = lastMove.GetPosBeforeMove();
			Pos posAfter = lastMove.GetPosAfterMove();
			if (abs(posAfter.GetY() - posBefore.GetY()) == 2 && (posAfter.GetY() == coord.GetY()) && abs(posAfter.GetX() - coord.GetX()) == 1)
				possibleMoves.push_back(Pos(posAfter.GetX(), coord.GetY() + (posAfter.GetY() > posBefore.GetY() ? -1 : 1)));
		}
	}
	return possibleMoves;
}

std::vector<Pos> Figure::FindStraightMoves(const Pos& coord, const Map& map)
{
	std::vector<Pos> possibleMoves;
	Pos nextPosition;
	int8_t emptyStatus;

	for (int i = 0; i != STRAIGHT_DIRECTIONS; ++i)
	{
		nextPosition = coord;
		do
		{
			if (i < 2)
				nextPosition = nextPosition.Add((i == 0 ? 1 : -1), 0); // offset along Ox
			else
				nextPosition = nextPosition.Add(0, (i == 2 ? 1 : -1)); // offset along Oy

			emptyStatus = map.CheckEmpty(coord, nextPosition);
			if (emptyStatus)
				possibleMoves.push_back(nextPosition);
		}
		while (emptyStatus == 1);
	}
	return possibleMoves;
}
