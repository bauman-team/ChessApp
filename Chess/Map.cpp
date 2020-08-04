#include "Map.h"
#include "Pos.h"
#include <cassert>

Map::Map()
{
	map[0] = new Rook(Pos(0, 0), Color::White);
	map[1] = new Knight(Pos(1, 0), Color::White);
	map[2] = new Bishop(Pos(2, 0), Color::White);
	map[3] = new Queen(Pos(3, 0), Color::White);
	map[4] = new King(Pos(4, 0), Color::White);
	map[5] = new Bishop(Pos(5, 0), Color::White);
	map[6] = new Knight(Pos(6, 0), Color::White);
	map[7] = new Rook(Pos(7, 0), Color::White);

	for (int i = 8; i != 16; ++i)
		map[i] = new Pawn(Pos(i % 8, 1), Color::White);

	for (int i = 16; i != 48; ++i)
		map[i] = new Empty(Pos(i % 8, i / 8));

	for (int i = 48; i != 56; ++i)
		map[i] = new Pawn(Pos(i % 8, 6), Color::Black);

	map[56] = new Rook(Pos(0, 7), Color::Black);
	map[57] = new Knight(Pos(1, 7), Color::Black);
	map[58] = new Bishop(Pos(2, 7), Color::Black);
	map[59] = new Queen(Pos(3, 7), Color::Black);
	map[60] = new King(Pos(4, 7), Color::Black);
	map[61] = new Bishop(Pos(5, 7), Color::Black);
	map[62] = new Knight(Pos(6, 7), Color::Black);
	map[63] = new Rook(Pos(7, 7), Color::Black);
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

void Map::Move(const Pos& from, const Pos& to)
{
	assert(from.IsValid() && to.IsValid());
	if (map[to.ToIndex()]->GetType() != FigureType::Empty)
	{
		// decrease numOfFigures
	}
	//delete map[to.ToIndex()];
	map[to.ToIndex()] = map[from.ToIndex()];
	map[from.ToIndex()] = new Empty(Pos(from));
}

void Map::SetToEmpty(const Pos& target)
{
	assert(target.IsValid());
	delete map[target.ToIndex()]; // TODO: don't delete figure, need to save it in log
	map[target.ToIndex()] = new Empty(Pos(target));
}

void Map::PawnToQueen(const Pos& target)
{
	assert(target.IsValid());
	Color pawnColor = map[target.ToIndex()]->GetColor();
	delete map[target.ToIndex()]; // TODO: don't delete figure, need to save it in log
	map[target.ToIndex()] = new Queen(Pos(target), pawnColor);
}

void Map::Castling(const Pos& from, const Pos& to)
{
	//assert(from.IsValid() && to.IsValid());
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

int8_t Map::CheckEmpty(const Pos& from, const Pos& to) const
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
