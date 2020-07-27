#include "Map.h"
#include <cassert>

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
	//SetMap();
}

void Map::Move(const Pos& from, const Pos& to)
{
	assert(from.IsValid() && to.IsValid());
	if (map[to.ToIndex()]->GetType() == FigureType::Empty)
	{
		delete map[to.ToIndex()];
	}
	else
	{
		delete map[to.ToIndex()]; // decrease numOfFigures
	}
	map[to.ToIndex()] = map[from.ToIndex()];
	map[from.ToIndex()] = new EmptY(Pos(from), FigureType::Empty);
}

void Map::SetToEmpty(const Pos& target)
{
	assert(target.IsValid());
	delete map[target.ToIndex()];
	map[target.ToIndex()] = new EmptY(Pos(target), FigureType::Empty);
}

void Map::PawnToQueen(const Pos& target)
{
	assert(target.IsValid());
	if (map[target.ToIndex()]->GetType() == FigureType::Pawn_black)
	{
		delete map[target.ToIndex()];
		map[target.ToIndex()] = new Queen(Pos(target), FigureType::Queen_black);
	}
	else
	{
		delete map[target.ToIndex()];
		map[target.ToIndex()] = new Queen(Pos(target), FigureType::Queen_white);
	}
}

void Map::Castling(const Pos& from, const Pos& to)
{
	assert(from.IsValid() && to.IsValid());
	Move(from, to);
	if (map[from.ToIndex()]->GetType() == FigureType::King_black)
	{
		(from.GetX() > to.GetX()) ? Move(Pos(0, 7), Pos(3, 7)) : Move(Pos(7, 7), Pos(5, 7));
	}
	else
	{
		(from.GetX() > to.GetX()) ? Move(Pos(0, 0), Pos(3, 0)) : Move(Pos(7, 0), Pos(5, 0));
	}
}

int8_t Map::CheckEmpty(const Pos& from, const Pos& to)
{
	if (to.IsValid())
	{
		if (map[to.ToIndex()]->GetType() == FigureType::Empty)
			return 1; // if Pos to is Empty
		if (to_underlying(map[from.ToIndex()]->GetType()) < 6 && to_underlying(map[to.ToIndex()]->GetType()) > 5 ||
			to_underlying(map[from.ToIndex()]->GetType()) > 5 && to_underlying(map[to.ToIndex()]->GetType()) < 6)
			return 2; // if Pos contains the figure with opposite color
	}
	return 0; // if Pos contains the figure with same color or output border
}

Figure* Map::GetFigureAt(const Pos& pos) const
{
	assert(pos.IsValid());
	return map[pos.ToIndex()];
}

Figure* Map::GetFigureAt(int index) const
{
	assert(index >= 0 && index <= 63);
	return map[index];
}
