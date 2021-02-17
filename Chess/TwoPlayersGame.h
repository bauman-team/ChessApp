#pragma once
#include <TGUI/TGUI.hpp>
#include "Game.h"
#include "Player.h"

#include <thread>
#include <mutex>
//#include <ctime>
//#include <cstdlib>

class TwoPlayersGame : public Game
{
protected:
	Player* player1;
	Player* player2;
	Player* activePlayer;

	bool isTimeLimited;

	GameStatus virtual CheckGameFinal() override; // TODO: move to class Map

public:
	TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& _mapProperties);

	void virtual Show() override;
	void virtual ChangeActivePlayer() override;

	void virtual SetPosition(int mouseX, int mouseY) override;

	void virtual StartGame() override;

	void virtual SetPlayers(std::string name1, std::string name2, sf::Time timeLimit = sf::seconds(0));

	bool IsTimeLimited() const { return isTimeLimited; }

	// Methods for testing
	static void SpeedTestingOnProcessorThread(Map &map, Color activeColor, int count, std::atomic<int> &crucialCount);

	virtual ~TwoPlayersGame() override
	{
		delete player1;
		delete player2;
	}
};