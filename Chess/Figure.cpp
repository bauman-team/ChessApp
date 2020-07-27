#include "Figure.h"

Map* Figure::ptrMap = nullptr;

void SetMapPtr(Map* p)
{ 
	Figure::ptrMap = p;
}

std::vector<Pos> Knight::FindPossibleMoves()
{
	Pos nextPosition;
	for (int i = 0; i != 2; ++i)
	{
		nextPosition = Pos(coords.x + i % 2 + 1, coords.y + (i + 1) % 2 + 1);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.x - i % 2 + 1, coords.y + (i + 1) % 2 + 1);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.x + i % 2 + 1, coords.y - (i + 1) % 2 + 1);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.x - i % 2 + 1, coords.y - (i + 1) % 2 + 1);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
	}
	return possibleMoves;
}

bool Figure::MakeMoveTo(const Pos& nextPos)
{
	for (std::vector<Pos>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it)
		if(*it == nextPos)
		{
			ptrMap->Move(coords, nextPos);
			coords = nextPos;
			possibleMoves.clear();
			return true;
		}
	return false; // missclick or error
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
			nextPosition = Pos(nextPosition.x + (i == 0 || i == 1 ? 1 : -1), nextPosition.y + (i == 0 || i == 2 ? 1 : -1)); // diagonal
			if (ptrMap->CheckEmpty(coords, nextPosition))
				possibleMoves.push_back(nextPosition);
			else
				isChecking = false;
		}
	}
	return possibleMoves;
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
				nextPosition = Pos(nextPosition.x + (i == 0 ? 1 : -1), nextPosition.y);
			else // Oy
				nextPosition = Pos(nextPosition.x, nextPosition.y + (i == 2 ? 1 : -1));
			if (ptrMap->CheckEmpty(coords, nextPosition))
				possibleMoves.push_back(nextPosition);
			else
				isChecking = false;
		}
	}
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
				nextPosition = Pos(nextPosition.x + (i == 0 ? 1 : -1), nextPosition.y);
			else
				nextPosition = Pos(nextPosition.x, nextPosition.y + (i == 2 ? 1 : -1));
			if (ptrMap->CheckEmpty(coords, nextPosition))
				possibleMoves.push_back(nextPosition);
			else
				isChecking = false;
		}
	}
	for (int i = 0; i != 4; ++i)
	{
		isChecking = true;
		nextPosition = coords;
		while (isChecking)
		{
			nextPosition = Pos(nextPosition.x + (i == 0 || i == 1 ? 1 : -1), nextPosition.y + (i == 0 || i == 2 ? 1 : -1));
			if (ptrMap->CheckEmpty(coords, nextPosition))
				possibleMoves.push_back(nextPosition);
			else
				isChecking = false;
		}
	}
	return possibleMoves;
}

bool Pawn::MakeMoveTo(const Pos& nextPos)
{
	if (Figure::MakeMoveTo(nextPos)) // call base class method 
	{
		if (id == FigureType::Pawn_black && coords.y == 0 || id == FigureType::Pawn_white && coords.y == 7)
			ptrMap->PawnToQueen(coords);
		return true;
	}
	return false;
}

std::vector<Pos> Pawn::FindPossibleMoves()
{
	Pos nextPosition = coords;
	nextPosition = Pos(nextPosition.x, nextPosition.y + (id == FigureType::Pawn_black ? -1 : 1)); // black go down | white go up
	if (ptrMap->CheckEmpty(coords, nextPosition) == 1)
	{
		possibleMoves.push_back(nextPosition);
		if (coords.y == 1 && id == FigureType::Pawn_white || coords.y == 6 && id == FigureType::Pawn_black) // check for double move check position
		{
			nextPosition = Pos(nextPosition.x, nextPosition.y + (id == FigureType::Pawn_black ? -1 : 1));
			if (ptrMap->CheckEmpty(coords, nextPosition) == 1)
				possibleMoves.push_back(nextPosition);
		}
	}
	nextPosition = Pos(coords.x + 1, coords.y + (id == FigureType::Pawn_black ? -1 : 1)); // if can eat
	if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
		possibleMoves.push_back(nextPosition);
	nextPosition = Pos(coords.x - 1, coords.y + (id == FigureType::Pawn_black ? -1 : 1));
	if (ptrMap->CheckEmpty(coords, nextPosition) == 2)
		possibleMoves.push_back(nextPosition);
	return possibleMoves;
}

bool King::MakeMoveTo(const Pos& nextPos)
{
	if (abs(nextPos.x - coords.x) == 2)
	{
		for (std::vector<Pos>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it)
			if (*it == nextPos)
			{
				ptrMap->Castling(coords, nextPos);
				coords = nextPos;
				possibleMoves.clear();
				return true;
			}
		return false; // missclick or error
	}
	return Figure::MakeMoveTo(nextPos); // call base class method 
}

std::vector<Pos> King::FindPossibleMoves()
{
	Pos nextPosition;
	for (int i = 0; i != 2; ++i)
	{
		nextPosition = Pos(coords.x, coords.y + 1 + i % 2 * -2);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.x - 1 + i % 2 * 2, coords.y + 1 + i % 2 * -2);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.x + 1 + i % 2 * -2, coords.y + 1 + i % 2 * -2);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
		nextPosition = Pos(coords.x + 1 + i % 2 * -2, coords.y);
		if (ptrMap->CheckEmpty(coords, nextPosition))
			possibleMoves.push_back(nextPosition);
	}
	return possibleMoves;
}
