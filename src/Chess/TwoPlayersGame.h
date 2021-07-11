#pragma once
#include "Game.h"
#include "Player.h"

//#include <thread>
#include <future>
#include <atomic> // for supporting linux
//#include <ctime>
//#include <cstdlib>

class Game;

class TwoPlayersGame : public Game
{
protected:
	std::unique_ptr<Player> player1;
	std::unique_ptr<Player> player2;
	std::unique_ptr<Player> activePlayer;

	bool isTimeLimited;
	mutable std::mutex mut;
public:
	TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& properties, GameMode mode, std::string name1, std::string name2, sf::Time timeLimit = sf::seconds(0));

	void virtual Show() override;
	void virtual ChangeActivePlayer();

	void ChangePlayer();

	void virtual SetPosition(int mouseX, int mouseY) override;

	void virtual StartGame() override;

	void virtual MakeUndoMove() override;

	bool IsTimeLimited() const { return isTimeLimited; }
};