#pragma once
#include "Game.h"
#include "Player.h"
#include "Resources.h"
#include "Drawer.h"
#include "Map.h"

class TwoPlayersGame : public Game
{
	
	Player* player1;
	Player* player2;
	Player* activePlayer;

public:
	TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& _mapProperties);

	void virtual StartGame() override;
	void virtual ChangeActivePlayer() override;

	void virtual SetPlayerChosenCell(int, int) override;

	void SetPlayers(std::string name1 = "player1", std::string name2 = "player2", double timeOfGame = -1);
};