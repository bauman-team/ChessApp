#include "TwoPlayersGame.h"
#include "Figure.h"
#include <iostream>

TwoPlayersGame::TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: Game(window, resource, properties)
{
	SetPlayers();
	activePlayer = player1;
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
	map.RunClearPossibleMoves(activePlayer->GetColor());
	drawer.ShowMap(map);
	drawer.ShowTimer(activePlayer->GetRemainingTime(), activePlayer->GetColor());
	drawer.DisplayWindow();
	//sf::sleep(sf::seconds(2));

	activePlayer = (activePlayer == player2) ? player1 : player2;
	activePlayer->SetChosenPosition(nullptr);
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
					activePlayer->GetColor() != map.GetFigureAt(*position)->GetColor()) // position and activePlayer colors aren't same
				{
					if (map.RunMakeMove(map.GetFigureAt(*activePlayer->GetChosenPosition()), *position))
					{
						chosenPositionIsPossible = true;
						ChangeActivePlayer();
					}
				}
				if (!chosenPositionIsPossible)
				{
					activePlayer->SetChosenPosition(position);
					if (activePlayer->GetColor() == map.GetFigureAt(*position)->GetColor()) // if this figure is an active player
					{
						map.RunFindMoves(map.GetFigureAt(*position));
					}
				}
			}
		}
	}
}

int8_t TwoPlayersGame::CheckGameFinal()
{
	Pos* kingPos = nullptr;
	for (int i = 0; i != 64 && !kingPos; ++i)
		if (map.GetFigureAt(i)->GetColor() == activePlayer->GetColor() && (map.GetFigureAt(i)->GetType() == FigureType::King_black || map.GetFigureAt(i)->GetType() == FigureType::King_white))
			kingPos = &map.GetFigureAt(i)->GetPos();
	if (map.CheckingShah(*kingPos))
	{
		((King*)map.GetFigureAt(*kingPos))->SetCastling(false);
		for (int i = 0; i != 64; ++i)
			if (map.GetFigureAt(i)->GetColor() == activePlayer->GetColor())
			{
				map.RunFindMoves(map.GetFigureAt(i));
				if (!map.GetFigureAt(i)->GetPossibleMoves().empty())
					return 0;
			}
		return 1;
	}
	else
	{
		for (int i = 0; i != 64; ++i)
			if (map.GetFigureAt(i)->GetColor() == activePlayer->GetColor())
			{
				map.RunFindMoves(map.GetFigureAt(i));
				if (!map.GetFigureAt(i)->GetPossibleMoves().empty())
					return 0;
			}
		return 2;
	}
}

void TwoPlayersGame::SetPlayers(std::string name1, std::string name2, sf::Time timeLimit)
{
	player1 = new Player(Color::White, name1, timeLimit);
	player2 = new Player(Color::Black, name2, timeLimit);
}