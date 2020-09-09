#include "PlayerWithAIGame.h"

void PlayerWithAIGame::SetPlayers(std::string name, bool _isPlayerMoveFirst, sf::Time timeLimit)
{
	/*if (timeLimit != sf::seconds(0))
		isTimeLimited = true;*/
	std::string name1, name2;
	isPlayerMoveFirst = _isPlayerMoveFirst;
	if (_isPlayerMoveFirst)
	{
		name1 = name;
		name2 = "bot";
	}
	else
	{
		name1 = "bot";
		name2 = name;
	}
	player1 = new Player(Color::White, name1, timeLimit);
	player2 = new Player(Color::Black, name2, timeLimit);
	activePlayer = player1;
}

bool PlayerWithAIGame::SetPlayerChosenCell(int mouseX, int mouseY)
{
	return TwoPlayersGame::SetPlayerChosenCell(mouseX, mouseY);
	// bot functions
}

void PlayerWithAIGame::ChangeActivePlayer()
{
	//TwoPlayersGame::ChangeActivePlayer(); // change
	map.RunClearPossibleMoves();
	//drawer.ShowMap(map);
	if (isTimeLimited)
		drawer.ShowTimer(activePlayer->GetRemainingTime(), activePlayer->GetColor());
	activePlayer->SetChosenPosition(Pos::NULL_POS);
	//drawer.DisplayWindow();
	

	activePlayer = (activePlayer == player2) ? player1 : player2;
	status = CheckGameFinal();

	map.RunFindMoves(activePlayer->GetColor());

	srand(std::time(NULL));

	int rand1 = rand() % map.GetFigureWithAccessMoves().size();
	//sf::sleep(sf::seconds(2));
	map.RunMakeMove(*map.GetFigureWithAccessMoves().at(rand1).figurePosition, map.GetFigureWithAccessMoves().at(rand1).possibleMoves->at(rand() % map.GetFigureWithAccessMoves().at(rand1).possibleMoves->size()));

	map.RunClearPossibleMoves();
	activePlayer = (activePlayer == player2) ? player1 : player2;
	map.RunFindMoves(activePlayer->GetColor());
	

	status = CheckGameFinal();
	if (isTimeLimited)
		activePlayer->StartTimer();
}

void PlayerWithAIGame::StartGame()
{
	Game::StartGame();
	map.RunFindMoves(activePlayer->GetColor());
	if (isTimeLimited)
		activePlayer->StartTimer();
}
