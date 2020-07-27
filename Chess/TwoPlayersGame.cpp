#include "TwoPlayersGame.h"

TwoPlayersGame::TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties)
	: Game(window, resource, properties)
{
	// activePlayer = &player1;
	// TODO: add initialization for 2 players objects
}

void TwoPlayersGame::StartGame()
{
	drawer.ShowMap(map);
}

void TwoPlayersGame::ChangeActivePlayer()
{
	//activePlayer = (activePlayer == &player2) ? &player1 : &player2;
}
