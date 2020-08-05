#include "Player.h"

Pos* Player::chosenPos = nullptr;

/*int8_t Player::CheckGameFinal()
{
	if (Figure::ptrKing->CheckingShah())
		return Figure::ptrMap->CheckingMate() ? 1 : 0;
	else
		return Figure::ptrMap->CheckingPat() ? 2 : 0;
	return 0;
}*/

Player::Player(Color _color, std::string _name, sf::Time timeLimit) : color(_color), name(_name), remainingTime(timeLimit), timeIsUp(false)
{
	 numOfFigures = 16; // if need add SAVE MOD replace numOfFigures to params
}

void Player::RunFindMoves(Figure* choseFigure)
{
	if (!choseFigure->IsMovesFound())
		choseFigure->FindPossibleMoves();
}

void Player::RunMakeMove(Figure* choseFigure, Pos& nextPosition)
{
	choseFigure->MakeMoveTo(nextPosition);
}

void Player::RunClearPossibleMoves(Map& map)
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (map.GetFigureAt(Pos(i, j))->GetColor() == color)
			{
				map.GetFigureAt(Pos(i, j))->ClearPossibleMoves();
			}
		}
	}
}

void Player::StartTimer()
{
	clock.restart();
}

sf::Time Player::GetRemainingTime()
{
	remainingTime -= clock.restart();
	if (remainingTime.asSeconds() > 0)
		return remainingTime;
	else
	{
		timeIsUp = true;
		sf::seconds(0);
	}
}
