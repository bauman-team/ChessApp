#include "TwoPlayersGame.h"

TwoPlayersGame::TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: Game(window, resource, properties)
{
	activePlayer = player1;
	// TODO: add initialization for 2 players objects
}

void TwoPlayersGame::StartGame()
{
	drawer.ShowMap(map);
	//drawer.ShowActiveFigure();
	//drawer.ShowPossibleMoves();
}

void TwoPlayersGame::ChangeActivePlayer()
{
	activePlayer = (activePlayer == &player2) ? &player1 : &player2;
}

void TwoPlayersGame::SetPlayers(std::string name1, std::string name2, double timeOfGame)
{

}
