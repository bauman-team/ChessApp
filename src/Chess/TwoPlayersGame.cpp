#include "TwoPlayersGame.h"

CHESSENGINE_API extern std::mutex mut1;

TwoPlayersGame::TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: Game{ window, resource, properties }, isTimeLimited{ false } { }

void TwoPlayersGame::Show()
{
	drawer.WindowIsResized();
	mut1.lock();
	drawer.ShowMap(map);
	drawer.ShowSideMenu(map);
	mut1.unlock();

	if (isTimeLimited && !activePlayer->GetIsBot())
		drawer.ShowTimer(activePlayer->GetRemainingTime());
	if (activePlayer->HasTime())
	{
		mut1.lock();
		auto selectedPos = drawer.GetSelectedPositionFrom();
		if (selectedPos.IsValid() && !drawer.GetSelectedPositionTo().IsValid())
		{
			drawer.ShowActiveFigure(selectedPos, map);
			drawer.ShowPossibleMoves(selectedPos, map);
		}
		mut1.unlock();
	}
	else
		status = GameStatus::TimeIsOver;
}

void TwoPlayersGame::ChangeActivePlayer()
{
	auto stopTime{ isTimeLimited };
	isTimeLimited = false; // stop game timer

	mut1.lock();
	map.ClearPossibleMoves();
	drawer.ClearSelect();
	mut1.unlock();

	sf::sleep(sf::seconds(2));

	activePlayer = activePlayer == player2 ? player1 : player2;

	status = map.CheckGameFinal(activePlayer->GetColor());
	drawer.RotateBoard();
	if (stopTime && status != GameStatus::Pat && status != GameStatus::Mat)
	{
		activePlayer->StartTimer(); // restart game(move) clock
		isTimeLimited = stopTime;
	}
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
					mut1.lock();
					drawer.SetSelectedPositionTo(position, map);
					mut1.unlock();
				}
			}
			else // select position
				drawer.SetSelectedPositionFrom(position);
		}
		else if (drawer.GetSelectedPositionTo().IsValid())
		{
			mut1.lock();
			drawer.SetSelectedFigure(mouseX, mouseY);
			mut1.unlock();
		}
	}
}

void TwoPlayersGame::StartGame()
{
	map.FindAllPossibleMoves(activePlayer->GetColor());
	if (isTimeLimited)
		activePlayer->StartTimer();
}

void TwoPlayersGame::SetPlayers(std::string name1, std::string name2, sf::Time timeLimit)
{
	if (timeLimit != sf::seconds(0))
		isTimeLimited = true;
	player1 = new Player{ Color::White, name1, timeLimit };
	player2 = new Player{ Color::Black, name2, timeLimit };
	activePlayer = player1;
}

inline void TwoPlayersGame::SpeedTestingOnProcessorThread(Map& map, Color activeColor, int count, std::atomic<int>& crucialCount)
{
	for (auto i = 0; i < count; ++i)
	{
		map.FindAllPossibleMoves(activeColor);
		map.ClearPossibleMoves();
	}
	++crucialCount;
}