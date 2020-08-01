#include "TwoPlayersGame.h"
#include "Figure.h"
#include <iostream>

TwoPlayersGame::TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: Game(window, resource, properties)
{
	SetPlayers();
	activePlayer = player1;
	// TODO: add initialization for 2 players objects
}

void TwoPlayersGame::StartGame()
{
	drawer.ShowMap(map);
	if (activePlayer->GetChosenPosition() != nullptr)
		drawer.ShowActiveFigure(map, *activePlayer->GetChosenPosition());
}

void TwoPlayersGame::ChangeActivePlayer()
{
	activePlayer = (activePlayer == player2) ? player1 : player2;
}

void TwoPlayersGame::SetPlayerChosenCell(int mouseX, int mouseY)
{
	Pos* position = drawer.TransformMousePosition(mouseX, mouseY);
	bool chosenPositionIsPossible = false;
	if (position->IsValid())
	{
		if (activePlayer->GetChosenPosition() != nullptr)
		{
			for (std::vector<Pos>::const_iterator it = map.GetFigureAt(*activePlayer->GetChosenPosition())->GetPossibleMoves().begin(); it != map.GetFigureAt(*activePlayer->GetChosenPosition())->GetPossibleMoves().end() && !chosenPositionIsPossible;)
			{
				if (*it == *position)
				{
					chosenPositionIsPossible = true;
					activePlayer->RunMakeMove(map.GetFigureAt(*activePlayer->GetChosenPosition()), *position);
					ChangeActivePlayer();
					break;
				}
				else
				{
					++it;
				}
			}
		}
		if (chosenPositionIsPossible)
		{
			activePlayer->SetChosenPosition(nullptr);
		}
		else
		{
			activePlayer->SetChosenPosition(position);
			if (activePlayer->GetColor() == map.GetFigureAt(*position)->GetColor())
			{
				activePlayer->RunFindMoves(map.GetFigureAt(*position));
			}
		}
	}
}

void TwoPlayersGame::SetPlayers(std::string name1, std::string name2, double timeOfGame)
{
	player1 = new Player(Color::White, name1, timeOfGame);
	player2 = new Player(Color::Black, name2, timeOfGame);
}
