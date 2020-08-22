#include "TwoPlayersGame.h"
#include "Figure.h"
#include <iostream>

TwoPlayersGame::TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: Game(window, resource, properties)
{
	SetPlayers();
	activePlayer = player1;
	map.RunFindMoves(activePlayer->GetColor());
}

void TwoPlayersGame::Show()
{
	drawer.ShowMap(map);
	drawer.ShowTimer(activePlayer->GetRemainingTime(), activePlayer->GetColor());
	if (activePlayer->HasTime())
	{
		if (activePlayer->GetChosenPosition() != nullptr)
		{
			drawer.ShowActiveFigure(map, *activePlayer->GetChosenPosition());
			drawer.ShowPossibleMoves(map, *activePlayer->GetChosenPosition());
		}
	}
}

void TwoPlayersGame::ChangeActivePlayer()
{
	map.RunClearPossibleMoves();
	drawer.ShowMap(map);
	drawer.ShowTimer(activePlayer->GetRemainingTime(), activePlayer->GetColor());
	drawer.DisplayWindow();
	//sf::sleep(sf::seconds(2));

	activePlayer = (activePlayer == player2) ? player1 : player2;
	activePlayer->SetChosenPosition(nullptr);
	map.RunFindMoves(activePlayer->GetColor());
	drawer.RotateBoard();
	activePlayer->StartTimer();
	isWin = CheckGameFinal();
}

void TwoPlayersGame::SetPlayerChosenCell(int mouseX, int mouseY)
{
	if (activePlayer->HasTime())
	{
		Pos* position = drawer.TransformMousePosition(mouseX, mouseY); // transform coords on window to position on map
		if (position != nullptr)
		{
			if (activePlayer->GetColor() == Color::Black)
			{
				*position = Pos(7 - position->GetX(), 7 - position->GetY());
			}
			if (position->IsValid()) // if position is correct
			{
				bool chosenPositionIsPossible = false;
				if (activePlayer->GetChosenPosition() != nullptr && // if chosen position exists and
					activePlayer->GetColor() != map.GetColor(*position)) // position and activePlayer colors aren't same
				{
					if (map.RunMakeMove(*activePlayer->GetChosenPosition(), *position))
					{
						chosenPositionIsPossible = true;
						ChangeActivePlayer();
					}
				}
				if (!chosenPositionIsPossible)
					activePlayer->SetChosenPosition(position);
			}
		}
	}
}

int8_t TwoPlayersGame::CheckGameFinal()
{
	Pos* kingPos = nullptr;
	for (int i = 0; i != 64 && !kingPos; ++i)
		if (map.GetColor(Pos::IndexToPosition(i)) == activePlayer->GetColor() &&
			(map.GetFigureType(Pos::IndexToPosition(i)) == FigureType::King_black ||
			map.GetFigureType(Pos::IndexToPosition(i)) == FigureType::King_white))
			kingPos = &Pos::IndexToPosition(i);
	if (map.CheckingShah(*kingPos))
	{
		map.SetCastling(activePlayer->GetColor(), false);
		if (!map.GetFigureWithAccessMoves().empty())
			return 0;
		return 1;
	}
	else
	{
		if (!map.GetFigureWithAccessMoves().empty())
			return 0;
		return 2;
	}
}

void TwoPlayersGame::SetPlayers(std::string name1, std::string name2, sf::Time timeLimit)
{
	player1 = new Player(Color::White, name1, timeLimit);
	player2 = new Player(Color::Black, name2, timeLimit);
}