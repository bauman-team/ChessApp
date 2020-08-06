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


std::vector<Pos> King::FindPossibleMoves()
{
	Pos nextPosition;
	for (int i = 0; i != 2; ++i)
	{
		nextPosition = Pos(coords.GetX(), coords.GetY() + 1 + i % 2 * -2);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.GetX() - 1 + i % 2 * 2, coords.GetY() + 1 + i % 2 * -2);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.GetX() + 1 + i % 2 * -2, coords.GetY() + 1 + i % 2 * -2);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.GetX() + 1 + i % 2 * -2, coords.GetY());
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
	}
	if (possibleCastling)
	{
		if (color == Color::White)
		{
			if (ptrMap->GetFigureAt(Pos(0, 0))->GetType() == FigureType::Rook_white 
				&& ptrMap->CheckEmpty(coords, Pos(1, 0)) == 1
				&& ptrMap->CheckEmpty(coords, Pos(2, 0)) == 1
				&& ptrMap->CheckEmpty(coords, Pos(3, 0)) == 1)
				if (((Rook*)ptrMap->GetFigureAt(Pos(0, 0)))->GetCastling())
					possibleMoves.push_back(Pos(2, 0));
			if (ptrMap->GetFigureAt(Pos(7, 0))->GetType() == FigureType::Rook_white
				&& ptrMap->CheckEmpty(coords, Pos(6, 0)) == 1
				&& ptrMap->CheckEmpty(coords, Pos(5, 0)) == 1)
				if (((Rook*)ptrMap->GetFigureAt(Pos(7, 0)))->GetCastling())
					possibleMoves.push_back(Pos(6, 0));
		}
		else
		{
			if (ptrMap->GetFigureAt(Pos(0, 7))->GetType() == FigureType::Rook_black
				&& ptrMap->CheckEmpty(coords, Pos(1, 7)) == 1
				&& ptrMap->CheckEmpty(coords, Pos(2, 7)) == 1
				&& ptrMap->CheckEmpty(coords, Pos(3, 7)) == 1)
				if (((Rook*)ptrMap->GetFigureAt(Pos(0, 7)))->GetCastling())
					possibleMoves.push_back(Pos(2, 7));
			if (ptrMap->GetFigureAt(Pos(7, 7))->GetType() == FigureType::Rook_black
				&& ptrMap->CheckEmpty(coords, Pos(6, 7)) == 1
				&& ptrMap->CheckEmpty(coords, Pos(5, 7)) == 1)
				if (((Rook*)ptrMap->GetFigureAt(Pos(7, 7)))->GetCastling())
					possibleMoves.push_back(Pos(6, 7));
		}
	}
	possibleMoves = ptrMap->CheckingPossibleMove(coords, possibleMoves);
	movesFound = true;
	return possibleMoves;
}

std::vector<Pos> Queen::FindPossibleMoves() // Rook + Bishop
{
	Pos nextPosition;
	bool isChecking;
	for (int i = 0; i != 4; ++i)
	{
		isChecking = true;
		nextPosition = coords;
		while (isChecking)
		{
			if (i < 2)
				nextPosition = Pos(nextPosition.GetX() + (i == 0 ? 1 : -1), nextPosition.GetY());
			else
				nextPosition = Pos(nextPosition.GetX(), nextPosition.GetY() + (i == 2 ? 1 : -1));
			if (ptrMap->CheckEmpty(coords, nextPosition))
			{
				possibleMoves.push_back(nextPosition);
				if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
					isChecking = false;
			}
			else
			{
				isChecking = false;
			}
		}
	}
	for (int i = 0; i != 4; ++i)
	{
		isChecking = true;
		nextPosition = coords;
		while (isChecking)
		{
			nextPosition = Pos(nextPosition.GetX() + (i == 0 || i == 1 ? 1 : -1), nextPosition.GetY() + (i == 0 || i == 2 ? 1 : -1));
			if (ptrMap->CheckEmpty(coords, nextPosition))
			{
				possibleMoves.push_back(nextPosition);
				if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
					isChecking = false;
			}
			else
			{
				isChecking = false;
			}
		}
	}
	possibleMoves = ptrMap->CheckingPossibleMove(coords, possibleMoves);
	movesFound = true;
	return possibleMoves;
}

std::vector<Pos> Bishop::FindPossibleMoves()
{
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
				possibleMoves.push_back(nextPosition);
				if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
					isChecking = false;
			}
			else
			{
				isChecking = false;
			}
		}
	}
	possibleMoves = ptrMap->CheckingPossibleMove(coords, possibleMoves);
	movesFound = true;
	return possibleMoves;
}

std::vector<Pos> Knight::FindPossibleMoves()
{
	Pos nextPosition;
	for (int i = 0; i != 2; ++i)
	{
		nextPosition = Pos(coords.GetX() + i % 2 + 1, coords.GetY() + (i + 1) % 2 + 1);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.GetX() - (i % 2 + 1), coords.GetY() + (i + 1) % 2 + 1);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.GetX() + i % 2 + 1, coords.GetY() - ((i + 1) % 2 + 1));
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.GetX() - (i % 2 + 1), coords.GetY() - ((i + 1) % 2 + 1));
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
	}
	possibleMoves = ptrMap->CheckingPossibleMove(coords, possibleMoves);
	movesFound = true;
	return possibleMoves;
}

std::vector<Pos> Rook::FindPossibleMoves()
{
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
				possibleMoves.push_back(nextPosition);
				if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
					isChecking = false;
			}
			else
			{
				isChecking = false;
			}
		}
	}
	possibleMoves = ptrMap->CheckingPossibleMove(coords, possibleMoves);
	movesFound = true;
	return possibleMoves;
}

std::vector<Pos> Pawn::FindPossibleMoves()
{
	Pos nextPosition = coords;
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
		if (lastMove->GetActiveFigure()->GetType() == FigureType::Pawn_black || lastMove->GetActiveFigure()->GetType() == FigureType::Pawn_white)
			if (abs(lastMove->GetPosAfterMove().GetY() - lastMove->GetPosBeforeMove().GetY()) == 2)
				if (lastMove->GetPosAfterMove().GetY() == coords.GetY() && abs(lastMove->GetPosAfterMove().GetX() - coords.GetX()) == 1)
					possibleMoves.push_back(Pos(lastMove->GetPosAfterMove().GetX(), coords.GetY() + (lastMove->GetPosAfterMove().GetY() > lastMove->GetPosBeforeMove().GetY() ? -1 : 1)));
	possibleMoves = ptrMap->CheckingPossibleMove(coords, possibleMoves);
	movesFound = true;
	return possibleMoves;
}


bool Figure::MakeMoveTo(const Pos& nextPos)
{
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