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

Player::Player(Color _color, std::string _name, double _timer) : color(_color), name(_name), timer(_timer)
{
	 numOfFigures = 16; // if need add SAVE MOD replace numOfFigures to params
}

void Player::RunFindMoves(Figure* choseFigure)
{
	choseFigure->FindPossibleMoves();
}

void Player::RunMakeMove(Figure* choseFigure, Pos& currentPosition)
{
	choseFigure->MakeMoveTo(currentPosition);
}
