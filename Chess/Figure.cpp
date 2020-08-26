#include "Figure.h"

Map* Figure::ptrMap = nullptr;

std::vector<Pos> Figure::FindDiagonalMoves(const Pos& coords)
{
	std::vector<Pos> moves;
	Pos nextPosition;
	bool isChecking;
	for (int i = 0; i != 4; ++i)
	{
		isChecking = true;
		nextPosition = coords;
		while (isChecking)
		{
			nextPosition = nextPosition.Add((i == 0 || i == 1 ? 1 : -1), (i == 0 || i == 2 ? 1 : -1)); // diagonal 
			if (ptrMap->CheckEmpty(coords, nextPosition))
			{
				moves.push_back(nextPosition);
				if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
					isChecking = false;
			}
			else
			{
				isChecking = false;
			}
		}
	}
	return moves;
}

Color Figure::GetFigureTypeColor(const FigureType& selectedType)
{
	if (selectedType == FigureType::Empty)
		return Color::None;
	if (selectedType == FigureType::King_black ||
		selectedType == FigureType::Queen_black ||
		selectedType == FigureType::Bishop_black ||
		selectedType == FigureType::Knight_black ||
		selectedType == FigureType::Rook_black ||
		selectedType == FigureType::Pawn_black)
		return Color::Black;
	return Color::White;
}

std::vector<Pos>& Figure::FindPossibleMoves(const FigureType& type, const Pos& figurePosition)
{
	if (type == FigureType::King_black || type == FigureType::King_white)
	{
		return FindPossibleMovesKing(figurePosition);
	}
	else if (type == FigureType::Queen_black || type == FigureType::Queen_white)
	{
		return FindPossibleMovesQueen(figurePosition);
	}
	else if (type == FigureType::Bishop_black || type == FigureType::Bishop_white)
	{
		return FindPossibleMovesBishop(figurePosition);
	}
	else if (type == FigureType::Knight_black || type == FigureType::Knight_white)
	{
		return FindPossibleMovesKnight(figurePosition);
	}
	else if (type == FigureType::Rook_black || type == FigureType::Rook_white)
	{
		return FindPossibleMovesRook(figurePosition);
	}
	else 
	{
		return FindPossibleMovesPawn(figurePosition);
	}
}

std::vector<Pos>& Figure::FindPossibleMovesKing(const Pos& coords)
{
	Pos nextPosition;
	std::vector<Pos>& possibleMoves = *(new std::vector<Pos>);
	for (int i = -1; i != 3; i += 2)
	{
		nextPosition = coords.Add(0, i);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords.Add(i, i);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords.Add(i, -i);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords.Add(i, 0);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
	}
	if (ptrMap->GetCastling(ptrMap->GetColor(coords)))
	{
		int y = (ptrMap->GetColor(coords) == Color::White) ? 0 : 7;
		std::vector<Pos> checkCastling;
		if (ptrMap->GetCastling(ptrMap->GetColor(coords), Pos(0, y)))
			if (ptrMap->CheckEmpty(coords, Pos(1, y)) == 1
				&& ptrMap->CheckEmpty(coords, Pos(2, y)) == 1
				&& ptrMap->CheckEmpty(coords, Pos(3, y)) == 1)
			{
				checkCastling.push_back(Pos(3, y));
				Map::PossibleMoves checkMoves;
				checkMoves.figurePosition = const_cast<Pos*>(&coords);
				checkMoves.possibleMoves = &checkCastling;
				ptrMap->CheckingPossibleMove(checkMoves);
				if (!checkMoves.possibleMoves->empty())
					possibleMoves.push_back(Pos(2, y));
				checkCastling.pop_back();
			}
		if (ptrMap->GetCastling(ptrMap->GetColor(coords), Pos(7, y)))
			if (ptrMap->CheckEmpty(coords, Pos(6, y)) == 1
				&& ptrMap->CheckEmpty(coords, Pos(5, y)) == 1)
			{
				checkCastling.push_back(Pos(5, y));
				Map::PossibleMoves checkMoves;
				checkMoves.figurePosition = const_cast<Pos*>(&coords);
				checkMoves.possibleMoves = &checkCastling;
				ptrMap->CheckingPossibleMove(checkMoves);
				if (!checkMoves.possibleMoves->empty())
					possibleMoves.push_back(Pos(6, y));
				checkCastling.pop_back();
			}
	}
	return possibleMoves;
}

std::vector<Pos>& Figure::FindPossibleMovesQueen(const Pos& coords)
{
	std::vector<Pos>& possibleMoves = *(new std::vector<Pos>);
	possibleMoves = FindStraightMoves(coords);
	std::vector<Pos> moreMoves = FindDiagonalMoves(coords); // TODO: fix bug with empty vector exception
	possibleMoves.insert(possibleMoves.end(), moreMoves.begin(), moreMoves.end());
	return possibleMoves;
}

std::vector<Pos>& Figure::FindPossibleMovesBishop(const Pos& coords)
{
	std::vector<Pos>& possibleMoves = *(new std::vector<Pos>);
	possibleMoves = FindDiagonalMoves(coords);
	return possibleMoves;
}

std::vector<Pos>& Figure::FindPossibleMovesKnight(const Pos& coords)
{
	Pos nextPosition;
	std::vector<Pos>& possibleMoves = *(new std::vector<Pos>);
	for (int i = -1, j = 2 * i; i != 3; i += 2, j = 2 * i)
	{
		nextPosition = coords.Add(i, j);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords.Add(i, -j);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords.Add(j, i);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords.Add(-j, i);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
	}
	return possibleMoves;
}

std::vector<Pos>& Figure::FindPossibleMovesRook(const Pos& coords)
{
	std::vector<Pos>& possibleMoves = *(new std::vector<Pos>);
	possibleMoves = FindStraightMoves(coords);
	return possibleMoves;
}

std::vector<Pos>& Figure::FindPossibleMovesPawn(const Pos& coords)
{
	Pos nextPosition = coords;
	std::vector<Pos>& possibleMoves = *(new std::vector<Pos>);
	Color color = ptrMap->GetColor(coords);
	nextPosition = nextPosition.Add(0, color == Color::Black ? -1 : 1); // black go down | white go up
	if (ptrMap->CheckEmpty(coords, nextPosition) == 1)
	{
		possibleMoves.push_back(nextPosition);
		if (coords.GetY() == 1 && color == Color::White || coords.GetY() == 6 && color == Color::Black) // check for double move check position
		{
			nextPosition = nextPosition.Add(0, color == Color::Black ? -1 : 1);
			if (ptrMap->CheckEmpty(coords, nextPosition) == 1)
				possibleMoves.push_back(nextPosition);
		}
	}

	nextPosition = coords.Add(1, color == Color::Black ? -1 : 1); // if can eat
	if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
		possibleMoves.push_back(nextPosition);
	nextPosition = coords.Add(-1, color == Color::Black ? -1 : 1);
	if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
		possibleMoves.push_back(nextPosition);

	MoveInfo* lastMove = ptrMap->GetLastMoveInfo();
	if (lastMove)
	{
		FigureType activeType = lastMove->GetTypeActiveFigure();
		if (activeType == FigureType::Pawn_black || activeType == FigureType::Pawn_white)
		{
			Pos posBefore = lastMove->GetPosBeforeMove();
			Pos posAfter = lastMove->GetPosAfterMove();
			if (abs(posAfter.GetY() - posBefore.GetY()) == 2 && (posAfter.GetY() == coords.GetY()) && abs(posAfter.GetX() - coords.GetX()) == 1)
				possibleMoves.push_back(Pos(posAfter.GetX(), coords.GetY() + (posAfter.GetY() > posBefore.GetY() ? -1 : 1)));
		}
	}
	return possibleMoves;
}

std::vector<Pos> Figure::FindStraightMoves(const Pos& coords)
{
	std::vector<Pos> moves;
	Pos nextPosition;
	bool isChecking;
	for (int i = 0; i != 4; ++i)
	{
		isChecking = true;
		nextPosition = coords;
		while (isChecking)
		{
			if (i < 2) // at first along Ox
				nextPosition = nextPosition.Add((i == 0 ? 1 : -1), 0);
			else // Oy
				nextPosition = nextPosition.Add(0, (i == 2 ? 1 : -1));
			if (ptrMap->CheckEmpty(coords, nextPosition))
			{
				moves.push_back(nextPosition);
				if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
					isChecking = false;
			}
			else
			{
				isChecking = false;
			}
		}
	}
	return moves;
}
