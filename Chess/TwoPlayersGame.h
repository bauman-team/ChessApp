#pragma once
#include "Game.h"
#include "Player.h"
#include "Resources.h"
#include "Map.h"
#include <string>
#include <thread>
#include <atomic>

class TwoPlayersGame : public Game
{
	
	Player* player1;
	Player* player2;
	Player* activePlayer;

	bool isTimeLimited;

	GameStatus virtual CheckGameFinal() override;

public:
	TwoPlayersGame(sf::RenderWindow* window, const Resources& resource, const MapProperties& _mapProperties);

	void virtual Show() override;
	void virtual ChangeActivePlayer() override;

	void virtual SetPlayerChosenCell(int, int) override;

	void virtual StartGame() override;

	void SetPlayers(std::string name1, std::string name2, sf::Time timeLimit = sf::seconds(0));

	bool IsTimeLimited() const { return isTimeLimited; }

	virtual ~TwoPlayersGame() override
	{
		delete player1;
		delete player2;
	}

	// Methods for testing
	static void SpeedTestingOnProcessorThread(Map &map, Color activeColor, int count, std::atomic<int> &crucialCount);
};

inline void TwoPlayersGame::SpeedTestingOnProcessorThread(Map &map, Color activeColor, int count, std::atomic<int> &crucialCount)
{
	for (int i = 0; i < count; ++i)
	{
		map.RunFindMoves(activeColor);
		map.RunClearPossibleMoves();
	}
	++crucialCount;
}