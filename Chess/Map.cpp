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
	MoveInfo info(from, to, map[from.ToIndex()]);
	if (map[to.ToIndex()]->GetType() != FigureType::Empty)
	{
		info.SetEatenFigure(map[to.ToIndex()]);
		// decrease numOfFigures
	}
	map[to.ToIndex()] = map[from.ToIndex()];
	map[from.ToIndex()] = new Empty(Pos(from));
	movesHistory.push_back(info);
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
	Pos rookFrom;
	Pos rookTo;
	int y = (map[to.ToIndex()]->GetColor() == Color::Black) ? 7 : 0;
	if (from.GetX() > to.GetX())
	{
		rookFrom = Pos(0, y);
		rookTo = Pos(3, y);
	}
	else
	{
		rookFrom = Pos(7, y);
		rookTo = Pos(5, y);
	}
	ChangeCoordsForCastling(*(Rook*)map[rookFrom.ToIndex()], rookTo);
	Move(rookFrom, rookTo);
}

bool Map::CheckingShah(const Pos& kingPos)
{
	if (true) // numOfFigures
	{ // high // method FROM KING
		Pos selectedPosition;
		bool isChecked;
		FigureType selectedFigureType;
		for (int x, y, i = 0; i != 8; ++i)
		{
			x = 0; y = 0;
			isChecked = false;
			do 
			{
				/*
					2 | 3 | 4
					---------
					1 | K | 5
					---------
					0 | 7 | 6
				*/
				if (i == 0 || i == 1 || i == 2)
					--x;
				if (i == 4 || i == 5 || i == 6)
					++x;
				if (i == 0 || i == 7 || i == 6)
					++y;
				if (i == 2 || i == 3 || i == 4)
					--y;
				selectedPosition = kingPos.AddToX(x).AddToY(y);
				if (selectedPosition.IsValid())
				{
					selectedFigureType = GetFigureAt(selectedPosition)->GetType();
					if (selectedFigureType != FigureType::Empty)
					{
						if (GetFigureAt(selectedPosition)->GetColor() != GetFigureAt(kingPos)->GetColor())
						{
							if (selectedFigureType == FigureType::Queen_black || selectedFigureType == FigureType::Queen_white)
								return true;
							if (i % 2 && (selectedFigureType == FigureType::Rook_black || selectedFigureType == FigureType::Rook_white)) // 1, 3, 5, 7 
								return true;
							if (!(i % 2) && (selectedFigureType == FigureType::Bishop_black || selectedFigureType == FigureType::Bishop_white)) // 0, 2, 4, 6
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
		// pawn
		if (GetFigureAt(kingPos)->GetColor() == Color::White)
		{
			selectedPosition = kingPos.AddToX(-1).AddToY(1);
			if (selectedPosition.IsValid())
				if (GetFigureAt(selectedPosition)->GetType() == FigureType::Pawn_black)
					return true;
			selectedPosition = kingPos.AddToX(1).AddToY(1);
			if (selectedPosition.IsValid())
				if (GetFigureAt(selectedPosition)->GetType() == FigureType::Pawn_black)
					return true;
		}
		else
		{
			selectedPosition = kingPos.AddToX(-1).AddToY(-1);
			if (selectedPosition.IsValid())
				if (GetFigureAt(selectedPosition)->GetType() == FigureType::Pawn_white)
					return true;
			selectedPosition = kingPos.AddToX(1).AddToY(-1);
			if (selectedPosition.IsValid())
				if (GetFigureAt(selectedPosition)->GetType() == FigureType::Pawn_white)
					return true;
		}
		// knight
		for (int i = 0; i != 2; ++i)
		{
			selectedPosition = kingPos.AddToX(i % 2 + 1).AddToY((i + 1) % 2 + 1);
			if (selectedPosition.IsValid())
				if (GetFigureAt(kingPos)->GetColor() != GetFigureAt(selectedPosition)->GetColor())
					if (GetFigureAt(selectedPosition)->GetType() == FigureType::Knight_black || GetFigureAt(selectedPosition)->GetType() == FigureType::Knight_white)
						return true;
			selectedPosition = kingPos.AddToX(-(i % 2 + 1)).AddToY((i + 1) % 2 + 1);
			if (selectedPosition.IsValid())
				if (GetFigureAt(kingPos)->GetColor() != GetFigureAt(selectedPosition)->GetColor())
					if (GetFigureAt(selectedPosition)->GetType() == FigureType::Knight_black || GetFigureAt(selectedPosition)->GetType() == FigureType::Knight_white)
						return true;
			selectedPosition = kingPos.AddToX(i % 2 + 1).AddToY(-((i + 1) % 2 + 1));
			if (selectedPosition.IsValid())
				if (GetFigureAt(kingPos)->GetColor() != GetFigureAt(selectedPosition)->GetColor())
					if (GetFigureAt(selectedPosition)->GetType() == FigureType::Knight_black || GetFigureAt(selectedPosition)->GetType() == FigureType::Knight_white)
						return true;
			selectedPosition = kingPos.AddToX(-(i % 2 + 1)).AddToY(-((i + 1) % 2 + 1));
			if (selectedPosition.IsValid())
				if (GetFigureAt(kingPos)->GetColor() != GetFigureAt(selectedPosition)->GetColor())
					if (GetFigureAt(selectedPosition)->GetType() == FigureType::Knight_black || GetFigureAt(selectedPosition)->GetType() == FigureType::Knight_white)
						return true;
		}
		return false;
	}
	else
	{ // low // method TO KING

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

void ChangeCoordsForCastling(Rook& selectedRook, Pos newCoords)
{
	selectedRook.coords = newCoords;
	selectedRook.possibleCastling = false;
}