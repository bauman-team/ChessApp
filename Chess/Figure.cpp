#include "Figure.h"

Map* Figure::ptrMap = nullptr;


Empty::Empty(Pos _coords) : Figure(_coords, Color::None)
{
	type = FigureType::Empty;
}

King::King(Pos _coords, Color _color) : Figure(_coords, _color)
{
	type = (color == Color::Black) ? FigureType::King_black : FigureType::King_white;
	possibleCastling = true;
}

Queen::Queen(Pos _coords, Color _color) : Figure(_coords, _color)
{
	type = (color == Color::Black) ? FigureType::Queen_black : FigureType::Queen_white;
}

Bishop::Bishop(Pos _coords, Color _color) : Figure(_coords, _color)
{
	type = (color == Color::Black) ? FigureType::Bishop_black : FigureType::Bishop_white;
}

Knight::Knight(Pos _coords, Color _color) : Figure(_coords, _color)
{
	type = (color == Color::Black) ? FigureType::Knight_black : FigureType::Knight_white;
}

Rook::Rook(Pos _coords, Color _color) : Figure(_coords, _color)
{
	type = (color == Color::Black) ? FigureType::Rook_black : FigureType::Rook_white;
	possibleCastling = true;
}

Pawn::Pawn(Pos _coords, Color _color) : Figure(_coords, _color)
{
	type = (color == Color::Black) ? FigureType::Pawn_black : FigureType::Pawn_white;
}

std::vector<Pos>& King::FindPossibleMoves()
{
	Pos coords = ptrMap->GetFigurePosition(this), nextPosition;
	for (int i = -1; i != 3; i += 2)
	{
		nextPosition = coords + Pos(0, i);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords + Pos(i, i);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords + Pos(i, -i);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords + Pos(i, 0);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
	}
	if (possibleCastling)
	{
		int y = (color == Color::White) ? 0 : 7;
		std::vector<Pos> checkCastling;
		if ((ptrMap->GetFigureAt(Pos(0, y))->GetType() == FigureType::Rook_white
			|| ptrMap->GetFigureAt(Pos(0, y))->GetType() == FigureType::Rook_black)
			&& ptrMap->GetFigureAt(Pos(0, y))->GetColor() == color
			&& ptrMap->CheckEmpty(coords, Pos(1, y)) == 1
			&& ptrMap->CheckEmpty(coords, Pos(2, y)) == 1
			&& ptrMap->CheckEmpty(coords, Pos(3, y)) == 1)
			if (((Rook*)ptrMap->GetFigureAt(Pos(0, y)))->GetCastling())
			{
				checkCastling.push_back(Pos(3, y));
				checkCastling = ptrMap->CheckingPossibleMove(coords, checkCastling);
				if (!checkCastling.empty())
				{
					checkCastling.pop_back();
					possibleMoves.push_back(Pos(2, y));
				}
			}
		if ((ptrMap->GetFigureAt(Pos(7, y))->GetType() == FigureType::Rook_white
			|| ptrMap->GetFigureAt(Pos(7, y))->GetType() == FigureType::Rook_black)
			&& ptrMap->GetFigureAt(Pos(7, y))->GetColor() == color
			&& ptrMap->CheckEmpty(coords, Pos(6, y)) == 1
			&& ptrMap->CheckEmpty(coords, Pos(5, y)) == 1)
			if (((Rook*)ptrMap->GetFigureAt(Pos(7, y)))->GetCastling())
			{
				checkCastling.push_back(Pos(5, y));
				checkCastling = ptrMap->CheckingPossibleMove(coords, checkCastling);
				if (!checkCastling.empty())
				{
					checkCastling.pop_back();
					possibleMoves.push_back(Pos(6, y));
				}
			}
	}
	movesFound = true;
	return possibleMoves;
}

std::vector<Pos>& Queen::FindPossibleMoves() // Rook + Bishop
{
	Pos coords = ptrMap->GetFigurePosition(this);
	possibleMoves = Rook::FindStraightMoves(coords);
	std::vector<Pos> moreMoves;
	moreMoves = Bishop::FindDiagonalMoves(coords); // TODO: fix bug with empty vector exception
	possibleMoves.insert(possibleMoves.end(), moreMoves.begin(), moreMoves.end());
	movesFound = true;
	return possibleMoves;
}

std::vector<Pos> Bishop::FindDiagonalMoves(Pos coords)
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
			nextPosition = Pos(nextPosition.GetX() + (i == 0 || i == 1 ? 1 : -1), nextPosition.GetY() + (i == 0 || i == 2 ? 1 : -1)); // diagonal
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

std::vector<Pos>& Bishop::FindPossibleMoves()
{
	Pos coords = ptrMap->GetFigurePosition(this);
	possibleMoves = FindDiagonalMoves(coords);
	movesFound = true;
	return possibleMoves;
}

std::vector<Pos>& Knight::FindPossibleMoves()
{
	Pos coords = ptrMap->GetFigurePosition(this), nextPosition;
	for (int i = -1, j = 2 * i; i != 3; i += 2)
	{
		nextPosition = coords + Pos(i, j);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords + Pos(i, -j);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords + Pos(j, i);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);

		nextPosition = coords + Pos(j, -i);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
	}
	movesFound = true;
	return possibleMoves;
}

std::vector<Pos> Rook::FindStraightMoves(Pos coords)
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
				nextPosition = Pos(nextPosition.GetX() + (i == 0 ? 1 : -1), nextPosition.GetY());
			else // Oy
				nextPosition = Pos(nextPosition.GetX(), nextPosition.GetY() + (i == 2 ? 1 : -1));
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

std::vector<Pos>& Rook::FindPossibleMoves()
{
	Pos coords = ptrMap->GetFigurePosition(this);
	possibleMoves = FindStraightMoves(coords);
	movesFound = true;
	return possibleMoves;
}

std::vector<Pos>& Pawn::FindPossibleMoves()
{
	Pos coords = ptrMap->GetFigurePosition(this), nextPosition = coords;
	nextPosition = Pos(nextPosition.GetX(), nextPosition.GetY() + (color == Color::Black ? -1 : 1)); // black go down | white go up
	if (ptrMap->CheckEmpty(coords, nextPosition) == 1)
	{
		possibleMoves.push_back(nextPosition);
		if (coords.GetY() == 1 && color == Color::White || coords.GetY() == 6 && color == Color::Black) // check for double move check position
		{
			nextPosition = Pos(nextPosition.GetX(), nextPosition.GetY() + (color == Color::Black ? -1 : 1));
			if (ptrMap->CheckEmpty(coords, nextPosition) == 1)
				possibleMoves.push_back(nextPosition);
		}
	}

	nextPosition = Pos(coords.GetX() + 1, coords.GetY() + (color == Color::Black ? -1 : 1)); // if can eat
	if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
		possibleMoves.push_back(nextPosition);
	nextPosition = Pos(coords.GetX() - 1, coords.GetY() + (color == Color::Black ? -1 : 1));
	if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
		possibleMoves.push_back(nextPosition);

	MoveInfo* lastMove = ptrMap->GetLastMoveInfo();
	if (lastMove)
	{
		FigureType activeType = lastMove->GetActiveFigure()->GetType();
		if (activeType == FigureType::Pawn_black || activeType == FigureType::Pawn_white)
		{
			Pos posBefore = lastMove->GetPosBeforeMove();
			Pos posAfter = lastMove->GetPosAfterMove();
			if (abs(posAfter.GetY() - posBefore.GetY()) == 2 && (posAfter.GetY() == coords.GetY()) && abs(posAfter.GetX() - coords.GetX()) == 1)
				possibleMoves.push_back(Pos(posAfter.GetX(), coords.GetY() + (posAfter.GetY() > posBefore.GetY() ? -1 : 1)));
		}
	}
	movesFound = true;
	return possibleMoves;
}

bool Figure::MakeMoveTo(const Pos& nextPos)
{
	Pos coords = ptrMap->GetFigurePosition(this);
	for (std::vector<Pos>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it)
		if (*it == nextPos)
		{
			ptrMap->Move(coords, nextPos);
			coords = nextPos;
			possibleMoves.clear();
			return true;
		}
	return false; // missclick or error
}


void Figure::ClearPossibleMoves()
{
	possibleMoves.clear();
	movesFound = false;
}

bool King::MakeMoveTo(const Pos& nextPos)
{
	Pos coords = ptrMap->GetFigurePosition(this);
	if (abs(nextPos.GetX() - coords.GetX()) == 2)
	{
		if (possibleCastling)
			for (std::vector<Pos>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it)
				if (*it == nextPos)
				{
					possibleCastling = false;
					ptrMap->Castling(coords, nextPos);
					coords = nextPos;
					possibleMoves.clear();
					return true;
				}
		return false; // missclick or error
	}
	if (Figure::MakeMoveTo(nextPos)) // call base class method 
		possibleCastling = false;
	return nextPos == coords;
}

bool Rook::MakeMoveTo(const Pos& nextPos)
{
	if (Figure::MakeMoveTo(nextPos)) // call base class method 
	{
		possibleCastling = false;
		return true;
	}
	return false;
}

bool Pawn::MakeMoveTo(const Pos& nextPos)
{
	Pos coords = ptrMap->GetFigurePosition(this);
	bool isCaptureEnPassant = abs(coords.GetX() - nextPos.GetX()) == 1 && abs(coords.GetY() - nextPos.GetY()) == 1;
	int lastCoordY = coords.GetY();
	if (nextPos.IsValid())
		if (ptrMap->GetFigureAt(nextPos)->GetType() != FigureType::Empty)
			isCaptureEnPassant = false;
	if (Figure::MakeMoveTo(nextPos)) // call base class method 
	{
		if (isCaptureEnPassant)
			ptrMap->SetToEmpty(Pos(coords.GetX(), lastCoordY));
		if (color == Color::Black && coords.GetY() == 0 || color == Color::White && coords.GetY() == 7)
			ptrMap->PawnToQueen(coords);
		return true;
	}
	return false;
}