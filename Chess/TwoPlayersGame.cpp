#include "TwoPlayersGame.h"
#include "Figure.h"
#include <iostream>

TwoPlayersGame::TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: Game(window, resource, properties)
{
	SetPlayers();
	activePlayer = player1;
}

void TwoPlayersGame::StartGame()
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
	activePlayer->RunClearPossibleMoves(map);
	drawer.ShowMap(map);
	drawer.ShowTimer(activePlayer->GetRemainingTime(), activePlayer->GetColor());
	drawer.DisplayWindow();
	sf::sleep(sf::seconds(2));

	activePlayer = (activePlayer == player2) ? player1 : player2;
	activePlayer->SetChosenPosition(nullptr);
	drawer.RotateBoard();
	activePlayer->StartTimer();
}

void TwoPlayersGame::SetPlayerChosenCell(int mouseX, int mouseY)
{
	Pos* position = drawer.TransformMousePosition(mouseX, mouseY); // transform coords on window to position on map
	if (activePlayer->GetColor() == Color::Black)
	{
		*position = Pos(7 - position->GetX(), 7 - position->GetY());
	}
	if (position->IsValid()) // if position is correct
	{
		bool chosenPositionIsPossible = false;
		if (activePlayer->GetChosenPosition() != nullptr && // if chosen position exists and
			activePlayer->GetColor() != map.GetFigureAt(*position)->GetColor()) // position and activePlayer colors aren't same
		{
			// compare this position with possible moves for last chosen figure
			std::vector<Pos>::const_iterator begin = map.GetFigureAt(*activePlayer->GetChosenPosition())->GetPossibleMoves().begin();
			std::vector<Pos>::const_iterator end = map.GetFigureAt(*activePlayer->GetChosenPosition())->GetPossibleMoves().end();
			for (std::vector<Pos>::const_iterator it = begin; it != end; ++it)
			{
				if (*it == *position)
				{
					chosenPositionIsPossible = true;
					activePlayer->RunMakeMove(map.GetFigureAt(*activePlayer->GetChosenPosition()), *position);
					ChangeActivePlayer();
					break; // if it possible delete
				}
			}
		}
		if (!chosenPositionIsPossible)
		{
			activePlayer->SetChosenPosition(position);
			if (activePlayer->GetColor() == map.GetFigureAt(*position)->GetColor()) // if this figure is an active player
			{
				activePlayer->RunFindMoves(map.GetFigureAt(*position));
			}
		}
	}
}

void TwoPlayersGame::SetPlayers(std::string name1, std::string name2, sf::Time timeLimit)
{
	player1 = new Player(Color::White, name1, timeLimit);
	player2 = new Player(Color::Black, name2, timeLimit);
}