#include "TwoPlayersGame.h"

std::mutex mut1;
std::mutex mut2;
std::mutex mut3;
std::mutex mut4;

TwoPlayersGame::TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: Game(window, resource, properties), isTimeLimited(false) {}

void TwoPlayersGame::Show()
{
	mut1.lock();
	drawer.ShowMap(map);
	mut1.unlock();
	mut3.lock();
	drawer.ShowGuiElems(gameGui);
	mut3.unlock();
	if (isTimeLimited && !activePlayer->GetIsBot())
		drawer.ShowTimer(activePlayer->GetRemainingTime(), activePlayer->GetColor());
	if (activePlayer->HasTime())
	{
		mut1.lock();
		Pos chosenPos = activePlayer->GetChosenPosition();
		if (chosenPos != Pos::NULL_POS)
		{
			drawer.ShowActiveFigure(map, chosenPos);
			drawer.ShowPossibleMoves(map, chosenPos);
		}
		mut1.unlock();
	}
	else
		status = GameStatus::TimeIsOver;
}

void TwoPlayersGame::ChangeActivePlayer()
{
	mut3.lock();
	UpdateSideMenu();
	mut3.unlock();
	mut1.lock();
	map.RunClearPossibleMoves();
	activePlayer->SetChosenPosition(Pos::NULL_POS);
	mut1.unlock();
	sf::sleep(sf::seconds(2));

	activePlayer = (activePlayer == player2) ? player1 : player2;

	/*sf::Clock clock; // test of speed algorithm calculation possible moves
	std::atomic<int> crucialCount = 0;
	int countOfThreads = 50, i = 0;
	while (crucialCount != countOfThreads)
	{
		for (; i != countOfThreads; ++i)
		{
			Map *copy = new Map(map);
			std::thread th(SpeedTestingOnProcessorThread, std::ref(*copy), activePlayer->GetColor(), 20000, std::ref(crucialCount));
			th.detach();
		}
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
		Pos position = drawer.TransformMousePosition(mouseX, mouseY); // transform coords on window to position on map
		if (position != Pos::NULL_POS)
		{
			if (activePlayer->GetColor() == Color::Black)
			{
				position = Pos(7 - position.GetX(), 7 - position.GetY());
			}
			if (position.IsValid()) // if position is correct
			{
				bool chosenPositionIsPossible = false;
				if (activePlayer->GetChosenPosition() != Pos::NULL_POS && // if chosen position exists and
					activePlayer->GetColor() != map.GetColor(position)) // position and activePlayer colors aren't same
				{
					if (map.RunMakeMove(activePlayer->GetChosenPosition(), position))
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

GameStatus TwoPlayersGame::CheckGameFinal()
{
	Pos kingPos = Pos::NULL_POS;
	for (int i = 0; i != 64 && kingPos == Pos::NULL_POS; ++i)
		if (map.GetColor(Pos::IndexToPosition(i)) == activePlayer->GetColor() &&
			(map.GetFigureType(Pos::IndexToPosition(i)) == FigureType::King_black ||
			map.GetFigureType(Pos::IndexToPosition(i)) == FigureType::King_white))
			kingPos = Pos::IndexToPosition(i);
	if (map.CheckingShah(kingPos))
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