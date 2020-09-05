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

void PlayerWithAIGame::SetPlayerChosenCell(int mouseX, int mouseY)
{
	TwoPlayersGame::SetPlayerChosenCell(mouseX, mouseY);
	// bot functions
}

void PlayerWithAIGame::ChangeActivePlayer()
{
	TwoPlayersGame::ChangeActivePlayer(); // change
}

void PlayerWithAIGame::StartGame()
{
	Game::StartGame();
	map.RunFindMoves(activePlayer->GetColor());
	if (isTimeLimited)
		activePlayer->StartTimer();
}
