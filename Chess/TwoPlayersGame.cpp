#include "TwoPlayersGame.h"
#include "Figure.h"
#include <iostream>

TwoPlayersGame::TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: Game(window, resource, properties), isTimeLimited(false) {}

void TwoPlayersGame::Show()
{
	drawer.ShowMap(map);
	if (isTimeLimited)
		drawer.ShowTimer(activePlayer->GetRemainingTime(), activePlayer->GetColor());
	if (activePlayer->HasTime())
	{
		if (activePlayer->GetChosenPosition() != Pos::NULL_POS)
		{
			drawer.ShowActiveFigure(map, activePlayer->GetChosenPosition());
			drawer.ShowPossibleMoves(map, activePlayer->GetChosenPosition());
		}
	}
	else
	{
		status = GameStatus::TimeIsOver;
	}
}

void TwoPlayersGame::ChangeActivePlayer()
{
	map.RunClearPossibleMoves();
	drawer.ShowMap(map);
	if (isTimeLimited)
		drawer.ShowTimer(activePlayer->GetRemainingTime(), activePlayer->GetColor());
	drawer.DisplayWindow();
	//sf::sleep(sf::seconds(2));

	activePlayer = (activePlayer == player2) ? player1 : player2;
	activePlayer->SetChosenPosition(Pos::NULL_POS);

	/*sf::Clock clock;
	for (int i = 0; i < 90000; ++i)
	{
		map.RunFindMoves(activePlayer->GetColor());
		map.RunClearPossibleMoves();
	}
	sf::Time time = clock.getElapsedTime();*/

	map.RunFindMoves(activePlayer->GetColor());
	drawer.RotateBoard();
	status = CheckGameFinal();
	if (isTimeLimited)
		activePlayer->StartTimer();
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
				if (activePlayer->GetChosenPosition() != Pos::NULL_POS && // if chosen position exists and
					activePlayer->GetColor() != map.GetColor(*position)) // position and activePlayer colors aren't same
				{
					if (map.RunMakeMove(activePlayer->GetChosenPosition(), *position))
					{
						chosenPositionIsPossible = true;
						ChangeActivePlayer();
					}
				}
				if (!chosenPositionIsPossible)
					activePlayer->SetChosenPosition(*position);
			}
			delete position;
		}
	}
}

GameStatus TwoPlayersGame::CheckGameFinal()
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
			return GameStatus::Shah;
		return GameStatus::Mat;
	}
	else
	{
		if (!map.GetFigureWithAccessMoves().empty())
			return GameStatus::Play;
		return GameStatus::Pat;
	}
}

void TwoPlayersGame::StartGame()
{
	Game::StartGame();
	map.RunFindMoves(activePlayer->GetColor());
	if (isTimeLimited)
		activePlayer->StartTimer();
}

void TwoPlayersGame::SetPlayers(std::string name1, std::string name2, sf::Time timeLimit)
{
	if (timeLimit != sf::seconds(0))
		isTimeLimited = true;
	player1 = new Player(Color::White, name1, timeLimit);
	player2 = new Player(Color::Black, name2, timeLimit);
	activePlayer = player1;
}