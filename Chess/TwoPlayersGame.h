#pragma once
#include "Game.h"
//#include "Player.h"
#include "Resources.h"
#include "Drawer.h"
#include "Map.h"

class TwoPlayersGame : public Game
{
	/*
	Player player1;
	Player player2;
	Player* activePlayer;
	*/
public:
	//TwoPlayersGame() {}
	TwoPlayersGame(sf::RenderWindow* window, const Resources& resource);
	//TwoPlayersGame(const Resources* resource);

	void virtual StartGame() override;
	void virtual ChangeActivePlayer() override;
};