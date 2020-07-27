#include "Map.h"

Map::Map()
{
	map[0] = new Rook(Pos(0, 0), FigureType::Rook_white);
	map[1] = new Knight(Pos(1, 0), FigureType::Knight_white);
	map[2] = new Bishop(Pos(2, 0), FigureType::Bishop_white);
	map[3] = new Queen(Pos(3, 0), FigureType::Queen_white);
	map[4] = new King(Pos(4, 0), FigureType::King_white);
	map[5] = new Bishop(Pos(5, 0), FigureType::Bishop_white);
	map[6] = new Knight(Pos(6, 0), FigureType::Knight_white);
	map[7] = new Rook(Pos(7, 0), FigureType::Rook_white);
	for (int i = 8; i != 16; ++i)
		map[i] = new Pawn(Pos(i % 8, 1), FigureType::Pawn_white);
	for (int i = 16; i != 48; ++i)
		map[i] = new EmptY(Pos(i % 8, i / 8), FigureType::Empty);
	for (int i = 48; i != 56; ++i)
		map[i] = new Pawn(Pos(i % 8, 6), FigureType::Pawn_black);
	map[56] = new Rook(Pos(0, 7), FigureType::Rook_black);
	map[57] = new Knight(Pos(1, 7), FigureType::Knight_black);
	map[58] = new Bishop(Pos(2, 7), FigureType::Bishop_black);
	map[59] = new Queen(Pos(3, 7), FigureType::Queen_black);
	map[60] = new King(Pos(4, 7), FigureType::King_black);
	map[61] = new Bishop(Pos(5, 7), FigureType::Bishop_black);
	map[62] = new Knight(Pos(6, 7), FigureType::Knight_black);
	map[63] = new Rook(Pos(7, 7), FigureType::Rook_black);
	SetMap();
}

void Map::Move(const Pos& from, const Pos& to)
{
	if (map[to.x + to.y * 8]->GetId() == FigureType::Empty)
	{
		delete map[to.x + to.y * 8];
	}
	else
	{
		delete map[to.x + to.y * 8]; // decrease numOfFigures
	}
	map[to.x + to.y * 8] = map[from.x + from.y * 8];
	map[from.x + from.y * 8] = new EmptY(Pos(from), FigureType::Empty);
}

void Map::SetToEmpty(const Pos& target)
{
	delete map[target.x + target.y * 8];
	map[target.x + target.y * 8] = new EmptY(Pos(target), FigureType::Empty);
}

void Map::PawnToQueen(const Pos& target)
{
	if (map[target.x + target.y * 8]->GetId() == FigureType::Pawn_black)
	{
		delete map[target.x + target.y * 8];
		map[target.x + target.y * 8] = new Queen(Pos(target), FigureType::Queen_black);
	}
	else
	{
		delete map[target.x + target.y * 8];
		map[target.x + target.y * 8] = new Queen(Pos(target), FigureType::Queen_white);
	}
}

void Map::Castling(const Pos& from, const Pos& to)
{
	if (map[from.x + from.y * 8]->GetId() == FigureType::King_black)
	{
		if (from.x > to.x)
		{
			Move(from, to);
			Move(Pos(0, 7), Pos(3, 7));
		}
		else
		{
			Move(from, to);
			Move(Pos(7, 7), Pos(5, 7));
		}
	}
	else
	{
		if (from.x > to.x)
		{
			Move(from, to);
			Move(Pos(0, 0), Pos(3, 0));
		}
		else
		{
			Move(from, to);
			Move(Pos(7, 0), Pos(5, 0));
		}
	}
}

int8_t Map::CheckEmpty(const Pos& from, const Pos& to)
{
	if (to.x + to.y * 8 >= 0 && to.x + to.y * 8 < 64)
	{
		if (map[to.x + to.y * 8]->GetId() == FigureType::Empty)
			return 1; // if Pos to is Empty
		if (to_underlying(map[from.x + from.y * 8]->GetId()) < 6 && to_underlying(map[to.x + to.y * 8]->GetId()) > 5 || to_underlying(map[from.x + from.y * 8]->GetId()) > 5 && to_underlying(map[to.x + to.y * 8]->GetId()) < 6)
			return 2; // if Pos contains the figure with opposite color
	}
	return 0; // if Pos contains the figure with same color or output border
}
