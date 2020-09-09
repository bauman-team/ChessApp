#include "PlayerWithAIGame.h"

void PlayerWithAIGame::SetPlayers(std::string name, bool _isPlayerMoveFirst, sf::Time timeLimit)
{
	/*if (timeLimit != sf::seconds(0))
		isTimeLimited = true;*/
	isPlayerMoveFirst = _isPlayerMoveFirst;
	if (_isPlayerMoveFirst)
	{
		player1 = new Player(Color::White, name, timeLimit);
		player2 = new Player(Color::Black, "bot", timeLimit);
		activePlayer = player1;
	}
	else
	{
		player1 = new Player(Color::White, "bot", timeLimit);
		player2 = new Player(Color::Black, name, timeLimit);
		activePlayer = player2;
	}
}

bool PlayerWithAIGame::SetPlayerChosenCell(int mouseX, int mouseY)
{
	return TwoPlayersGame::SetPlayerChosenCell(mouseX, mouseY);
	// bot functions
}

void PlayerWithAIGame::ChangeActivePlayer()
{
	if (isTimeLimited)
		drawer.ShowTimer(activePlayer->GetRemainingTime(), activePlayer->GetColor());
	activePlayer->SetChosenPosition(Pos::NULL_POS);

	map.RunClearPossibleMoves();
	activePlayer = (activePlayer == player2) ? player1 : player2;
	status = CheckGameFinal();
	map.RunFindMoves(activePlayer->GetColor());
	//
	srand(std::time(NULL));
	int rand1 = rand() % map.GetFigureWithAccessMoves().size();
	//sf::sleep(sf::seconds(2));
	map.RunMakeMove(*map.GetFigureWithAccessMoves().at(rand1).figurePosition, map.GetFigureWithAccessMoves().at(rand1).possibleMoves->at(rand() % map.GetFigureWithAccessMoves().at(rand1).possibleMoves->size()));
	//
	map.RunClearPossibleMoves();
	activePlayer = (activePlayer == player2) ? player1 : player2;
	status = CheckGameFinal();
	map.RunFindMoves(activePlayer->GetColor());

	if (isTimeLimited)
		activePlayer->StartTimer();
}

void PlayerWithAIGame::StartGame()
{
	Game::StartGame();
	if (!isPlayerMoveFirst)
	{
		ChangeActivePlayer();
	}
	else
	{
		map.RunFindMoves(activePlayer->GetColor());
		if (isTimeLimited)
			activePlayer->StartTimer();
	}
}
