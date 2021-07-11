#include "TwoPlayersGame.h"

TwoPlayersGame::TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties, GameMode mode, std::string name1, std::string name2, sf::Time timeLimit)
	: Game{ window, resource, properties, mode }, isTimeLimited{ false } 
{
	if (mode == GameMode::TwoPlayers)
	{
		if (timeLimit != sf::seconds(0))
			isTimeLimited = true;
		player1 = std::make_unique<Player>(Color::White, name1, timeLimit);
		player2 = std::make_unique<Player>(Color::Black, name2, timeLimit);
		activePlayer = std::move(player1);
	}
}

void TwoPlayersGame::Show()
{
	drawer.WindowIsResized();
	map.mut.lock();
	drawer.ShowMap(map);
	drawer.ShowSideMenu(map);
	map.mut.unlock();

	if (isTimeLimited && !activePlayer->GetIsBot())
		drawer.ShowTimer(activePlayer->GetRemainingTime());
	if (activePlayer->HasTime())
	{
		map.mut.lock();
		auto selectedPos = drawer.GetSelectedPositionFrom();
		if (selectedPos.IsValid() && !drawer.GetSelectedPositionTo().IsValid())
		{
			drawer.ShowActiveFigure(selectedPos, map);
			drawer.ShowPossibleMoves(selectedPos, map);
		}
		map.mut.unlock();
	}
	else
		status = GameStatus::TimeIsOver;
}

void TwoPlayersGame::ChangeActivePlayer()
{
	auto stopTime{ isTimeLimited };
	isTimeLimited = false; // stop game timer

	map.mut.lock();
	map.ClearPossibleMoves();
	drawer.ClearSelect();
	map.mut.unlock();

	sf::sleep(sf::seconds(2));

	ChangePlayer();

	status = map.CheckGameFinal(activePlayer->GetColor());
	drawer.RotateBoard();
	if (stopTime && status != GameStatus::Pat && status != GameStatus::Mat)
	{
		activePlayer->StartTimer(); // restart game(move) clock
		isTimeLimited = stopTime;
	}
}

void TwoPlayersGame::ChangePlayer()
{
	std::unique_ptr<Player> tempPlayer;
	if (player1)
	{
		tempPlayer = std::move(player1);
		player2 = std::move(activePlayer);
	}
	else
	{
		tempPlayer = std::move(player2);
		player1 = std::move(activePlayer);
	}
	activePlayer = std::move(tempPlayer);
}

void TwoPlayersGame::SetPosition(int mouseX, int mouseY)
{
	if (activePlayer->HasTime())
	{
		auto position{ drawer.TransformMousePosition(mouseX, mouseY) }; // transform coords on window to position on map
		if (position.IsValid() && // if position is correct
			(activePlayer->GetColor() == Color::Black ? // rotate for black
				position = Pos{ static_cast<uint8_t>(7 - position.GetX()), static_cast<uint8_t>(7 - position.GetY()) } : position,
				!drawer.GetSelectedPositionTo().IsValid() || drawer.GetSelectedPositionTo() == position))
		{
			if (drawer.GetSelectedPositionFrom().IsValid()) // if chosen position exists
			{
				MoveStatus status = map.MakeMove(drawer.GetSelectedPositionFrom(), position, drawer.GetSelectedFigure()); // try to move
				if (status == MoveStatus::Made)
					ChangeActivePlayer();
				else if (status == MoveStatus::NotFound)
				{
					drawer.ClearSelect();
					drawer.SetSelectedPositionFrom(position);
				}
				else
				{
					map.mut.lock();
					drawer.SetSelectedPositionTo(position, map);
					map.mut.unlock();
				}
			}
			else // select position
				drawer.SetSelectedPositionFrom(position);
		}
		else if (drawer.GetSelectedPositionTo().IsValid())
		{
			map.mut.lock();
			drawer.SetSelectedFigure(mouseX, mouseY);
			map.mut.unlock();
		}
	}
}

void TwoPlayersGame::StartGame()
{
	map.FindAllPossibleMoves(activePlayer->GetColor());
	if (isTimeLimited)
		activePlayer->StartTimer();
}

void TwoPlayersGame::MakeUndoMove()
{
	mut.lock();
	if (map.GetMovesCount() > 1)
	{
		map.mut.lock();
		map.UndoMove();
		map.UndoMove();
		map.ClearPossibleMoves();
		drawer.ClearSelect();
		map.mut.unlock();
		status = map.CheckGameFinal(activePlayer->GetColor());
	}
	mut.unlock();
}
