#include "PlayerWithAIGame.h"
#include "Menu.h"

extern std::mutex mu;

void PlayerWithAIGame::SetPlayers(std::string name1, std::string name2, sf::Time timeLimit)
{
	if (timeLimit != sf::seconds(0))
		isTimeLimited = true;
	isPlayerMoveFirst = (name2 == Menu::GetBotName());
	player1 = new Player(Color::White, name1, timeLimit);
	player2 = new Player(Color::Black, name2, timeLimit);
	activePlayer = (isPlayerMoveFirst) ? player1 : player2;
}

bool PlayerWithAIGame::SetPlayerChosenCell(int mouseX, int mouseY)
{
	return TwoPlayersGame::SetPlayerChosenCell(mouseX, mouseY);
	// bot functions
}

void PlayerWithAIGame::ChangeActivePlayer()
{
	mu.lock();
	if (isTimeLimited)
		drawer.ShowTimer(activePlayer->GetRemainingTime(), activePlayer->GetColor());
	activePlayer->SetChosenPosition(Pos::NULL_POS);
	mu.unlock();

	map.RunClearPossibleMoves();
	activePlayer = (activePlayer == player2) ? player1 : player2;
	map.RunFindMoves(activePlayer->GetColor());
	status = CheckGameFinal();
	//
	srand(std::time(NULL));
	int rand1 = rand() % map.GetFigureWithAccessMoves().size();
	//sf::sleep(sf::seconds(2));
	map.RunMakeMove(*map.GetFigureWithAccessMoves().at(rand1).figurePosition, map.GetFigureWithAccessMoves().at(rand1).possibleMoves->at(rand() % map.GetFigureWithAccessMoves().at(rand1).possibleMoves->size()));
	//
	map.RunClearPossibleMoves();
	activePlayer = (activePlayer == player2) ? player1 : player2;
	map.RunFindMoves(activePlayer->GetColor());
	status = CheckGameFinal();

	if (isTimeLimited)
		activePlayer->StartTimer();
}

void PlayerWithAIGame::StartGame()
{
	if (!isPlayerMoveFirst)
	{
		drawer.RotateBoard();
		ChangeActivePlayer();
	}
	TwoPlayersGame::StartGame();
}
