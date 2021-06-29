#pragma once
#include "Game.h"
#include "Player.h"

#include <thread>
#include <mutex>
#include <atomic> // for supporting linux
//#include <ctime>
//#include <cstdlib>

class Game;

class TwoPlayersGame : public Game
{
protected:
	Player* player1;
	Player* player2;
	Player* activePlayer;

	bool isTimeLimited;

public:
	TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties, GameMode mode)
		: Game{ window, resource, properties, mode }, isTimeLimited{ false } { }

	void virtual Show() override;
	void virtual ChangeActivePlayer() override;

	void virtual SetPosition(int mouseX, int mouseY) override;

	void virtual StartGame() override;

	void virtual SetPlayers(std::string name1, std::string name2, sf::Time timeLimit = sf::seconds(0)) override;

	void virtual MakeUndoMove() override;

	bool IsTimeLimited() const { return isTimeLimited; }

	virtual ~TwoPlayersGame() override
	{
		delete player1;
		delete player2;
	}
};